/*
 *  Copyright 2017 by Leander Beernaert (leanderbb@gmail.com)
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
#include "yal/compiler/stages/stagemovecheck.h"
#include "yal/compiler/compiler.h"
#include "yal/ast/astnodes.h"
#include "yal/util/prettyprint.h"
#include "yal/util/log.h"
#include "yal/util/stackjump.h"
#include "yal/ast/declbase.h"
#include "yal/ast/astsearch.h"
#include <memory>
#include <unordered_map>

namespace yal {

    static bool CanMove(const QualType& from,
                        const SourceInfo siFrom,
                        const QualType& to,
                        const SourceInfo siTo,
                        Compiler& compiler){

        Log& log = compiler.getLog();
        (void) to;
        (void) siTo;

        if (from.getQualifier().requiresReplace()
                && !from.getType()->isTriviallyCopiable()) {
            PrettyPrint::SourceErrorPrint(log,
                                          siFrom,
                                          compiler.getSourceManager());
            log.error("Can't move value. Value is likely a struct member and "
                      " needs to be reset before it can be moved. "
                      " Use <replace>().\n");
            return false;
        }

        return true;
    }

    class MoveAstVisitor final : public RecursiveAstVisitor {
    public:
        MoveAstVisitor(Compiler& compiler,
                       Module& module,
                       StackJump& jump):
            m_compiler(compiler),
            m_module(module),
            m_stackJump(jump),
            m_error(false) {
            m_activeScope = m_module.getDeclNode()->getModuleDeclScope();
            m_moveMap = std::make_unique<MoveMap>();
        }

#define YAL_AST_SKIP_NODE_CONTAINERS
#define YAL_AST_NODE_TYPE(TYPE) virtual void visit(TYPE&) override final;
#include "yal/ast/astnodes.def"
#undef YAL_AST_NODE_TYPE
#undef YAL_AST_SKIP_NODE_CONTAINERS

        bool didError() const {
            return m_error;
        }

    private:
        struct MoveState {
            bool moved = true;
            const Statement* stmtWhenMoved = nullptr;
        };

        void pushScope(const DeclScope *scope);

        void popScope();

        void onError();

        void annotateExpr(StmtExpression* expr);

        void annotateExprList(ExprList* args);

        MoveState& findState(const DeclBase* decl);

    private:
        using MoveMap = std::unordered_map<const DeclBase*, MoveState>;

        Compiler& m_compiler;
        Module& m_module;
        StackJump& m_stackJump;
        const DeclScope* m_activeScope;
        std::unique_ptr<MoveMap> m_moveMap;
        bool m_error;
    };


    void
    MoveAstVisitor::pushScope(const DeclScope* scope) {
        m_activeScope = scope;
        for (auto& declit : m_activeScope->getDeclsConst()) {
            const DeclBase* decl =declit.second;
            switch (decl->getAstType()) {

            case AstType::DeclVar: {
                MoveState state;
                m_moveMap->insert(std::make_pair(decl, state));
                break;
            }
            case AstType::DeclParamVar:
            case AstType::DeclParamVarSelf:{
                MoveState state;
                state.moved = false;
                m_moveMap->insert(std::make_pair(decl, state));
                break;
            }
            default:
                continue;
            }
        }
    }

    void
    MoveAstVisitor::popScope() {

        for (auto& declit : m_activeScope->getDeclsConst()) {
            const DeclBase* decl =declit.second;
            switch (decl->getAstType()) {
            case AstType::DeclVar:
            case AstType::DeclParamVar:
            case AstType::DeclParamVarSelf:
                m_moveMap->erase(decl);
            default:
                continue;
            }
        }

        m_activeScope = m_activeScope->getParentScope();
    }


    void
    MoveAstVisitor::annotateExpr(StmtExpression *expr) {
        // check for varref and mark them as moved
        {
            AstExprLeafSearch<ExprVarRef> varRefSearch;
            const ExprVarRef* varRef = varRefSearch.search(expr);
            if (varRef != nullptr) {
                MoveState& moveState = findState(varRef->getDeclVar());
                moveState.moved = true;
                moveState.stmtWhenMoved = varRef;
            }
        }
    }

    void
    MoveAstVisitor::annotateExprList(ExprList* args) {
        for (auto& expr : args->getChildRange()) {
            annotateExpr(expr);
        }
    }

    void
    MoveAstVisitor::onError() {
        m_error = true;
        m_moveMap.reset();
        m_stackJump.trigger();
    }


    MoveAstVisitor::MoveState&
    MoveAstVisitor::findState(const DeclBase* decl) {
        auto it = m_moveMap->find(decl);
        // shouldn't happen, this is check in earlier stages
        YAL_ASSERT(it != m_moveMap->end());
        MoveState& state = it->second;
        return state;
    }

    void
    MoveAstVisitor::visit(DeclFunction& node) {
        pushScope(node.getFunctionDeclScope());
        if (node.getFunctionBody() != nullptr) {
            node.getFunctionBody()->acceptVisitor(*this);
        }
        popScope();
    }

    void
    MoveAstVisitor::visit(DeclTypeFunction& node) {
        pushScope(node.getFunctionDeclScope());
        if (node.getFunctionBody() != nullptr) {
            node.getFunctionBody()->acceptVisitor(*this);
        }
        popScope();
    }

    void
    MoveAstVisitor::visit(DeclStruct&) {
    }

    void
    MoveAstVisitor::visit(DeclVar& node) {
        const QualType destType = node.getQualType();
        StmtExpression* initExpr = node.getExpression();
        initExpr->acceptVisitor(*this);
        const QualType exprType = node.getExpression()->getQualType();
        if (!CanMove(exprType,
                     initExpr->getSourceInfo(),
                     destType,
                     node.getSourceInfo(),
                     m_compiler)) {
            onError();
        }

        annotateExpr(initExpr);

        MoveState& state = findState(&node);
        state.moved = false;
        state.stmtWhenMoved = nullptr;
    }

    void
    MoveAstVisitor::visit(DeclParamVar&) {
        YAL_ASSERT(false);
    }

    void
    MoveAstVisitor::visit(DeclStrongAlias&) {

    }

    void
    MoveAstVisitor::visit(DeclWeakAlias&) {

    }

    void
    MoveAstVisitor::visit(RefType& ) {
        YAL_ASSERT(false);
    }

    void
    MoveAstVisitor::visit(StmtReturn& node) {
        if (node.getExpression()) {
            node.getExpression()->acceptVisitor(*this);
        }

        //TODO: Move this to scoped lifetime check + simpler way??

        // check for return of local variable out of scope?
        // can either be :
        // 1: reference to a var: &x
        // 2: var that is a reference

        {// check 1:
            const ExprVarRef* exprVarRef
                    = dyn_cast<ExprVarRef>(node.getExpression());

            if (exprVarRef != nullptr) {
                const QualType qt = exprVarRef->getQualType();
                if (qt.getQualifier().isReference()) {
                    const DeclVar* decl = exprVarRef->getDeclVar();

                    if (decl->getExpression() == nullptr
                            && (decl->getAstType() == AstType::DeclParamVar
                                || decl->getAstType() == AstType::DeclParamVarSelf)) {
                        if (!decl->getQualType().isReference()) {
                            // reference to function parameter
                            Log& log = m_compiler.getLog();
                            PrettyPrint::SourceErrorPrint(log,
                                                          exprVarRef->getSourceInfo(),
                                                          m_compiler.getSourceManager());
                            log.error("Can not return reference to variable bound "
                                      "to local scope\n");
                            log.error("Variable referenced here:\n");
                            PrettyPrint::SourceErrorPrint(log,
                                                          decl->getSourceInfo(),
                                                          m_compiler.getSourceManager());
                            onError();
                        }
                    } else {
                        // is an actual Decl var
                        const ExprUnaryOperator* unaryOp
                                = dyn_cast<ExprUnaryOperator>(decl->getExpression());
                        // check if the decl is a reference
                        if (unaryOp != nullptr
                                && unaryOp->getOperatorType()  == UnaryOperatorType::Reference) {
                            const ExprVarRef* localVarRef
                                    = dyn_cast<ExprVarRef>(unaryOp->getExpression());
                            if (localVarRef != nullptr) {
                                const DeclVar* decl = localVarRef->getDeclVar();
                                if (m_activeScope == decl->getScopeWhereDeclared()) {
                                    Log& log = m_compiler.getLog();
                                    PrettyPrint::SourceErrorPrint(log,
                                                                  exprVarRef->getSourceInfo(),
                                                                  m_compiler.getSourceManager());
                                    log.error("Can not return reference to variable bound "
                                              "to local scope\n");
                                    log.error("Variable referenced here:\n");
                                    PrettyPrint::SourceErrorPrint(log,
                                                                  localVarRef->getSourceInfo(),
                                                                  m_compiler.getSourceManager());
                                    onError();
                                }
                            }
                        }
                    }
                }
            }
        }

        {// check 2:

            const ExprUnaryOperator* unaryOp
                    = dyn_cast<ExprUnaryOperator>(node.getExpression());

            if (unaryOp != nullptr
                    && unaryOp->getOperatorType()  == UnaryOperatorType::Reference) {
                const ExprVarRef* exprVarRef
                        = dyn_cast<ExprVarRef>(unaryOp->getExpression());
                if (exprVarRef != nullptr) {
                    const DeclVar* decl = exprVarRef->getDeclVar();
                    if (m_activeScope->getDecl(decl->getIdentifier(), true)) {
                        Log& log = m_compiler.getLog();
                        PrettyPrint::SourceErrorPrint(log,
                                                      exprVarRef->getSourceInfo(),
                                                      m_compiler.getSourceManager());
                        log.error("Can not return reference to variable bound "
                                  "to local scope\n");
                        onError();
                    }
                }
            }
        }
    }

    void
    MoveAstVisitor::visit(StmtDecl& node) {
        node.getDecl()->acceptVisitor(*this);
    }

    void
    MoveAstVisitor::visit(StmtAssign& node) {
        StmtExpression* valueExpr = node.getValueExpr();
        StmtExpression* destExpr = node.getDestExpr();
        const ExprVarRef* dstExprVarRef = dyn_cast<ExprVarRef>(destExpr);

        // only visit left if different than varref
        // TODO: Need better way to handle this!
        if ( dstExprVarRef == nullptr) {
            destExpr->acceptVisitor(*this);
        }

        valueExpr->acceptVisitor(*this);

        const QualType qualFrom = valueExpr->getQualType();
        const QualType qualTo = destExpr->getQualType();

        if (!CanMove(qualFrom,
                     valueExpr->getSourceInfo(),
                     qualTo,
                     destExpr->getSourceInfo(),
                     m_compiler)) {
            onError();
        }

        annotateExpr(valueExpr);

        // TODO: Need better way to handle this!
        if (dstExprVarRef != nullptr) {
            MoveState& mvstate = findState(dstExprVarRef->getDeclVar());
            mvstate.moved = false;
            mvstate.stmtWhenMoved = nullptr;
        }
    }

    void
    MoveAstVisitor::visit(ExprVarRef& node) {
        if (!node.getQualType().getQualifier().isReference()) {
            const MoveState& state = findState(node.getDeclVar());
            if (state.moved) {
                Log& log = m_compiler.getLog();
                PrettyPrint::SourceErrorPrint(log,
                                              node.getSourceInfo(),
                                              m_compiler.getSourceManager());
                log.error("Can not use variable '%' as it has been moved and "
                          "is no longer valid.\n",
                          node.getDeclVar()->getIdentifier());
                if (state.stmtWhenMoved != nullptr) {
                    log.error("Variable '%' was moved here:\n",
                              node.getDeclVar()->getIdentifier());
                    PrettyPrint::SourceErrorPrint(log,
                                                  state.stmtWhenMoved->getSourceInfo(),
                                                  m_compiler.getSourceManager());

                }
                onError();
            }
        }
    }

    void
    MoveAstVisitor::visit(ExprUnaryOperator& node) {
        node.getExpression()->acceptVisitor(*this);
    }

    void
    MoveAstVisitor::visit(ExprBinaryOperator& node) {
        node.getExpressionRight()->acceptVisitor(*this);
        node.getExpressionLeft()->acceptVisitor(*this);
    }

    void
    MoveAstVisitor::visit(ExprBoolLiteral&) {

    }

    void
    MoveAstVisitor::visit(ExprIntegerLiteral& ) {

    }

    void
    MoveAstVisitor::visit(ExprFloatLiteral&) {

    }

    void
    MoveAstVisitor::visit(ExprStructVarRef& node) {
        node.getExpression()->acceptVisitor(*this);
    }

    void
    MoveAstVisitor::visit(ExprFnCall& node) {
        if (node.getFunctionArgs() != nullptr) {
            node.getFunctionArgs()->acceptVisitor(*this);
            annotateExprList(node.getFunctionArgs());
            /*
             // This is unlikely to happen, left here for reference
            const TypeDecl* fnType
                    = dyn_cast<TypeDecl>(node.getFunctionType()->getType());
            YAL_ASSERT(fnType != nullptr);

            const DeclFunction* fnDecl
                    = dyn_cast<DeclFunction>(fnType->getDecl());
            YAL_ASSERT(fnDecl != nullptr);

            const DeclParamVarContainer* fnParams = fnDecl->getParams();
            const ExprList* fnArgs = node.getFunctionArgs();

            for(size_t i = 0; i < fnParams->getCount(); ++i) {
                const DeclParamVar* param = (*fnParams)[i];
                const StmtExpression* expr = (*fnArgs)[i];

                const QualType exprQt = expr->getQualType();
                const QualType paramQt = param->getQualType();

                if (paramQt.isReference()) {
                    if (exprQt.isRValue()) {
                        Log& log = m_compiler.getLog();
                        PrettyPrint::SourceErrorPrint(log,
                                                      node.getSourceInfo(),
                                                      m_compiler.getSourceManager());
                        log.error("Can not use rvalue as a parameter as reference for arg '%' in function '%'.",
                                  param->getName(), fnDecl->getIdentifier());
                        onError();
                    }
                }
            } */
        }
    }

    void
    MoveAstVisitor::visit(ExprTypeFnCall& node) {
        if (node.getFunctionArgs() != nullptr) {

            // TODO:
            // For every function call, check if type is
            // movable, if so, make sure the expression is an
            // rvalue instead of an lvalue?

            node.getFunctionArgs()->acceptVisitor(*this);
            annotateExprList(node.getFunctionArgs());
        }
    }

    void
    MoveAstVisitor::visit(ExprTypeFnCallStatic& node) {
        if (node.getFunctionArgs() != nullptr) {
            node.getFunctionArgs()->acceptVisitor(*this);
            annotateExprList(node.getFunctionArgs());
        }
    }

    void
    MoveAstVisitor::visit(DeclParamVarSelf&) {

    }

    void
    MoveAstVisitor::visit(ExprVarRefSelf& node) {
        YAL_ASSERT(node.getQualType().getType() != nullptr);
    }

    void
    MoveAstVisitor::visit(ExprRangeCast& node) {
        node.getExpression()->acceptVisitor(*this);
    }

    void
    MoveAstVisitor::visit(ExprStructInit& node) {
        if (node.getMemberInitList() != nullptr) {
            node.getMemberInitList()->acceptVisitor(*this);

        }
    }

    void
    MoveAstVisitor::visit(StructMemberInit& node) {
        node.getInitExpr()->acceptVisitor(*this);
    }

    StageMoveCheck::StageMoveCheck(Compiler &compiler,
                                   Module &module):
        m_compiler(compiler),
        m_module(module) {

    }

    bool
    StageMoveCheck::execute(DeclBase *decl) {
        StackJump jmp;
        MoveAstVisitor visitor (m_compiler, m_module, jmp);
        jmp.mark();
        decl->acceptVisitor(visitor);
        return !visitor.didError();
    }

}
