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

#include "yal/compiler/stages/stageexprtype.h"
#include "yal/compiler/compiler.h"
#include "yal/ast/astnodes.h"
#include "yal/util/prettyprint.h"
#include "yal/util/log.h"
#include "yal/util/stackjump.h"

namespace yal {


    static bool CanConvert(const QualType& from,
                           const SourceInfo siFrom,
                           const QualType& to,
                           const SourceInfo siTo,
                           Compiler& compiler) {
        (void) siTo;
        YAL_ASSERT(from.isValid() && to.isValid());


        const Type* typeFrom= from.getType();
        const Type* typeTo = to.getType();
        Log& log = compiler.getLog();
        // check types
        if(!typeFrom->isCastableTo(typeTo)) {
            PrettyPrint::SourceErrorPrint(log,
                                          siFrom,
                                          compiler.getSourceManager());
            log.error("Cannot convert from '%' to '%', types are not compatible.\n",
                      from,to);
            return false;
        }

        // check qualifiers
        const Qualifier qualFrom = from.getQualifier();
        const Qualifier qualTo = to.getQualifier();

        if (qualFrom.isImmutable() && !qualTo.isImmutable()) {
            PrettyPrint::SourceErrorPrint(log,
                                          siFrom,
                                          compiler.getSourceManager());
            log.error("Cannot convert from '%' to '%', can't convert immutable to mutable.\n",
                      from,to);
            return false;
        }

        if (qualFrom.isReference() != qualTo.isReference()) {
            PrettyPrint::SourceErrorPrint(log,
                                          siFrom,
                                          compiler.getSourceManager());

            if (!qualFrom.isReference() && qualTo.isReference()) {
                log.error("Cannot convert from '%' to '%', use '&' operator.\n",
                          from,to);
            } else {
                log.error("Cannot convert from '%' to '%'\n",
                          from,to);
            }
            return false;
        }

        // TODO: Move to move evaluater
        if (qualFrom.requiresReplace() && !typeFrom->isTriviallyCopiable()) {
            PrettyPrint::SourceErrorPrint(log,
                                          siFrom,
                                          compiler.getSourceManager());
            log.error("Can't move value. Value needs to be replaced before "
                      "it can be moved. Use replace().\n",
                      from,to);
            return false;
        }

        return true;
    }


    class ExprTypeAstVisitor : public RecursiveAstVisitor {

    public:

        ExprTypeAstVisitor(Compiler& compiler,
                           Module& module,
                           StackJump& jump):
            m_compiler(compiler),
            m_module(module),
            m_activeScope(nullptr),
            m_stackJump(jump),
            m_error(false){

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

        void pushScope(DeclScope* scope);

        void popScope();

        void onError();

    public:
        Compiler& m_compiler;
        Module& m_module;
        DeclScope* m_activeScope;
        StackJump& m_stackJump;
        bool m_error;
    };

    void
    ExprTypeAstVisitor::pushScope(DeclScope* scope) {
        m_activeScope = scope;
    }

    void
    ExprTypeAstVisitor::popScope() {
        YAL_ASSERT(m_activeScope != nullptr);
        YAL_ASSERT(m_activeScope->getParentScope() != nullptr);
        m_activeScope = m_activeScope->getParentScope();
    }

    void
    ExprTypeAstVisitor::onError() {
        m_error = true;
        m_stackJump.trigger();
    }

    void
    ExprTypeAstVisitor::visit(DeclFunction& node) {
        pushScope(node.getDeclScope());
        if (node.getFunctionBody() != nullptr) {
            node.getFunctionBody()->acceptVisitor(*this);
        }
        popScope();
    }

    void
    ExprTypeAstVisitor::visit(DeclTypeFunction& node) {
        pushScope(node.getDeclScope());
        if (node.getFunctionBody() != nullptr) {
            node.getFunctionBody()->acceptVisitor(*this);
        }
        popScope();
    }

    void
    ExprTypeAstVisitor::visit(DeclStruct& node) {
        pushScope(node.getDeclScope());
        node.getMembers()->acceptVisitor(*this);
        popScope();
    }

    void
    ExprTypeAstVisitor::visit(DeclVar& node) {
        YAL_ASSERT(node.getVarType()->getType() != nullptr);

        const QualType destType = node.getVarType()->getQualType();
        if (node.getExpression() != nullptr) {
            node.getExpression()->acceptVisitor(*this);

            const QualType exprType = node.getExpression()->getQualType();
            if (!CanConvert(exprType,
                            node.getExpression()->getSourceInfo(),
                            destType,
                            node.getSourceInfo(),
                            m_compiler)) {
                onError();
            }
        }

        // check for recursive struct types that aren't references
        if (destType.getType()->isStruct() && m_activeScope->isStructScope()) {
            DeclStruct* dclStruct = dyn_cast<DeclStruct>(m_activeScope->getScopeDecl());
            YAL_ASSERT(dclStruct != nullptr);

            if (dclStruct->getDeclType()->getTypeId() == destType.getType()->getTypeId()
                    && !destType.getQualifier().isReference()) {
                Log& log = m_compiler.getLog();
                PrettyPrint::SourceErrorPrint(log,
                                              node.getSourceInfo(),
                                              m_compiler.getSourceManager());
                log.error("Can not declare struct variable inside struct "
                          "'%' with the same type without being a reference.\n",
                          dclStruct->getIdentifier());
                onError();
            }
        }
    }

    void
    ExprTypeAstVisitor::visit(DeclParamVar& node) {
        YAL_ASSERT(node.getExpression() == nullptr);
    }

    void
    ExprTypeAstVisitor::visit(DeclStrongAlias&) {

    }

    void
    ExprTypeAstVisitor::visit(DeclWeakAlias&) {

    }

    void
    ExprTypeAstVisitor::visit(RefType& ) {
        YAL_ASSERT(false);
    }

    void
    ExprTypeAstVisitor::visit(StmtReturn& node) {
        YAL_ASSERT(m_activeScope->isFunctionTypeScope());
        YAL_ASSERT(m_activeScope->getScopeDecl() != nullptr);

        StmtExpression* expr = node.getExpression();
        // check type if we have an expression
        if (expr != nullptr) {
            expr->acceptVisitor(*this);

            DeclFunctionBase* functionDecl
                    = static_cast<DeclFunctionBase*>(m_activeScope->getScopeDecl());

            if (!CanConvert(expr->getQualType(),
                            expr->getSourceInfo(),
                            functionDecl->getReturnType()->getQualType(),
                            functionDecl->getReturnType()->getSourceInfo(),
                            m_compiler)) {
                onError();
            }
        }
    }

    void
    ExprTypeAstVisitor::visit(StmtDecl& node) {
        node.getDecl()->acceptVisitor(*this);
    }

    void
    ExprTypeAstVisitor::visit(StmtAssign& node) {
        StmtExpression* dstExpr = node.getDestExpr();
        StmtExpression* valExpr = node.getValueExpr();
        dstExpr->acceptVisitor(*this);
        valExpr->acceptVisitor(*this);

        const QualType dstqt = dstExpr->getQualType();
        const QualType valqt = valExpr->getQualType();

        if (!CanConvert(valqt,
                        valExpr->getSourceInfo(),
                        dstqt,
                        dstExpr->getSourceInfo(),
                        m_compiler)) {
            onError();
        }
    }

    void
    ExprTypeAstVisitor::visit(ExprVarRef& node) {
        (void) node;
        YAL_ASSERT(node.getQualType().isValid());
    }

    void
    ExprTypeAstVisitor::visit(ExprUnaryOperator& node) {
        node.getExpression()->acceptVisitor(*this);
        Log& log = m_compiler.getLog();
        switch (node.getOperatorType()) {
        case UnaryOperatorType::BitNot:{
            const Type* exprType = node.getExpression()->getQualType().getType();
            const TypeBuiltin* typeBuiltin = dyn_cast<TypeBuiltin>(exprType);

            if (typeBuiltin == nullptr || !typeBuiltin->isUnsigned() ) {
                PrettyPrint::SourceErrorPrint(log,
                                              node.getSourceInfo(),
                                              m_compiler.getSourceManager());
                log.error("Operator only works on signed types\n");
                onError();
            }
            node.setQualType(QualType::Create(Qualifier(), exprType));
            break;
        }
        case UnaryOperatorType::Negate: {
            const Type* exprType = node.getExpression()->getQualType().getType();
            const TypeBuiltin* typeBuiltin = dyn_cast<TypeBuiltin>(exprType);

            if (typeBuiltin == nullptr || !typeBuiltin->isSigned() ) {
                PrettyPrint::SourceErrorPrint(log,
                                              node.getSourceInfo(),
                                              m_compiler.getSourceManager());
                log.error("Operator only works on signed types\n");
                onError();
            }
            node.setQualType(QualType::Create(Qualifier(), exprType));
            break;
        }
        case UnaryOperatorType::Not: {
            const Type* exprType = node.getExpression()->getQualType().getType();
            const TypeBuiltin* typeBuiltin = dyn_cast<TypeBuiltin>(exprType);

            if (typeBuiltin == nullptr || !typeBuiltin->isBool()) {
                PrettyPrint::SourceErrorPrint(log,
                                              node.getSourceInfo(),
                                              m_compiler.getSourceManager());
                log.error("Operator only works on boolean expressions\n");
                onError();
            }
            node.setQualType(QualType::Create(Qualifier(), exprType));
            break;
        }
        case UnaryOperatorType::Reference: {
            const QualType exprQt = node.getExpression()->getQualType();
            Qualifier qual = exprQt.getQualifier();

            if (qual.isReference()) {
                PrettyPrint::SourceErrorPrint(log,
                                              node.getSourceInfo(),
                                              m_compiler.getSourceManager());
                log.error("Expression result is already a reference, can't "
                          "create a reference of a reference\n");
                onError();
            }

            qual.setReference();
            node.setQualType(QualType::Create(qual, exprQt.getType()));
            break;
        }
        default:
            YAL_ASSERT(false);
            PrettyPrint::SourceErrorPrint(log,
                                          node.getSourceInfo(),
                                          m_compiler.getSourceManager());
            log.error("Unknown operator type\n");
            onError();
        }
    }

    void
    ExprTypeAstVisitor::visit(ExprBinaryOperator& node) {
        node.getExpressionLeft()->acceptVisitor(*this);
        node.getExpressionRight()->acceptVisitor(*this);
        Log& log = m_compiler.getLog();
        switch(node.getOperatorType()) {

        case BinaryOperatorType::Div:
        case BinaryOperatorType::Mult:
        case BinaryOperatorType::Plus:
        case BinaryOperatorType::Minus:{
            const Type* typeTo= node.getExpressionLeft()->getQualType().getType();
            const Type* typeFrom = node.getExpressionRight()->getQualType().getType();

            // check types
            if(!typeFrom->isCastableTo(typeTo)) {
                PrettyPrint::SourceErrorPrint(log,
                                              node.getSourceInfo(),
                                              m_compiler.getSourceManager());
                log.error("Cannot convert from '%' to '%', types are not compatible.\n",
                          *typeFrom, *typeTo);
                onError();
            }

            if (dyn_cast<TypeBuiltin>(typeTo) == nullptr) {
                // TODO operator traits ??
                log.error("Type '%' does not implement this operator.\n",
                          *typeTo);
                onError();
            }

            node.setQualType(QualType::Create(Qualifier(), typeTo));
            break;
        }

        case BinaryOperatorType::BitAnd:
        case BinaryOperatorType::BitOr:
        case BinaryOperatorType::BitXor:{
            // check if left and right are unsigned integers
            // and if they match

            const TypeBuiltin* typeLeft= dyn_cast<TypeBuiltin>(node.getExpressionLeft()->getQualType().getType());
            const TypeBuiltin* typeRight = dyn_cast<TypeBuiltin>(node.getExpressionRight()->getQualType().getType());
            Log& log = m_compiler.getLog();


            if (typeLeft == nullptr
                    || typeRight == nullptr
                    || !typeLeft->isUnsigned()
                    || !typeRight->isUnsigned()) {
                PrettyPrint::SourceErrorPrint(log,
                                              node.getSourceInfo(),
                                              m_compiler.getSourceManager());
                log.error("Bit operators only work with unsigned integers \
                          (u8, u16, u32, u64).\n");
                          onError();
            }

            // check types
            if(!typeLeft->isCastableTo(typeRight)) {
                PrettyPrint::SourceErrorPrint(log,
                                              node.getSourceInfo(),
                                              m_compiler.getSourceManager());
                log.error("Cannot convert from '%' to '%', types are not compatible.\n",
                          *typeRight, *typeLeft);
                onError();
            }

            node.setQualType(QualType::Create(Qualifier(), typeLeft));
            break;
        }

        case BinaryOperatorType::Eq:
        case BinaryOperatorType::Ne:
        case BinaryOperatorType::Lt:
        case BinaryOperatorType::Le:
        case BinaryOperatorType::Ge:
        case BinaryOperatorType::Gt: {
            // check if left and right expr match
            // if types are comparable
            // set result of expression to bool

            const Type* typeTo= node.getExpressionLeft()->getQualType().getType();
            const Type* typeFrom = node.getExpressionRight()->getQualType().getType();
            Log& log = m_compiler.getLog();
            // check types
            if(!typeFrom->isCastableTo(typeTo)) {
                PrettyPrint::SourceErrorPrint(log,
                                              node.getSourceInfo(),
                                              m_compiler.getSourceManager());
                log.error("Cannot convert from '%' to '%', types are not compatible.\n",
                          *typeFrom, *typeTo);
                onError();
            }

            if (dyn_cast<TypeBuiltin>(typeTo) == nullptr) {
                // TODO operator <=>
                log.error("Type '%' does  not implement <=> operator.\n",
                          *typeTo);
                onError();
            }

            node.setQualType(QualType::Create(Qualifier(),
                                              m_module.getTypeContext().getTypeBuiltinBool()));
            break;
        }

        case BinaryOperatorType::And:
        case BinaryOperatorType::Or: {
            // check if left and right expr match
            // if types are comparable
            // set result of expression to bool

            const TypeBuiltin* typeLeft= dyn_cast<TypeBuiltin>(node.getExpressionLeft()->getQualType().getType());
            const TypeBuiltin* typeRight = dyn_cast<TypeBuiltin>(node.getExpressionRight()->getQualType().getType());
            Log& log = m_compiler.getLog();


            if (typeLeft == nullptr
                    || typeRight == nullptr
                    || !typeLeft->isBool()
                    || !typeRight->isBool()) {
                PrettyPrint::SourceErrorPrint(log,
                                              node.getSourceInfo(),
                                              m_compiler.getSourceManager());
                log.error("And/Or only supports boolean expressions.\n");
                onError();
            }

            node.setQualType(QualType::Create(Qualifier(),
                                              m_module.getTypeContext().getTypeBuiltinBool()));
            break;
        }

        default:
            YAL_ASSERT(false);
            PrettyPrint::SourceErrorPrint(log,
                                          node.getSourceInfo(),
                                          m_compiler.getSourceManager());
            log.error("Unknown operator type\n");
            onError();
        }
    }

    void
    ExprTypeAstVisitor::visit(ExprBoolLiteral&) {

    }

    void
    ExprTypeAstVisitor::visit(ExprIntegerLiteral& ) {

    }

    void
    ExprTypeAstVisitor::visit(ExprDecimalLiteral&) {

    }

    void
    ExprTypeAstVisitor::visit(ExprStructVarRef& node) {
        Log& log = m_compiler.getLog();
        StmtExpression* expr = node.getExpression();
        expr->acceptVisitor(*this);
        // TODO: check if experession is a valid type

        const QualType exprqt = expr->getQualType();

        // check if expression is a struct type
        if (!exprqt.getType()->isStruct()) {
            PrettyPrint::SourceErrorPrint(log,
                                          node.getSourceInfo(),
                                          m_compiler.getSourceManager());
            log.error("Type of expression '%' is not of type struct.\n",
                      exprqt);
            onError();
            onError();
        }

        // see if variable exists in that struct
        const TypeDecl* exprType = dyn_cast<TypeDecl>(exprqt.getType());
        const DeclStruct* declStruct = dyn_cast<DeclStruct>(exprType->getDecl());
        YAL_ASSERT(declStruct != nullptr);

        const DeclVar* decl = declStruct->getMemberByName(node.getMemberName());

        if (decl == nullptr) {
            PrettyPrint::SourceErrorPrint(log,
                                          node.getSourceInfo(),
                                          m_compiler.getSourceManager());
            log.error("Struct '%' does not have a member named '%'.\n",
                      declStruct->getIdentifier(),
                      node.getMemberName());
            onError();
            onError();
        }

        // set correct qualifier based on expression
        Qualifier qualifier = decl->getQualifier();

        if (!qualifier.isReference()) {
            // only enabel this qualifier for non-reference memebers
            qualifier.setRequiresReplace(true);
        }

        // if struct is immutable, mark variable as immutable
        if (exprqt.getQualifier().isImmutable()){
            qualifier.setImmutable();
        }

        node.setQualType(QualType::Create(qualifier, decl->getVarType()->getType()));
    }

    void
    ExprTypeAstVisitor::visit(ExprFnCall& node) {
        Log& log = m_compiler.getLog();

        // get function declartion
        const Type* fncallType = node.getFunctionType()->getType();
        const TypeDecl* fnType = dyn_cast<TypeDecl>(fncallType);

        if (fnType == nullptr || !fncallType->isFunction()) {
            PrettyPrint::SourceErrorPrint(log,
                                          node.getFunctionType()->getSourceInfo(),
                                          m_compiler.getSourceManager());
            log.error("Type '%' is not a function.\n",
                      fncallType->getIdentifier());
            onError();
        }

        // check function arg count

        const DeclFunction* fndecl  = dyn_cast<DeclFunction>(fnType->getDecl());
        YAL_ASSERT(fndecl != nullptr);

        ExprList* args = node.getFunctionArgs();
        DeclParamVarContainer* params =fndecl->getParams();

        const size_t callArgCount = args != nullptr ? args->getCount() : 0;
        const size_t paramCount = params != nullptr ? params->getCount() : 0;

        if (callArgCount != paramCount) {
            PrettyPrint::SourceErrorPrint(log,
                                          args->getSourceInfo(),
                                          m_compiler.getSourceManager());
            log.error("Invalid function arg count, function '%' expects % arguments,\
                      but only % provided.\n",
                      fndecl->getIdentifier(),
                      paramCount,
                      callArgCount);
            onError();
        }


        // validate function args
        if (node.getFunctionArgs() != nullptr) {
            node.getFunctionArgs()->acceptVisitor(*this);

            auto paramIter = params->childBegin();
            for (auto argIter = args->childBegin();
                 paramIter != params->childEnd();
                 ++paramIter, ++argIter) {
                if (!CanConvert((*argIter)->getQualType(),
                                (*argIter)->getSourceInfo(),
                                (*paramIter)->getVarType()->getQualType(),
                                (*paramIter)->getSourceInfo(),
                                m_compiler)) {
                    onError();
                }
            }
        }

        // Update expression type
        node.setQualType(fndecl->getReturnQualType());
    }

    void
    ExprTypeAstVisitor::visit(ExprTypeFnCall& node) {
        Log& log = m_compiler.getLog();

        StmtExpression* expr = node.getExpression();
        expr->acceptVisitor(*this);

        const QualType exprqt = expr->getQualType();

        // check if experession is a valid type
        if (!exprqt.getType()->isFunctionTargetable()) {
            PrettyPrint::SourceErrorPrint(log,
                                          expr->getSourceInfo(),
                                          m_compiler.getSourceManager());
            log.error("Expression type '%' is not function targetable.\n",
                      *exprqt.getType());
            onError();
        }

        // check if fucntion exists
        const TypeDecl* fnType =
                exprqt.getType()->getFunctionWithName(node.getFunctionName());

        if ( fnType == nullptr || !fnType->isTypeFunction()) {
            PrettyPrint::SourceErrorPrint(log,
                                          node.getSourceInfo(),
                                          m_compiler.getSourceManager());
            log.error("Type '%' does not contain any target function named '%'.\n",
                      *exprqt.getType(),
                      node.getFunctionName());
            onError();
        }

        const DeclTypeFunction* fndecl = dyn_cast<DeclTypeFunction>(fnType->getDecl());
        YAL_ASSERT(fndecl != nullptr);

        //check if function is static type
        if (fndecl->isStatic()) {
            PrettyPrint::SourceErrorPrint(log,
                                          node.getSourceInfo(),
                                          m_compiler.getSourceManager());
            log.error("Function '%' is static, use '%::%(...)' to call this function instead.\n",
                      node.getFunctionName(),
                      *exprqt.getType(),
                      node.getFunctionName());
            onError();
        }

        // check if is function is immutable

        if (exprqt.getQualifier().isImmutable() && !fndecl->isImmutable()) {
            PrettyPrint::SourceErrorPrint(log,
                                          node.getSourceInfo(),
                                          m_compiler.getSourceManager());
            log.error("Can not call mutable function '%' on immutable expression.\n",
                      fndecl->getIdentifier());
            onError();
        }


        // check function arg count

        ExprList* args = node.getFunctionArgs();
        DeclParamVarContainer* params =fndecl->getParams();

        const size_t callArgCount = args != nullptr ? args->getCount() : 0;
        const size_t paramCount = params != nullptr
                ? (fndecl->isStatic() ? params->getCount() : params->getCount() - 1 )
                : 0;

        if (callArgCount != paramCount) {
            PrettyPrint::SourceErrorPrint(log,
                                          args->getSourceInfo(),
                                          m_compiler.getSourceManager());
            log.error("Invalid function arg count, function '%' expects % arguments,\
                      but only % provided.\n",
                      fndecl->getIdentifier(),
                      paramCount,
                      callArgCount);
            onError();
        }


        // validate function args
        if (node.getFunctionArgs() != nullptr) {
            node.getFunctionArgs()->acceptVisitor(*this);

            auto paramIter = params->childBegin();
            // skip over self param var
            if (!fndecl->isStatic()) {
                paramIter++;
            }

            for (auto argIter = args->childBegin();
                 paramIter != params->childEnd();
                 ++paramIter, ++argIter) {
                if (!CanConvert((*argIter)->getQualType(),
                                (*argIter)->getSourceInfo(),
                                (*paramIter)->getVarType()->getQualType(),
                                (*paramIter)->getSourceInfo(),
                                m_compiler)) {
                    onError();
                }
            }
        }

        // update function type
        node.setFunctionType(fnType);
        // Update expression type
        node.setQualType(fndecl->getReturnQualType());
    }


    void
    ExprTypeAstVisitor::visit(ExprTypeFnCallStatic& node) {
        Log& log = m_compiler.getLog();

        const RefType* targetRefType = node.getTargetType();
        const Type* targetType = targetRefType->getType();

        // check if experession is a valid type
        if (!targetType->isFunctionTargetable()) {
            PrettyPrint::SourceErrorPrint(log,
                                          targetRefType->getSourceInfo(),
                                          m_compiler.getSourceManager());
            log.error("Type '%' is not function targetable.\n",
                      *targetType);
            onError();
        }

        // check if fucntion exists
        const TypeDecl* fnType =
                targetType->getFunctionWithName(node.getFunctionName());

        if ( fnType == nullptr || !fnType->isTypeFunction()) {
            PrettyPrint::SourceErrorPrint(log,
                                          node.getSourceInfo(),
                                          m_compiler.getSourceManager());
            log.error("Type '%' does not contain any target function named '%'.\n",
                      *targetType,
                      node.getFunctionName());
            onError();
        }

        const DeclTypeFunction* fndecl = dyn_cast<DeclTypeFunction>(fnType->getDecl());
        YAL_ASSERT(fndecl != nullptr);

        //check if function is static type
        if (!fndecl->isStatic()) {
            PrettyPrint::SourceErrorPrint(log,
                                          node.getSourceInfo(),
                                          m_compiler.getSourceManager());
            log.error("Function '%' is not static, it must be called from an "
                      "instance of this struct.\n",
                      node.getFunctionName());
            onError();
        }

        // check function arg count

        ExprList* args = node.getFunctionArgs();
        DeclParamVarContainer* params =fndecl->getParams();

        const size_t callArgCount = args != nullptr ? args->getCount() : 0;
        const size_t paramCount = params != nullptr
                ? params->getCount()
                : 0;

        if (callArgCount != paramCount) {
            PrettyPrint::SourceErrorPrint(log,
                                          args->getSourceInfo(),
                                          m_compiler.getSourceManager());
            log.error("Invalid function arg count, function '%' expects % arguments,\
                      but only % provided.\n",
                      fndecl->getIdentifier(),
                      paramCount,
                      callArgCount);
            onError();
        }


        // validate function args
        if (node.getFunctionArgs() != nullptr) {
            node.getFunctionArgs()->acceptVisitor(*this);

            auto paramIter = params->childBegin();
            // skip over self param var
            if (!fndecl->isStatic()) {
                paramIter++;
            }

            for (auto argIter = args->childBegin();
                 paramIter != params->childEnd();
                 ++paramIter, ++argIter) {
                if (!CanConvert((*argIter)->getQualType(),
                                (*argIter)->getSourceInfo(),
                                (*paramIter)->getVarType()->getQualType(),
                                (*paramIter)->getSourceInfo(),
                                m_compiler)) {
                    onError();
                }
            }
        }

        // update function type
        node.setFunctionType(fnType);
        // Update expression type
        node.setQualType(fndecl->getReturnQualType());
    }

    void
    ExprTypeAstVisitor::visit(DeclParamVarSelf&) {

    }


    void
    ExprTypeAstVisitor::visit(ExprVarRefSelf& node) {
        YAL_ASSERT(node.getQualType().getType() != nullptr);
    }

    void
    ExprTypeAstVisitor::visit(ExprRangeCast& node) {
        Log& log = m_compiler.getLog();
        const QualType dstType = node.getDestType()->getQualType();
        const TypeBuiltin* dstBuiltinType = dyn_cast<TypeBuiltin>(dstType.getType());
        if (dstBuiltinType == nullptr || !dstBuiltinType->isNumeric()) {

            PrettyPrint::SourceErrorPrint(log,
                                          node.getDestType()->getSourceInfo(),
                                          m_compiler.getSourceManager());
            log.error("range_cast<> can only be used to cast to builtin \
                      numeric types.\n");
                      onError();
        }

        StmtExpression* expr = node.getExpression();
        expr->acceptVisitor(*this);

        const QualType exprType = expr->getQualType();
        const TypeBuiltin* exprBuiltinType = dyn_cast<TypeBuiltin>(exprType.getType());


        if (exprBuiltinType == nullptr || !exprBuiltinType->isNumeric()) {

            PrettyPrint::SourceErrorPrint(log,
                                          expr->getSourceInfo(),
                                          m_compiler.getSourceManager());
            log.error("range_cast<> can only be used with builtin \
                      numeric types.\n");
                      onError();
        }

        const Qualifier qualFrom = exprType.getQualifier();
        const Qualifier qualTo = dstType.getQualifier();

        if (qualFrom.isImmutable() && !qualTo.isImmutable()) {
            PrettyPrint::SourceErrorPrint(log,
                                          expr->getSourceInfo(),
                                          m_compiler.getSourceManager());
            log.error("Cannot convert from '%' to '%', can't convert immutable to mutable.\n",
                      exprType,dstType);
            onError();
        }

        if (qualFrom.isReference() && !qualTo.isReference()) {
            PrettyPrint::SourceErrorPrint(log,
                                          expr->getSourceInfo(),
                                          m_compiler.getSourceManager());
            if (!dstBuiltinType->isTriviallyCopiable()
                    || !exprBuiltinType->isTriviallyCopiable()) {
                log.error("Cannot convert from '%' to '%', types are not \
                          trivially copiable. Use copy trait.\n",
                          exprType,dstType);
                onError();
            }
        }


    }


    void
    ExprTypeAstVisitor::visit(ExprStructInit& node) {

        Log& log = m_compiler.getLog();
        // check if type is struct

        const Type* structType = node.getStructType()->getType();
        const TypeDecl* declStrucType = dyn_cast<TypeDecl>(structType);

        if (declStrucType == nullptr || !structType->isStruct()) {
            PrettyPrint::SourceErrorPrint(log,
                                          node.getSourceInfo(),
                                          m_compiler.getSourceManager());
            log.error("Can not use struct initializer on non struct type '%'.",
                      *structType);
            onError();
        }

        DeclStruct* declStruct = dyn_cast<DeclStruct>(declStrucType->getDecl());
        YAL_ASSERT(declStruct != nullptr);

        // count members without default constructors
        uint32_t membersWithoutDefault = 0;

        for (auto& member : declStruct->getMembers()->getChildRange()) {
            if (member->getExpression() == nullptr) {
                membersWithoutDefault++;
            }
        }


        if (node.getMemberInitList() != nullptr) {
            for (auto& memberInit : node.getMemberInitList()->getChildRange()) {

                // check if the member actually exists ,maybe do this elsewhere?
                const DeclVar* member
                        = declStruct->getMemberByName(memberInit->getMemberName());
                if (member == nullptr) {
                    PrettyPrint::SourceErrorPrint(log,
                                                  member->getSourceInfo(),
                                                  m_compiler.getSourceManager());
                    log.error("Struct '%' has no member named '%'.",
                              *structType,
                              memberInit->getMemberName());
                    onError();
                }

                // visit expression
                StmtExpression* initExpr = memberInit->getInitExpr();
                initExpr->acceptVisitor(*this);

                // check if we can expression type is valid

                const QualType from = initExpr->getQualType();
                const QualType to = member->getVarType()->getQualType();

                const Type* typeFrom= from.getType();
                const Type* typeTo = to.getType();


                // check types
                if(!typeFrom->isCastableTo(typeTo)) {
                    PrettyPrint::SourceErrorPrint(log,
                                                  memberInit->getSourceInfo(),
                                                  m_compiler.getSourceManager());
                    log.error("Cannot convert from '%' to '%', types are not compatible.\n",
                              from,to);
                    onError();
                }

                // check qualifiers
                const Qualifier qualFrom= from.getQualifier();
                const Qualifier qualTo = to.getQualifier();
                if (qualFrom.isReference() && !qualTo.isReference()) {
                    PrettyPrint::SourceErrorPrint(log,
                                                  memberInit->getSourceInfo(),
                                                  m_compiler.getSourceManager());
                    //TODO: insert copy trait
                    if (!typeTo->isTriviallyCopiable() || !typeFrom->isTriviallyCopiable()) {
                        log.error("Cannot convert from '%' to '%', types are not \
                                  trivially copiable. Use copy trait.\n",
                                  from,to);
                        onError();
                    }
                }


                if (member->getExpression() == nullptr) {
                    membersWithoutDefault--;
                }
            }

        }

        // check if all members are properly initialized

        if (membersWithoutDefault != 0) {
            PrettyPrint::SourceErrorPrint(log,
                                          node.getSourceInfo(),
                                          m_compiler.getSourceManager());
            log.error("Can not create struct '%', % member(s) remain(s) unintialized.\n",
                      *structType,
                      membersWithoutDefault);
            onError();
        }

    }

    void
    ExprTypeAstVisitor::visit(StructMemberInit&) {
        YAL_ASSERT(false);

    }

    StageExprType::StageExprType(Compiler& compiler,
                                 Module& module):
        m_compiler(compiler),
        m_module(module){
    }


    bool
    StageExprType::execute(DeclBase* decl) {
        StackJump jmp;
        ExprTypeAstVisitor visitor(m_compiler,m_module, jmp);
        jmp.mark();
        decl->acceptVisitor(visitor);
        return !visitor.didError();
    }




}
