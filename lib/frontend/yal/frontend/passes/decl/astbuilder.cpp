/*
 *  Copyright 2018 by Leander Beernaert (lbb-dev@pm.me)
 *
 *  This file is part of YAL.
 *
 *  YAL is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, either version 3
 *  of the License, or (at your option) any later version.
 *
 *  YAL is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with YAL. If not, see <http://www.gnu.org/licenses/>.
 */

#include "yal/frontend/passes/decl/astbuilder.h"

#include "yal/error/errorreporter.h"
#include "yal/frontend/ast/astnodes.h"
#include "yal/frontend/parser/sttype.h"
#include "yal/frontend/parser/syntaxtreenodes.h"
#include "yal/frontend/parser/syntaxtreevisitorimpl.h"
#include "yal/frontend/passes/passes.h"
#include "yal/frontend/passes/decl/errorspassdecl.h"
#include "yal/frontend/types/types.h"
#include "yal/frontend/module.h"
#include "yal/frontend/modulemanager.h"
#include "yal/util/scopedstackelem.h"
#include "yal/util/strconversions.h"
#include "yal/util/stackjump.h"

#include <limits>
#include <stack>
#include <vector>

namespace yal::frontend {

    static Qualifier MakeQualifier(const STQualType& stqt) {
        Qualifier qual;
        if (stqt.m_qualifiers & STQualType::kQualReference) {
            qual.setReference();
        }

        if (stqt.m_qualifiers & STQualType::kQualMutable) {
            qual.setMutable();
        }

        return qual;
    }

    class AstBuilder::ScopeGuard {
    public:
        YAL_NO_COPY_CLASS(ScopeGuard);

        ScopeGuard(AstBuilder& visitor,
                   const AstBuilder::ScopedState& newState):
            m_visitor(visitor),
            m_prevState(visitor.m_scopedState) {
            m_visitor.m_scopedState = newState;
        }

        ~ScopeGuard() {
            m_visitor.m_scopedState = m_prevState;
        }

    private:
        AstBuilder& m_visitor;
        AstBuilder::ScopedState m_prevState;
    };

    template <typename T>
    using StackType = std::stack<T, std::vector<T>>;
    using StackDecl = StackType<DeclBase*>;
    using StackStatement = StackType<Statement*>;
    using StackExpr = StackType<StmtExpression*>;
    using StackScope = StackType<DeclScope*>;

    struct AstBuilder::State {
        StackJump jump;
        StackStatement stackStatements;
        StackExpr stackExpressions;
        StackDecl stackDecls;
        StackScope stackScope;

        void onError() {
            jump.trigger();
        }
    };


    AstBuilder::AstBuilder(PassOptions& options)
        : m_errReporter(options.errReporter)
        , m_module(options.module)
        , m_moduleManager(options.modManager)
        , m_state(nullptr) {

    }

    void
    AstBuilder::visit(const STDeclModule& node) {
        DeclModule* declModule = m_module.getDeclNode();
        ScopedStackElem<StackScope> sscope(getState().stackScope,
                                           &declModule->getModuleDeclScope());
        // process nodes
        for (auto& decl : node.getDecls()) {
            resolve(*decl);
        }

        // collect ast decls
        StackDecl& stackDecls = getState().stackDecls;
        DeclModule::Decls decls(DeclModule::Decls::allocator_type(m_module.getASTContext().getAllocator()));
        decls.resize(stackDecls.size(), nullptr);
        for(size_t idx = decls.size() -1 ; !stackDecls.empty(); --idx) {
            DeclBase* decl = stackDecls.top();
            decls[idx] = decl;
            stackDecls.pop();
        }
        declModule->setDecls(std::move(decls));
    }

    void
    AstBuilder::visit(const STDeclStruct& node) {
        const StringRef declName = node.getName().getString();
        TypeContext& typeCtx = m_module.getTypeContext();
        ASTContext& astCtx = m_module.getASTContext();
        DeclScope* parentScope = getState().stackScope.top();

        const Identifier identifier(declName, m_module);

        // check if type has been declared
        Type* type = typeCtx.getByIdentifier(identifier);
        if (type == nullptr) {
            onUndefinedType(node);
        }

        {
            // check for existing struct decl
            const DeclNamed* existingDecl = parentScope->getDecl(type->getIdentifier(), false);
            if (existingDecl != nullptr ) {
                onDuplicateSymbol(node, *existingDecl);
            }
        }

        TypeStruct* typeStruct = dyn_cast<TypeStruct>(type);
        YAL_ASSERT(typeStruct != nullptr);

        // Create Decl
        DeclStruct* declStruct = astCtx.getAllocator()
                .construct<DeclStruct>(m_module,
                                       typeStruct,
                                       *parentScope);

        DeclScope& declScopeStruct = declStruct->getStructDeclScope();

        DeclStruct::Members members(DeclStruct::Members::allocator_type(astCtx.getAllocator()));
        members.reserve(node.getMembers().size());

        // process each struct member
        for (auto& member : node.getMembers()) {

            const STIdentifier& name = member->getName();
            const STQualType& stqt = member->getType();

            // can't use mutability qualifier on struct members
            if (stqt.m_qualifiers & STQualType::kQualMutable) {
                auto err = std::make_unique<ErrorInvaldMutabilityQualifierUsage>(*member);
                m_errReporter.report(std::move(err));
                getState().onError();
            }

            // check for duplicate decl
            const Identifier memberId(name.getString());
            const DeclNamed* existingDecl = declScopeStruct.getDecl(memberId,false);
            if (existingDecl != nullptr ) {
                onDuplicateSymbol(name, *existingDecl);
            }

            // check if type exists
            Type* memberType = resolveType(*stqt.m_type);
            if (memberType == nullptr) {
                onUndefinedType(*stqt.m_type);
            }

            Qualifier qual = MakeQualifier(stqt);
            qual.setMutable();
            QualType qt = QualType::Create(qual, memberType);

            DeclVar* declVar = astCtx.getAllocator()
                    .construct<DeclVar>(m_module,
                                        name.getString(),
                                        name.getSourceInfo(),
                                        declScopeStruct,
                                        qt,
                                        nullptr);
            declScopeStruct.addDecl(declVar);
            members.push_back(declVar);
        }

        typeStruct->setDecl(*declStruct);
        declStruct->setMembers(std::move(members));

        // push to decl stack
        getState().stackDecls.push(declStruct);
    }


    void
    AstBuilder::visit(const STDeclTypeFunctions& node) {
        DeclScope* parentScope = getState().stackScope.top();

        // check if target type has been declared
        const STType& stTargetType = node.getType();
        Type* targetType = resolveType(stTargetType);
        if (targetType == nullptr) {
            onUndefinedType(stTargetType);
        }
        YAL_ASSERT(targetType->isFunctionTargetable());

        DeclTypeFunctions* typeFunctions =m_module.getASTContext().getAllocator()
                .construct<DeclTypeFunctions>(m_module,
                                              *parentScope,
                                              node.getSourceInfo(),
                                              *targetType);

        ScopedState newState;
        newState.implDeclType = targetType;
        ScopeGuard guard(*this, newState);

        const STDeclTypeFunctions::Decls& stDecls = node.getDecls();
        DeclTypeFunctions::Decls decls(DeclTypeFunctions::Decls::allocator_type(m_module.getASTContext().getAllocator()));
        decls.reserve(stDecls.size());

        ScopedStackElem<StackScope> sscope(getState().stackScope,
                                           &typeFunctions->getImplScope());
        for (auto& decl : stDecls) {
            resolve(*decl);
            DeclBase* declType = getState().stackDecls.top();
            YAL_ASSERT(declType != nullptr);
            getState().stackDecls.pop();
            DeclTypeFunction* declTypeFn = dyn_cast<DeclTypeFunction>(declType);
            YAL_ASSERT(declTypeFn != nullptr);
            decls.push_back(declTypeFn);
        }

        typeFunctions->setDecls(std::move(decls));

        // push to decl stack
        getState().stackDecls.push(typeFunctions);
    }

    void
    AstBuilder::visit(const STDeclFunction& node) {
        TypeContext& typeCtx = m_module.getTypeContext();
        ASTContext& astCtx = m_module.getASTContext();
        Type* targetType = m_scopedState.implDeclType;
        DeclScope* parentScope = getState().stackScope.top();

        // check if type has been declared
        Identifier fnId = TypeFunction::CreateIdentitier(m_module, &node, targetType);
        Type* type = typeCtx.getByIdentifier(fnId);
        if (type == nullptr) {
            onUndefinedType(node);
        }

        {
            // check for existing decl
            const DeclNamed* existingDecl = parentScope->getDecl(type->getIdentifier(), false);
            if (existingDecl != nullptr ) {
                onDuplicateSymbol(node, *existingDecl);
            }
        }

        // check return type
        QualType returnType;
        const STQualType* stReturnType = node.getReturnType();
        if (stReturnType != nullptr) {
            // check if target type has been declared
            const STType* returnStType = stReturnType->m_type;
            Type* returnTypePtr = resolveType(*returnStType);
            if (returnTypePtr == nullptr) {
                onUndefinedType(*returnStType);
            }

            // Return values of function are treated as rvalues
            Qualifier qualReturn = MakeQualifier(*stReturnType);
            qualReturn.setLValue(false);
            returnType = QualType::Create(qualReturn,
                                          returnTypePtr);
        }

        TypeFunction* typeFn = dyn_cast<TypeFunction>(type);
        YAL_ASSERT(typeFn != nullptr);

        // Create Decl
        DeclFunction* declFn = nullptr;
        if (targetType == nullptr) {
            declFn = astCtx.getAllocator()
                    .construct<DeclFunction>(m_module,
                                             typeFn,
                                             *getState().stackScope.top(),
                                             returnType);
        } else {
            QualType targeQt = QualType::Create(Qualifier(), targetType);
            declFn = astCtx.getAllocator()
                    .construct<DeclTypeFunction>(m_module,
                                                 typeFn,
                                                 *getState().stackScope.top(),
                                                 returnType,
                                                 targeQt);
        }

        parentScope->addDecl(declFn);
        DeclScope& declScopeFn = declFn->getFunctionScope();

        // Process function parameters
        const STDeclFunction::Params& stParams = node.getParams();
        if (!stParams.empty()) {

            DeclFunction::Params params(DeclFunction::Params::allocator_type(astCtx.getAllocator()));
            params.reserve(stParams.size());

            // process each struct member
            size_t idx = 0;
            for (auto& param : stParams) {

                const STIdentifier& name = param->getName();
                const STQualType& stqt = param->getType();

                // check for duplicate decl
                const DeclNamed* existingDecl = declScopeFn.getDecl(Identifier(name.getString()),false);
                if (existingDecl != nullptr ) {
                    onDuplicateSymbol(name, *existingDecl);
                }

                DeclParamVar* declVar = nullptr;

                if (name.getString() == "self") {

                    // can only use self as 1st param
                    if (idx != 0) {
                        auto err = std::make_unique<ErrorInvalidLocationForSelf>(*typeFn,
                                                                                 name.getSourceInfo());
                        m_errReporter.report(std::move(err));
                        getState().onError();
                    }

                    // can only use self if in a target function
                    if (targetType == nullptr) {
                        auto err = std::make_unique<ErrorInvalidUseOfSelf>(*typeFn,
                                                                           name.getSourceInfo());
                        m_errReporter.report(std::move(err));
                        getState().onError();
                    }

                    QualType qt = QualType::Create(MakeQualifier(stqt), targetType);

                    declVar = astCtx.getAllocator()
                            .construct<DeclParamVarSelf>(m_module,
                                                         name.getSourceInfo(),
                                                         declScopeFn,
                                                         qt);
                } else {

                    // check if type exists
                    Type* memberType = resolveType(*stqt.m_type);
                    if (memberType == nullptr) {
                        onUndefinedType(*stqt.m_type);
                    }

                    QualType qt = QualType::Create(MakeQualifier(stqt), memberType);

                    declVar = astCtx.getAllocator()
                            .construct<DeclParamVar>(m_module,
                                                     name.getString(),
                                                     name.getSourceInfo(),
                                                     declScopeFn,
                                                     qt);
                }
                declScopeFn.addDecl(declVar);
                params.push_back(declVar);
                ++idx;
            }

            declFn->setParams(std::move(params));
        }
        typeFn->setDecl(declFn);

        const STDeclFunction::Body& body = node.getBody();
        if (!body.empty()) {

            StatementList statements(StatementList::allocator_type(m_module.getASTContext().getAllocator()));
            StackStatement& stack = getState().stackStatements;
            const size_t currStmtStackSize = stack.size();

            ScopedStackElem<StackScope> sscope(getState().stackScope,
                                               &declScopeFn);

            // process statements
            for(auto& stmt : body) {
                resolve(*stmt);
            }

            // collect statements
            statements.resize(body.size(), nullptr);
            for(size_t idx = statements.size() -1 ;
                !stack.empty() && stack.size() >= currStmtStackSize; --idx) {
                Statement* stmt = stack.top();
                statements[idx] = stmt;
                stack.pop();
            }
            YAL_ASSERT(statements.size() == body.size());
            declFn->setBody(std::move(statements));
        }

        // push to decl stack
        getState().stackDecls.push(declFn);
    }

    void
    AstBuilder::visit(const STDeclImport& node) {
        const Module* moduleImported =
                m_moduleManager.getModuleByName(node.getModuleIdentifier());
        YAL_ASSERT(moduleImported != nullptr);

        DeclImport* declImport = m_module.getASTContext().getAllocator()
                .construct<DeclImport>(m_module,
                                       *getState().stackScope.top(),
                                       node.getSourceInfo(),
                                       *moduleImported);

        getState().stackDecls.push(declImport);
    }

    void
    AstBuilder::visit(const STDeclVar& node) {
        const STIdentifier& declName = node.getName();
        DeclScope* parentScope = getState().stackScope.top();

        // check duplicate symbol
        const DeclNamed* existingDecl = parentScope->getDecl(Identifier(declName.getString()), false);
        if (existingDecl != nullptr ) {
            onDuplicateSymbol(node, *existingDecl);
        }

        // check if type exists
        QualType qt;
        const STQualType* stqt = node.getType();
        if (stqt != nullptr) {

            // can't use mutability qualifier on declvar
            if (stqt->m_qualifiers & STQualType::kQualMutable) {
                auto err = std::make_unique<ErrorInvaldMutabilityQualifierUsage>(node);
                m_errReporter.report(std::move(err));
                getState().onError();
            }

            Type* memberType = resolveType(*stqt->m_type);
            if (memberType == nullptr) {
                onUndefinedType(*stqt->m_type);
            }
            Qualifier qualifier = MakeQualifier(*stqt);
            if (node.isImmutable()) {
                qualifier.setImmutable();
            } else {
                qualifier.setMutable();
            }
            qt = QualType::Create(qualifier, memberType);
        } else {
            Qualifier qualifier;
            if (node.isImmutable()) {
                qualifier.setImmutable();
            } else {
                qualifier.setMutable();
            }
            qt = QualType::Create(qualifier, nullptr);
        }

        // get init expr if any
        StmtExpression* initExpr = nullptr;
        if (const STExpression* expr = node.getExpression(); expr != nullptr) {
            StackExpr& stackExpr = getState().stackExpressions;
            const size_t stackSize = stackExpr.size();
            resolve(*expr);
            if (stackExpr.size() == stackSize + 1) {
                initExpr = stackExpr.top();
                stackExpr.pop();
            } else {
                YAL_ASSERT_MESSAGE(false, "Failed to populate expr stack");
            }
        }

        DeclVar* decl = m_module.getASTContext().getAllocator()
                .construct<DeclVar>(m_module,
                                    declName.getString(),
                                    node.getSourceInfo(),
                                    *parentScope,
                                    qt,
                                    initExpr);
        parentScope->addDecl(decl);
        getState().stackDecls.push(decl);
    }


    void
    AstBuilder::visit(const STDeclAlias& node) {
        const STType& aliasedType = node.getAliasedType();

        DeclScope* parentScope = getState().stackScope.top();
        YAL_ASSERT(parentScope != nullptr);

        // check if aliased type has been declared
        Type* type = resolveType(aliasedType);
        if (type == nullptr) {
            onUndefinedType(aliasedType);
        }

        // check if alias type has been declared
        const Identifier aliasId(node.getName().getString(), m_module);
        Type* aliasType  = m_module.getTypeContext().getByIdentifier(aliasId);
        if (aliasType == nullptr) {
            onUndefinedType(aliasedType);
        }

        if (aliasType->isAliasWeak()) {
            TypeAliasWeak* typeAliasWeak = dyn_cast<TypeAliasWeak>(aliasType);
            YAL_ASSERT(typeAliasWeak != nullptr);

            DeclAliasWeak* decl = m_module.getASTContext().getAllocator()
                    .construct<DeclAliasWeak>(m_module,
                                              *parentScope,
                                              *typeAliasWeak);
            getState().stackDecls.push(decl);
        }
        else if (aliasType->isAliasStrong()) {
            TypeAliasStrong* typeAliasStrong = dyn_cast<TypeAliasStrong>(aliasType);
            YAL_ASSERT(typeAliasStrong != nullptr);
            DeclAliasStrong* decl = m_module.getASTContext().getAllocator()
                    .construct<DeclAliasStrong>(m_module,
                                                *parentScope,
                                                *typeAliasStrong);
            getState().stackDecls.push(decl);
        }
        else
        {
            YAL_ASSERT_MESSAGE(false, "Shouldn't be reached!");
        }
    }

    void
    AstBuilder::visit(const STStmtReturn& node) {

        StmtExpression* retExpr = nullptr;
        if (const STExpression* expr = node.getExpr(); expr != nullptr) {
            StackExpr& stackExpr = getState().stackExpressions;
            const size_t stackSize = stackExpr.size();
            resolve(*expr);
            if (stackExpr.size() == stackSize + 1) {
                retExpr = stackExpr.top();
                stackExpr.pop();
            } else {
                YAL_ASSERT_MESSAGE(false, "Failed to populate expr stack");
            }
        }

        StmtReturn* stmtRet = m_module.getASTContext().getAllocator()
                .construct<StmtReturn>(m_module,
                                       node.getSourceInfo(),
                                       retExpr);

        getState().stackStatements.push(stmtRet);
    }

    void
    AstBuilder::visit(const STStmtDecl& node) {
        StackDecl& stackDecl = getState().stackDecls;
        const size_t stackSize = stackDecl.size();
        (void) stackSize;
        resolve(node.getDecl());
        YAL_ASSERT(!stackDecl.empty());
        DeclBase* decl = stackDecl.top();
        stackDecl.pop();
        YAL_ASSERT(stackDecl.size() == stackSize);

        StmtDecl* stmt = m_module.getASTContext().getAllocator()
                .construct<StmtDecl>(m_module,
                                     node.getSourceInfo(),
                                     decl);

        getState().stackStatements.push(stmt);
    }

    void
    AstBuilder::visit(const STStmtExpression& node) {
        StackExpr& stackExpr = getState().stackExpressions;
        const size_t stackSize = stackExpr.size();
        (void) stackSize;
        resolve(node.getExpr());
        YAL_ASSERT(!stackExpr.empty());
        StmtExpression* expr = stackExpr.top();
        stackExpr.pop();
        YAL_ASSERT(stackExpr.size() == stackSize);
        getState().stackStatements.push(expr);
    }

    void
    AstBuilder::visit(const STStmtAssign& node) {
        const STExpression& stExprLeft = node.getLeftExpr();
        const STExpression& stExprRight = node.getRightExpr();

        StackExpr& stack = getState().stackExpressions;
        const size_t stackSize = stack.size();
        (void) stackSize;
        resolve(stExprLeft);
        resolve(stExprRight);
        YAL_ASSERT(!stack.empty());
        YAL_ASSERT(stack.size() == stackSize + 2);

        StmtExpression* exprRight = stack.top();
        stack.pop();
        StmtExpression* exprLeft = stack.top();
        stack.pop();

        YAL_ASSERT(exprRight != nullptr && exprLeft != nullptr);
        //TODO: Improve this approach. This won't catch hidden references
        // or moving of references between variables
        // check the scope of the varRef is the same if the
        // right expression ends up in a var ref or struct var ref
        if (IsExprVarRef(*exprLeft)) {
            ExprVarRef* exprVarRefLeft = static_cast<ExprVarRef*>(exprLeft);
            const DeclVar& declLeft = exprVarRefLeft->getDeclVar();
            if (declLeft.getQualType().isReference()) {
                if (ExprUnaryOperator* unaryOp = dyn_cast<ExprUnaryOperator>(exprRight);
                        unaryOp != nullptr) {
                    const StmtExpression* expr = unaryOp->getExpression();

                    // & regular var
                    if (IsExprVarRef(*expr)) {
                        const ExprVarRef* exprVarRefRight= static_cast<const ExprVarRef*>(expr);
                        const DeclVar& declRight= exprVarRefRight->getDeclVar();
                        if (const DeclScope* scope = declLeft.getScopeWhereDeclared().value_or(nullptr);
                                scope != nullptr && scope->getDecl(declRight.getIdentifier(),false) == nullptr) {
                            // Error grabing reference out of this scope
                            auto error = std::make_unique<ErrorAssignRefWithInvalidScope>(node,
                                                                                          declLeft,
                                                                                          declRight);
                            m_errReporter.report(std::move(error));
                            getState().onError();
                        }
                    }

                    // & struct var
                    if (const ExprStructVarRef* structVarRef = dyn_cast<ExprStructVarRef>(expr);
                            structVarRef != nullptr) {
                        if (IsExprVarRef(*structVarRef->getExpression())) {
                            const ExprVarRef* exprVarRefRight= static_cast<const ExprVarRef*>(structVarRef->getExpression());
                            const DeclVar& declRight= exprVarRefRight->getDeclVar();
                            if (const DeclScope* scope = declLeft.getScopeWhereDeclared().value_or(nullptr);
                                    scope != nullptr && scope->getDecl(declRight.getIdentifier(),false) == nullptr) {
                                // Error grabing reference out of this scope
                                auto error = std::make_unique<ErrorAssignRefWithInvalidScope>(node,
                                                                                              declLeft,
                                                                                              declRight);
                                m_errReporter.report(std::move(error));
                                getState().onError();
                            }
                        }
                    }
                }
            }
        }

        StmtAssign* stmt = m_module.getASTContext().getAllocator()
                .construct<StmtAssign>(m_module,
                                       node.getSourceInfo(),
                                       exprLeft,
                                       exprRight);

        getState().stackStatements.push(stmt);
    }

    void
    AstBuilder::visit(const STExprVarRef& node) {

        const STIdentifier& varName = node.getVarName();
        DeclScope* parentScope = getState().stackScope.top();
        // check for symbol
        const DeclNamed* existingDecl = parentScope->getDecl(Identifier(varName.getString()), false);
        if (existingDecl == nullptr ) {
            onUndefinedSymbol(varName);
        }

        if (!existingDecl->isDeclVar()) {
            onSymbolNotDeclVar(varName, *existingDecl);
        }

        const DeclVar* declVar = static_cast<const DeclVar*>(existingDecl);

        ExprVarRef* expr = nullptr;
        if (varName.getString() == "self") {
            expr = m_module.getASTContext().getAllocator()
                    .construct<ExprVarRefSelf>(m_module,
                                               node.getSourceInfo(),
                                               declVar);
        } else {
            expr = m_module.getASTContext().getAllocator()
                    .construct<ExprVarRef>(m_module,
                                           node.getSourceInfo(),
                                           declVar);
        }

        getState().stackExpressions.push(expr);
    }

    void
    AstBuilder::visit(const STExprUnaryOperator& node) {

        StackExpr& stackExpr = getState().stackExpressions;
        const size_t stackSize = stackExpr.size();
        (void) stackSize;
        const STExpression& stExpr = node.getExpression();
        resolve(stExpr);
        YAL_ASSERT(!stackExpr.empty());
        YAL_ASSERT(stackExpr.size() == stackSize + 1);

        StmtExpression* expr = stackExpr.top();
        stackExpr.pop();

        ExprUnaryOperator* unaryOpExpr = m_module.getASTContext().getAllocator()
                .construct<ExprUnaryOperator>(m_module,
                                              node.getSourceInfo(),
                                              node.getOperatorType(),
                                              expr);

        stackExpr.push(unaryOpExpr);
    }

    void
    AstBuilder::visit(const STExprBinaryOperator& node) {
        const STExpression& stExprLeft = node.getExpressionLeft();
        const STExpression& stExprRight = node.getExpressionRight();

        StackExpr& stack = getState().stackExpressions;
        const size_t stackSize = stack.size();
        (void) stackSize;
        resolve(stExprLeft);
        resolve(stExprRight);
        YAL_ASSERT(!stack.empty());
        YAL_ASSERT(stack.size() == stackSize + 2);

        StmtExpression* exprRight = stack.top();
        stack.pop();
        StmtExpression* exprLeft = stack.top();
        stack.pop();

        ExprBinaryOperator* exprBinaryOp = m_module.getASTContext().getAllocator()
                .construct<ExprBinaryOperator>(m_module,
                                               node.getSourceInfo(),
                                               node.getOperatorType(),
                                               exprLeft,
                                               exprRight);

        stack.push(exprBinaryOp);
    }

    void
    AstBuilder::visit(const STExprBoolLiteral& node) {
        StackExpr& stack = getState().stackExpressions;

        ExprBoolLiteral* expr = m_module.getASTContext().getAllocator()
                .construct<ExprBoolLiteral>(m_module,
                                            node.getSourceInfo(),
                                            (node.getValue() == "true")
                                            ? true
                                            : false);

        stack.push(expr);
    }

    void
    AstBuilder::visit(const STExprIntegerLiteral& node) {

        StackExpr& stack = getState().stackExpressions;
        uint64_t value = 0;
        IntegerType intType = IntegerType::I32;
        bool negative = false;
        bool valid = false;

        if (StringRefToInteger(value, node.getValue(), negative)) {

            switch (node.getTokenType()) {
            case Token::IntegerLiteralI8: {
                const int64_t& negValue = reinterpret_cast<int64_t&>(value);
                valid = negValue >= static_cast<int64_t>(std::numeric_limits<int8_t>::lowest())
                        && negValue <= static_cast<int64_t>(std::numeric_limits<int8_t>::max());
                intType = IntegerType::I8;
                break;
            }
            case Token::IntegerLiteralI16: {
                const int64_t& negValue = reinterpret_cast<int64_t&>(value);
                valid = negValue >= static_cast<int64_t>(std::numeric_limits<int16_t>::lowest())
                        && negValue <= static_cast<int64_t>(std::numeric_limits<int16_t>::max());
                intType = IntegerType::I16;
                break;
            }
            case Token::IntegerLiteralI32:{
                const int64_t& negValue = reinterpret_cast<int64_t&>(value);
                valid = negValue >= static_cast<int64_t>(std::numeric_limits<int32_t>::lowest())
                        && negValue <= static_cast<int64_t>(std::numeric_limits<int32_t>::max());
                intType = IntegerType::I32;
                break;
            }
            case Token::IntegerLiteralI64:{
                const int64_t& negValue = reinterpret_cast<int64_t&>(value);
                valid = negValue >= static_cast<int64_t>(std::numeric_limits<int64_t>::lowest())
                        && negValue <= static_cast<int64_t>(std::numeric_limits<int64_t>::max());
                intType = IntegerType::I64;
                break;
            }
            case Token::IntegerLiteralU8:
                valid = !negative
                        && value <= static_cast<uint8_t>(std::numeric_limits<uint8_t>::max());
                intType = IntegerType::U8;
                break;
            case Token::IntegerLiteralU16:
                valid = !negative
                        && value <= static_cast<uint16_t>(std::numeric_limits<uint16_t>::max());
                intType = IntegerType::U16;
                break;
            case Token::IntegerLiteral:
            case Token::IntegerLiteralU32:
                intType = IntegerType::U32;
                valid = !negative
                        && value <= static_cast<uint32_t>(std::numeric_limits<uint32_t>::max());
                break;
            case Token::IntegerLiteralU64:
                intType = IntegerType::U64;
                valid = !negative
                        && value <= std::numeric_limits<uint64_t>::max();
                break;
            default:
                YAL_ASSERT_MESSAGE(false, "Shouldn't be reached");
                break;
            }

            if (valid) {
                ExprIntegerLiteral* expr = m_module.getASTContext().getAllocator()
                        .construct<ExprIntegerLiteral>(m_module,
                                                       node.getSourceInfo(),
                                                       intType,
                                                       value);
                stack.push(expr);
            }
        }
        if (!valid) {
            auto error = std::make_unique<ErrorInvalidIntLiteral>(node);
            m_errReporter.report(std::move(error));
            getState().onError();
        }
    }

    void
    AstBuilder::visit(const STExprFloatLiteral& node) {
        StackExpr& stack = getState().stackExpressions;

        double value = 0;
        bool valid = false;
        DecimalType decimalType = DecimalType::Decimal32;
        if (StringRefToDecimal(value, node.getValue())) {

            switch(node.getTokenType()) {

            case Token::DecimalLiteral:
            case Token::DecimalLiteral32: {
                valid = value >= static_cast<double>(std::numeric_limits<float>::lowest())
                        && value <= static_cast<double>(std::numeric_limits<float>::max());
            }
                break;
            case Token::DecimalLiteral64:
                decimalType = DecimalType::Decimal64;
                valid = true;
                break;

            default:
                YAL_ASSERT_MESSAGE(false, "Shouldn't be reached");
                break;
            }

            if (valid) {
                ExprFloatLiteral* expr = m_module.getASTContext().getAllocator()
                        .construct<ExprFloatLiteral>(m_module,
                                                     node.getSourceInfo(),
                                                     value,
                                                     decimalType);
                stack.push(expr);
            }
        }

        if (!valid) {
            auto error = std::make_unique<ErrorInvalidFloatLiteral>(node);
            m_errReporter.report(std::move(error));
            getState().onError();
        }
    }

    void
    AstBuilder::visit(const STExprStructVarRef& node) {
        StackExpr& stackExpr = getState().stackExpressions;
        const size_t stackSize = stackExpr.size();
        (void) stackSize;
        const STExpression& stExpr = node.getExpression();
        resolve(stExpr);
        YAL_ASSERT(!stackExpr.empty());
        YAL_ASSERT(stackExpr.size() == stackSize + 1);

        StmtExpression* expr = stackExpr.top();
        stackExpr.pop();

        ExprStructVarRef* exprVarRef = m_module.getASTContext().getAllocator()
                .construct<ExprStructVarRef>(m_module,
                                             node.getSourceInfo(),
                                             expr,
                                             node.getVarName().getString());

        stackExpr.push(exprVarRef);
    }

    void
    AstBuilder::visit(const STExprFnCall& node) {
        StackExpr& stackExpr = getState().stackExpressions;
        //TypeContext& typeCtx = m_module.getTypeContext();
        //Type* typeTarget = nullptr;
        const TypeFunction* typeFn= nullptr;
        StmtExpression* exprFn = nullptr;

        // if has target type
        // eval type  - static
        if (node.getFunctionType() == STExprFnCall::FnType::RegularOrStatic) {
            Type* type = resolveType(node.getName());
            if (type == nullptr) {
                onUndefinedSymbol(node.getName());
            }
            typeFn = dyn_cast<TypeFunction>(type);
            if (typeFn == nullptr) {
                auto err = std::make_unique<ErrorTypeIsNotFunction>(*type,
                                                                     node.getName().getSourceInfo());
                m_errReporter.report(std::move(err));
                getState().onError();
            }

            if (typeFn->isTypeFunction() && !typeFn->isTypeFunctionStatic()) {
                auto err = std::make_unique<ErrorTypeFunctionIsNotStatic>(*typeFn,
                                                                          node.getName().getSourceInfo());
                m_errReporter.report(std::move(err));
                getState().onError();
            }
        }
        // if has expr
        // eval expr - runtime type call
        else if (node.getFunctionType() == STExprFnCall::FnType::Instance) {
            // Get Expression, we can't evaluate much else at this point
            const size_t stackSize = stackExpr.size();
            (void) stackSize;
            const STExpression* stExpr = node.getExpr();
            YAL_ASSERT(stExpr != nullptr);
            resolve(*stExpr);
            YAL_ASSERT(!stackExpr.empty());
            YAL_ASSERT(stackExpr.size() == stackSize + 1);
            exprFn = stackExpr.top();
            stackExpr.pop();
        }else {
            YAL_ASSERT_MESSAGE(false, "Unknown function type");
        }

        // process fn args
        const STExprFnCall::ParamList& paramSt = node.getParams();
        ExprList params(ExprList::allocator_type(m_module.getASTContext().getAllocator()));
        if (!paramSt.empty()) {
            params.reserve(paramSt.size());
            for (auto& param : paramSt) {
                const size_t stackSize = stackExpr.size();
                (void) stackSize;
                resolve(*param);
                YAL_ASSERT(!stackExpr.empty());
                YAL_ASSERT(stackExpr.size() == stackSize + 1);
                StmtExpression* paramExpr = stackExpr.top();
                stackExpr.pop();
                params.push_back(paramExpr);
            }
        }

        // create type
        ExprFnCall* exprFnCall = nullptr;
        if (typeFn != nullptr) {
            if (typeFn->isFunction()) {
                exprFnCall = m_module.getASTContext().getAllocator()
                        .construct<ExprFnCall>(m_module,
                                               node.getSourceInfo(),
                                               typeFn,
                                               std::move(params));
            } else if(typeFn->isTypeFunctionStatic()) {
                exprFnCall = m_module.getASTContext().getAllocator()
                        .construct<ExprTypeFnCall>(m_module,
                                                   node.getSourceInfo(),
                                                   typeFn,
                                                   std::move(params));
            } else {
                YAL_ASSERT_MESSAGE(false, "Shouldn't happen");
            }
        } else if (node.getFunctionType() == STExprFnCall::FnType::Instance) {
            exprFnCall = m_module.getASTContext().getAllocator()
                    .construct<ExprTypeFnCall>(m_module,
                                               node.getSourceInfo(),
                                               exprFn,
                                               node.getName(),
                                               std::move(params));
        }
        // push
        stackExpr.push(exprFnCall);
    }

    void
    AstBuilder::visit(const STExprCast& node) {
        const STQualType& qtType = node.getTargetType();
        Type* targetType = resolveType(*qtType.m_type);
        // check if type has been defined
        if (targetType == nullptr) {
            onUndefinedType(*qtType.m_type);
        }

        StackExpr& stackExpr = getState().stackExpressions;
        const size_t stackSize = stackExpr.size();
        (void) stackSize;
        const STExpression& stExpr = node.getExpr();
        resolve(stExpr);
        YAL_ASSERT(!stackExpr.empty());
        YAL_ASSERT(stackExpr.size() == stackSize + 1);

        StmtExpression* expr = stackExpr.top();
        stackExpr.pop();

        const QualType qt = QualType::Create(MakeQualifier(qtType), targetType);

        ExprCast* exprCast = m_module.getASTContext().getAllocator()
                .construct<ExprCast>(m_module,
                                     node.getSourceInfo(),
                                     qt,
                                     expr);
        stackExpr.push(exprCast);
    }

    void
    AstBuilder::visit(const STExprStructInit& node) {
        StackExpr& stackExpr = getState().stackExpressions;
        const STType* stType = node.getStructType();

        const Type* type = resolveType(*stType);
        if (type == nullptr) {
            onUndefinedType(*stType);
        }

        const Type* resolvedType = &type->resolve();

        const TypeStruct* typeStruct = dyn_cast<TypeStruct>(resolvedType);
        if (typeStruct == nullptr) {
            auto error = std::make_unique<ErrorTypeIsNotStruct>(*type,
                                                                stType->getSourceInfo());
            m_errReporter.report(std::move(error));
            getState().onError();
        }

        // process struct init members
        ExprStructInit::MemberInitExprList members(ExprStructInit::MemberInitExprList::allocator_type(m_module.getASTContext().getAllocator()));
        const STExprStructInit::MemberInitList& stmembers = node.getMemeberInitExprs();
        if (!stmembers.empty()) {
            members.reserve(stmembers.size());
            const STDeclStruct& declStruct= typeStruct->getSTDecl();
            const STDeclStruct::Members& structMembers = declStruct.getMembers();
            for (auto& stmember : stmembers) {

                auto it = std::find_if(std::begin(structMembers),
                                       std::end(structMembers),
                                       [stmember](const STDeclStruct::Members::value_type& v) -> bool {
                    return v->getName().getString() == stmember->getName().getString();
                });

                if (it == std::end(structMembers)) {
                    auto err = std::make_unique<ErrorUndefinedStructMember>(*typeStruct,
                                                                            stmember->getName());
                    m_errReporter.report(std::move(err));
                    getState().onError();
                }

                const size_t stackSize = stackExpr.size();
                (void) stackSize;
                const STExpression& stExpr = stmember->getExpr();
                resolve(stExpr);
                YAL_ASSERT(!stackExpr.empty());
                YAL_ASSERT(stackExpr.size() == stackSize + 1);
                StmtExpression* expr = stackExpr.top();
                stackExpr.pop();

                StructMemberInit* initExpr = m_module.getASTContext().getAllocator()
                        .construct<StructMemberInit>(m_module,
                                                     node.getSourceInfo(),
                                                     stmember->getName().getString(),
                                                     expr);
                members.push_back(initExpr);
            }
        }

        // push result
        ExprStructInit* structInit = m_module.getASTContext().getAllocator()
                .construct<ExprStructInit>(m_module,
                                           node.getSourceInfo(),
                                           *type,
                                           *typeStruct,
                                           std::move(members));
        stackExpr.push(structInit);
    }

    void
    AstBuilder::visit(const STStmtListScoped& node) {
        const STStatementList& scopedStatements = node.getStatements();
        StatementList statements(StatementList::allocator_type(m_module.getASTContext().getAllocator()));
        DeclScope* parentScope = getState().stackScope.top();
        StmtListScoped* stmtList = m_module.getASTContext().getAllocator()
                .construct<StmtListScoped>(m_module,
                                           node.getSourceInfo(),
                                           parentScope);

        if (!scopedStatements.empty()) {
            // set new scope for statements
            ScopedStackElem<StackScope> sscope(getState().stackScope,
                                               &stmtList->getListScope());
            StackStatement& stack = getState().stackStatements;
            const size_t currStmtStackSize = stack.size();

            // process statements
            for(auto& stmt : scopedStatements) {
                resolve(*stmt);
            }

            // collect statements
            statements.resize(scopedStatements.size(), nullptr);
            for(size_t idx = statements.size() -1 ;
                !stack.empty() && stack.size() > currStmtStackSize; --idx) {
                Statement* stmt = stack.top();
                statements[idx] = stmt;
                stack.pop();
            }

            YAL_ASSERT(statements.size() ==scopedStatements.size());
            stmtList->setStatements(std::move(statements));
        }
        getState().stackStatements.push(stmtList);
    }

    bool
    AstBuilder::execute() {
        State state;
        m_state = &state;

        const STDeclModule* stDeclModule =
                m_module.getSTContext().getDeclModule();
        if (YAL_STACKJUMP_MARK(m_state->jump)) {
            return !m_errReporter.hasErrors();
        }
        visit(*stDeclModule);
        return !m_errReporter.hasErrors();
    }

    AstBuilder::State&
    AstBuilder::getState() {
        YAL_ASSERT(m_state != nullptr);
        return *m_state;
    }


    void
    AstBuilder::onUndefinedType(const STType &type) {
        auto error = std::make_unique<ErrorUndefinedTypeRef>(type.getIdentifier().getString(),
                                                             type.getSourceInfo());
        m_errReporter.report(std::move(error));
        getState().onError();
    }

    void
    AstBuilder::onUndefinedType(const STDeclNamed &decl) {
        auto error = std::make_unique<ErrorUndefinedTypeRef>(decl.getName().getString(),
                                                             decl.getSourceInfo());
        m_errReporter.report(std::move(error));
        getState().onError();
    }

    void
    AstBuilder::onDuplicateSymbol(const STDeclNamed &decl,
                                  const DeclNamed &existing) {
        auto error = std::make_unique<ErrorDuplicateSymbol>(decl.getName().getString(),
                                                            decl.getSourceInfo(),
                                                            existing.getIdentifier().getName(),
                                                            existing.getSourceInfo());
        m_errReporter.report(std::move(error));
        getState().onError();
    }

    void
    AstBuilder::onDuplicateSymbol(const STIdentifier& id,
                                  const DeclNamed& existing) {
        auto error = std::make_unique<ErrorDuplicateSymbol>(id.getString(),
                                                            id.getSourceInfo(),
                                                            existing.getIdentifier().getName(),
                                                            existing.getSourceInfo());
        m_errReporter.report(std::move(error));
        getState().onError();
    }

    void
    AstBuilder::onUndefinedSymbol(const STIdentifier& id) {
        auto error = std::make_unique<ErrorUndefinedSymbol>(id.getString(),
                                                            id.getSourceInfo());
        m_errReporter.report(std::move(error));
        getState().onError();
    }

    void
    AstBuilder::onSymbolNotDeclVar(const STIdentifier& id,
                                   const DeclNamed& decl) {
        auto error = std::make_unique<ErrorSymNotDeclVar>(id.getString(),
                                                          id.getSourceInfo(),
                                                          decl);
        m_errReporter.report(std::move(error));
        getState().onError();
    }

    Type*
    AstBuilder::resolveType(const STType& stType) {
        TypeContext& typeCtx = m_module.getTypeContext();
        return typeCtx.resolveType(stType);
    }

    Type*
    AstBuilder::resolveType(const STIdentifier& identifier) {
        TypeContext& typeCtx = m_module.getTypeContext();
        return typeCtx.resolveType(identifier);
    }
}
