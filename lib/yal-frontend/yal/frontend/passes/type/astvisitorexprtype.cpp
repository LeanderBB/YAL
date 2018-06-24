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

#include "yal/frontend/passes/type/astvisitorexprtype.h"
#include "yal/error/errorreporter.h"
#include "yal/frontend/ast/astnodes.h"
#include "yal/frontend/module.h"
#include "yal/frontend/passes/decl/errorspassdecl.h"
#include "yal/frontend/passes/type/errorspasstype.h"
#include "yal/frontend/passes/passes.h"
#include "yal/frontend/parser/sttype.h"
#include "yal/frontend/types/operators.h"
#include "yal/frontend/types/typebuiltin.h"
#include "yal/frontend/types/typefunction.h"
#include "yal/frontend/types/typestruct.h"

namespace yal::frontend {

    class AstVisitorExprType::DeclScopeGuard {
    public:
        DeclScopeGuard(AstVisitorExprType& visitor,
                       const DeclScope& scope):
            m_visitor(visitor),
    #if defined(YAL_ASSERT_ENABLED)
            m_scope(scope),
    #endif
            m_prevScope(visitor.m_activeScope){
            YAL_ASSERT(visitor.m_activeScope == scope.getParentScope());
            visitor.m_activeScope = &scope;
        }

        ~DeclScopeGuard() {
            YAL_ASSERT(m_visitor.m_activeScope == &m_scope);
            m_visitor.m_activeScope = m_prevScope;
        }

    private:
        AstVisitorExprType& m_visitor;
#if defined(YAL_ASSERT_ENABLED)
        const DeclScope& m_scope;
#endif
        const DeclScope* m_prevScope;
    };

    AstVisitorExprType::AstVisitorExprType(PassOptions &options):
        m_module(options.module),
        m_errReporter(options.errReporter),
        m_activeScope(nullptr),
        m_jump() {

    }

    void
    AstVisitorExprType::execute() {
        DeclModule* modDecl = m_module.getDeclNode();
        m_jump.mark();
        modDecl->acceptVisitor(*this);
    }

    void
    AstVisitorExprType::visit(DeclModule& node) {
        DeclScopeGuard guard(*this, node.getModuleDeclScope());
        for (auto& decl : node.getDeclarations()) {
            decl->acceptVisitor(*this);
        }
    }

    void
    AstVisitorExprType::visit(DeclFunction& node) {
        DeclScopeGuard guard(*this, node.getFunctionScope());
        DeclFunction::Body& body = node.getFunctionBody();
        for (auto& stmt : body) {
            stmt->acceptVisitor(*this);
        }
    }

    void
    AstVisitorExprType::visit(DeclTypeFunction& node) {
        DeclScopeGuard guard(*this, node.getFunctionScope());
        DeclFunction::Body& body = node.getFunctionBody();
        for (auto& stmt : body) {
            stmt->acceptVisitor(*this);
        }
    }

    void
    AstVisitorExprType::visit(DeclStruct& node) {
        DeclScopeGuard guard(*this, node.getStructDeclScope());
        DeclStruct::Members& members = node.getMembers();
        for (auto& member: members) {
            member->acceptVisitor(*this);
        }

    }

    void
    AstVisitorExprType::visit(DeclVar& node) {

        if (DeclVar::StmtExpressionOpt exprOpt = node.getExpression(); exprOpt.has_value()) {
            // visit expr
            StmtExpression* expr = exprOpt.value();
            expr->acceptVisitor(*this);

            // check type conversion
            const QualType qtVar = node.getQualType();
            const QualType qtExpr = expr->getQualType();
            isValidTypeConversion(qtExpr,
                                  qtVar,
                                  node.getSourceInfo());

        } else {
#if defined(YAL_ASSERT_ENABLED)
            const QualType qtVar = node.getQualType();
            YAL_ASSERT(qtVar.isValid());
#endif
        }
    }

    void
    AstVisitorExprType::visit(DeclParamVar& node) {
        (void) node;
        YAL_ASSERT(node.getQualType().isValid());
    }

    void
    AstVisitorExprType::visit(DeclStrongAlias&) {}

    void
    AstVisitorExprType::visit(DeclWeakAlias&) {}

    void
    AstVisitorExprType::visit(StmtReturn& node) {
        const DeclScope* enclosingScope = m_activeScope->getFirstScopeOfTypeDecl();
        YAL_ASSERT(enclosingScope->isFunctionTypeScope());
        YAL_ASSERT(enclosingScope->getScopeDecl() != nullptr);

        StmtReturn::StmtExpressionOpt exprOpt = node.getExpression();
        if (exprOpt.has_value()) {
            // visit return expr
            StmtExpression* expr = exprOpt.value();
            expr->acceptVisitor(*this);

            // Get function decl
            const DeclBase* decl = enclosingScope->getScopeDecl();
            YAL_ASSERT(decl->getAstType() == AstType::DeclFunction
                       || decl->getAstType() == AstType::DeclTypeFunction);
            const DeclFunction* declFn = static_cast<const DeclFunction*>(decl);

            // check type conversion
            isValidTypeConversion(expr->getQualType(),
                                  declFn->getReturnType(),
                                  node.getSourceInfo());
        }
    }

    void
    AstVisitorExprType::visit(StmtDecl& node) {
        node.getDecl()->acceptVisitor(*this);
    }

    void
    AstVisitorExprType::visit(StmtAssign& node) {
        StmtExpression* exprLeft = node.getLeftExpr();
        StmtExpression* exprRight = node.getRightExpr();

        // visit nodes
        exprLeft->acceptVisitor(*this);
        exprRight->acceptVisitor(*this);

        const QualType qtFrom = exprRight->getQualType();
        const QualType qtTo = exprLeft->getQualType();

        // check for LValue assignment
        if (!qtTo.isLValue()) {
            auto error = std::make_unique<ErrorTypeExprUnassignable>(*exprLeft);
            onError(std::move(error));
        }

        // check conversion type
        isValidTypeConversion(qtFrom, qtTo, node.getSourceInfo());

        if (qtTo.isImmutable()) {
            auto error = std::make_unique<ErrorTypeAssignToImmutable>(node);
            onError(std::move(error));
        }
    }

    void
    AstVisitorExprType::visit(ExprVarRef& node) {
        (void) node;
        YAL_ASSERT(node.getQualType().isValid());
    }

    void
    AstVisitorExprType::visit(ExprUnaryOperator& node) {
        StmtExpression* expr = node.getExpression();
        expr->acceptVisitor(*this);
        const QualType qtExpr = expr->getQualType();
        const Type* typeExpr = qtExpr.getType();
        switch(node.getOperatorType()) {

        case UnaryOperatorType::BitNot: {
            const TypeBuiltin* typeBuiltin = dyn_cast<TypeBuiltin>(typeExpr);
            if (typeBuiltin == nullptr || !typeBuiltin->isUnsigned()) {
                auto error = std::make_unique<ErrorTypeBitOpNonUnsigned>(node);
                onError(std::move(error));
            }
            Qualifier qual;
            qual.setMutable();
            node.setQualType(QualType::Create(qual, typeExpr));
            break;
        }
        case UnaryOperatorType::Negate: {
            const TypeBuiltin* typeBuiltin = dyn_cast<TypeBuiltin>(typeExpr);
            if (typeBuiltin == nullptr || !typeBuiltin->isSigned()) {
                auto error = std::make_unique<ErrorTypeUnaryOpNegate>(node);
                onError(std::move(error));
            }
            Qualifier qual;
            qual.setMutable();
            node.setQualType(QualType::Create(qual, typeExpr));
            break;
        }
        case UnaryOperatorType::Not: {
            const TypeBuiltin* typeBuiltin = dyn_cast<TypeBuiltin>(typeExpr);
            if (typeBuiltin == nullptr || !typeBuiltin->isBool()) {
                auto error = std::make_unique<ErrorTypeBoolOpNonBool>(node);
                onError(std::move(error));
            }
            Qualifier qual;
            qual.setMutable();
            node.setQualType(QualType::Create(qual, typeExpr));
            break;
        }
        case UnaryOperatorType::Reference: {
            // chek for double reference
            if (qtExpr.isReference()) {
                auto error = std::make_unique<ErrorTypeUnaryOpRefDoubleRef>(node);
                onError(std::move(error));
            }

            // check if expression is an lvalue
            if (!qtExpr.isLValue()) {
                auto error = std::make_unique<ErrorTypeUnaryOpRefNonLValue>(node);
                onError(std::move(error));
            }

            Qualifier qual = qtExpr.getQualifier();
            qual.setReference();
            node.setQualType(QualType::Create(qual, typeExpr));
            break;
        }
        default:
            YAL_ASSERT_MESSAGE(false, "Shouldn't be reached!");
        }
    }

    void
    AstVisitorExprType::visit(ExprBinaryOperator& node) {
        StmtExpression* exprLeft = node.getExpressionLeft();
        StmtExpression* exprRight = node.getExpressionRight();
        exprLeft->acceptVisitor(*this);
        exprRight->acceptVisitor(*this);

        const QualType qtLeft = exprLeft->getQualType();
        const QualType qtRight = exprRight->getQualType();

        switch (node.getOperatorType()) {
        case BinaryOperatorType::Div:
        case BinaryOperatorType::Mult:
        case BinaryOperatorType::Plus:
        case BinaryOperatorType::Minus:{

            // check if types are convertible
            isValidTypeConversion(qtRight, qtLeft, node.getSourceInfo());

            //TODO:Revist when traits are available
            if (dyn_cast<TypeBuiltin>(qtLeft.getType()) == nullptr) {
                auto error = std::make_unique<ErrorTypeMissingTrait>(*qtLeft.getType(),
                                                                     node.getSourceInfo());
                onError(std::move(error));
            }

            Qualifier qual;
            qual.setMutable();
            node.setQualType(QualType::Create(qual, qtLeft.getType()));
            break;
        }

        case BinaryOperatorType::BitAnd:
        case BinaryOperatorType::BitOr:
        case BinaryOperatorType::BitXor:{
            // check if left and right are unsigned integers
            const TypeBuiltin* typeLeft= dyn_cast<TypeBuiltin>(qtLeft.getType());
            const TypeBuiltin* typeRight = dyn_cast<TypeBuiltin>(qtRight.getType());

            if (typeLeft == nullptr
                    || typeRight == nullptr
                    || !typeLeft->isUnsigned()
                    || !typeRight->isUnsigned()) {
                auto error = std::make_unique<ErrorTypeBitOpNonUnsigned>(node);
                onError(std::move(error));
            }

            // check if types are convertible
            isValidTypeConversion(qtRight, qtLeft, node.getSourceInfo());

            Qualifier qual;
            qual.setMutable();
            node.setQualType(QualType::Create(qual, qtLeft.getType()));
            break;
        }

        case BinaryOperatorType::Eq:
        case BinaryOperatorType::Ne:
        case BinaryOperatorType::Lt:
        case BinaryOperatorType::Le:
        case BinaryOperatorType::Ge:
        case BinaryOperatorType::Gt: {

            // check if types are convertible
            isValidTypeConversion(qtRight, qtLeft, node.getSourceInfo());

            if (dyn_cast<TypeBuiltin>(qtLeft.getType()) == nullptr) {
                // TODO : revisit when traits are available <=>
                auto error = std::make_unique<ErrorTypeMissingTrait>(*qtLeft.getType(),
                                                                     node.getSourceInfo());
                onError(std::move(error));
            }

            const Type* typeBool = m_module.getTypeContext().getTypeBuiltinBool();
            Qualifier qual;
            qual.setMutable();
            node.setQualType(QualType::Create(qual,typeBool));
            break;
        }

        case BinaryOperatorType::And:
        case BinaryOperatorType::Or: {
            // check if both types are boolean
            const TypeBuiltin* typeLeft= dyn_cast<TypeBuiltin>(qtLeft.getType());
            const TypeBuiltin* typeRight = dyn_cast<TypeBuiltin>(qtRight.getType());

            if (typeLeft == nullptr
                    || typeRight == nullptr
                    || !typeLeft->isBool()
                    || !typeRight->isBool()) {
                auto error = std::make_unique<ErrorTypeBoolOpNonBool>(node);
                onError(std::move(error));
            }

            const Type* typeBool = m_module.getTypeContext().getTypeBuiltinBool();
            Qualifier qual;
            qual.setMutable();
            node.setQualType(QualType::Create(qual, typeBool));
            break;
        }

        default:
            YAL_ASSERT_MESSAGE(false, "Shouldn't be reached!");
        }
    }

    void
    AstVisitorExprType::visit(ExprBoolLiteral&) {}

    void
    AstVisitorExprType::visit(ExprIntegerLiteral&) {}

    void
    AstVisitorExprType::visit(ExprFloatLiteral&) {}

    void
    AstVisitorExprType::visit(ExprStructVarRef& node) {
        StmtExpression* expr = node.getExpression();
        expr->acceptVisitor(*this);
        const QualType qtExpr = expr->getQualType();

        // check if type of expression is a struct type
        const TypeStruct* typeStruct = dyn_cast<TypeStruct>(qtExpr.getType());
        if (typeStruct == nullptr) {
            auto error = std::make_unique<ErrorTypeStructVarRefNonStruct>(node);
            onError(std::move(error));
        }

        const DeclStruct& decl = typeStruct->getDecl();

        std::optional<const DeclVar*> declOp =
                decl.getMemberByName(node.getMemberName());
        if (!declOp.has_value()) {
            auto error = std::make_unique<ErrorTypeStructVarRefUndefinedMember>(node);
            onError(std::move(error));
        }

        const QualType qtDecl = declOp.value()->getQualType();

        Qualifier qual = qtDecl.getQualifier();

        if (!qual.isReference()) {
            // only enabel this qualifier for non-reference memebers
            qual.setRequiresReplace(true);
        }

        // if struct is immutable, mark variable as immutable
        if (qtExpr.isImmutable()){
            qual.setImmutable();
        } else if (qtExpr.isMutable()) {
            qual.setMutable();
        }

        node.setQualType(QualType::Create(qual, qtDecl.getType()));
    }

    void
    AstVisitorExprType::visit(ExprFnCall& node) {
        const TypeFunction* typeFn = node.getFunctionType();
        YAL_ASSERT(typeFn->isFunction());

        const DeclFunction& declFn = typeFn->getDecl();

        const ExprList& args = node.getFunctionArgs();
        const DeclFunction::Params& params = declFn.getParams();

        // Check arg number match
        if (args.size() != params.size()) {
            auto error = std::make_unique<ErrorTypeFnCallInvalidArgCount>(node);
            onError(std::move(error));
        }

        // Validate args
        if (!args.empty()) {
            auto itParam = params.begin();
            auto itArgs = args.begin();
            for(; itArgs != args.end(); ++itArgs, ++itParam) {
                StmtExpression* expr = *itArgs;
                expr->acceptVisitor(*this);
                const QualType qtExpr = expr->getQualType();
                const QualType qtParam = (*itParam)->getQualType();
                isValidTypeConversion(qtExpr, qtParam, expr->getSourceInfo());
            }
        }

        node.setQualType(declFn.getReturnType());
    }

    void
    AstVisitorExprType::visit(ExprTypeFnCall& node) {

        const TypeFunction* typeFn = nullptr;
        const DeclFunction* declFn = nullptr;
        const DeclTypeFunction* declTypeFn = nullptr;
        QualType qtTarget;

        const bool isStaticFn = node.isStaticCall();
        if (isStaticFn) {
            // static function call, types have been evaluated
            typeFn = node.getFunctionType();
            YAL_ASSERT(typeFn != nullptr && typeFn->isTypeFunctionStatic());
            declFn = &typeFn->getDecl();
            YAL_ASSERT(declFn != nullptr);

        } else {
            // runtime tyep call, need to evaluate expr type
            ExprTypeFnCall::StmtExpressionOpt exprOpt = node.getExpression();
            if (exprOpt.has_value()) {
                // check if type is function targetable
                StmtExpression* expr = exprOpt.value();
                expr->acceptVisitor(*this);
                qtTarget = expr->getQualType();
                const Type* typeTarget = qtTarget.getType();
                const STIdentifier* functionIdentifier = node.getFunctionName();
                // can we actually call functions on this type
                if (!typeTarget->isFunctionTargetable()) {
                    auto error = std::make_unique<ErrorFnOnNonTargetType>(functionIdentifier->getString(),
                                                                          functionIdentifier->getSourceInfo(),
                                                                          typeTarget);
                    onError(std::move(error));
                }
                // check if function exists
                typeFn = typeTarget->getFunctionWithName(functionIdentifier->getString());
                if (typeFn == nullptr) {
                    auto error = std::make_unique<ErrorTypeFunctionUndefined>(*typeTarget,
                                                                              *functionIdentifier);
                    onError(std::move(error));
                }

                if (!typeFn->isTypeFunction() || typeFn->isTypeFunctionStatic()) {
                    auto error = std::make_unique<ErrorTypeIsNotTypeFunction>(*typeFn,
                                                                              node.getFunctionName()->getSourceInfo());
                    onError(std::move(error));
                }

                declFn = &typeFn->getDecl();
                // update function type now that we know
                node.setFunctionType(typeFn);
            } else {
                YAL_ASSERT(false);
            }
        }

        declTypeFn = dyn_cast<DeclTypeFunction>(declFn);
        YAL_ASSERT(declTypeFn != nullptr);

        // check if type function mutability matches
        if (qtTarget.isValid()
                && qtTarget.isImmutable()
                && !declTypeFn->isImmutable()) {
            auto error = std::make_unique<ErrorTypeTypeFnCallImmutable>(node,
                                                                        *declTypeFn);
            onError(std::move(error));
        }

        const ExprList& args = node.getFunctionArgs();
        const DeclFunction::Params& params = declFn->getParams();

        const size_t argSize = args.size();
        const size_t countParam = isStaticFn
                ? params.size()
                : params.size() - 1;

        // Check arg number match
        if (argSize != countParam) {
            auto error = std::make_unique<ErrorTypeFnCallInvalidArgCount>(node);
            onError(std::move(error));
        }

        // Validate args
        if (!args.empty()) {
            auto itParam = params.begin();
            if (!isStaticFn) {
                // adavance on param iterator to skip of self
                ++itParam;
            }
            auto itArgs = args.begin();
            for(; itArgs != args.end(); ++itArgs, ++itParam) {
                StmtExpression* expr = *itArgs;
                expr->acceptVisitor(*this);
                const QualType qtExpr = expr->getQualType();
                const QualType qtParam = (*itParam)->getQualType();
                isValidTypeConversion(qtExpr, qtParam, expr->getSourceInfo());
            }
        }

        node.setQualType(declFn->getReturnType());
    }

    void
    AstVisitorExprType::visit(DeclParamVarSelf& node) {
        (void) node;
        YAL_ASSERT(node.getQualType().isValid());
    }

    void
    AstVisitorExprType::visit(ExprVarRefSelf& node) {
        (void) node;
        YAL_ASSERT(node.getQualType().isValid());
    }

    void
    AstVisitorExprType::visit(ExprCast& node) {
        StmtExpression& expr = node.getExpression();
        expr.acceptVisitor(*this);
        // check if types are castable
        isValidCastConversion(node);
    }

    void
    AstVisitorExprType::visit(ExprStructInit& node) {
        const QualType qt = node.getQualType();
        const TypeStruct* typeStruct = dyn_cast<TypeStruct>(qt.getType());
        YAL_ASSERT(typeStruct != nullptr);
        const DeclStruct& decl = typeStruct->getDecl();

        // check if all members are initialized
        ExprStructInit::MemberInitExprList& memberInits = node.getMemberInitExprList();
        if (memberInits.size() != decl.getMembers().size()) {
            auto error = std::make_unique<ErrorTypeStructInitNumMembers>(node);
            onError(std::move(error));
        }

        for (auto& memberInit : memberInits) {
            // get member
            auto declMemberOpt = decl.getMemberByName(memberInit->getMemberName());
            YAL_ASSERT(declMemberOpt.has_value());
            const DeclVar* declMember = declMemberOpt.value();

            // eval expression
            StmtExpression* expr = memberInit->getInitExpr();
            expr->acceptVisitor(*this);
            const QualType exprQt = expr->getQualType();

            // verify conversion
            isValidTypeConversion(exprQt,
                                  declMember->getQualType(),
                                  memberInit->getSourceInfo());
        }
    }

    void
    AstVisitorExprType::visit(StructMemberInit&) {}


    void
    AstVisitorExprType::visit(StmtListScoped& node) {
        DeclScopeGuard guard(*this, node.getListScope());
        for (auto& stmt : node.getStatements()) {
            stmt->acceptVisitor(*this);
        }
    }

    void
    AstVisitorExprType::onError(std::unique_ptr<Error>&& error) {
        const bool isFatal = error->isFatal();
        m_errReporter.report(std::move(error));
        if (isFatal) {
            m_jump.trigger();
        }
    }

    void
    AstVisitorExprType::isValidTypeConversion(const QualType qtFrom,
                                              const QualType qtTo,
                                              const SourceInfo &location) {
        YAL_ASSERT(qtFrom.isValid() && qtTo.isValid());

        const Type* typeFrom = qtFrom.getType();
        const Type* typeTo = qtTo.getType();

        if (!typeFrom->isCastableToAuto(*typeTo)) {
            auto error = std::make_unique<ErrorTypeIncompatible>(qtFrom,
                                                                 qtTo,
                                                                 location);
            onError(std::move(error));
        }

        if (qtFrom.isImmutable()
                && !qtTo.isImmutable()
                && (!qtFrom.isTriviallyCopiable() || !qtTo.isTriviallyCopiable())) {
            auto error = std::make_unique<ErrorTypeMutability>(qtFrom,
                                                               qtTo,
                                                               location);
            onError(std::move(error));
        }

        if (qtFrom.isReference() != qtTo.isReference()) {
            auto error = std::make_unique<ErrorTypeReference>(qtFrom,
                                                              qtTo,
                                                              location);
            onError(std::move(error));
        }
    }

    void
    AstVisitorExprType::isValidCastConversion(const ExprCast &cast) {
        const QualType qtFrom = cast.getExpression().getQualType();
        const QualType qtTo = cast.getQualType();

        const Type* typeFrom = qtFrom.getType();
        const Type* typeTo = qtTo.getType();

        if (!typeFrom->isCastableToRequest(*typeTo)) {
            auto error = std::make_unique<ErrorTypeIncompatibleCast>(cast);
            onError(std::move(error));
        }

        if (qtFrom.isImmutable()
                && !qtTo.isImmutable()
                && (!qtFrom.isTriviallyCopiable() || !qtTo.isTriviallyCopiable())) {
            auto error = std::make_unique<ErrorTypeMutability>(qtFrom,
                                                               qtTo,
                                                               cast.getSourceInfo());
            onError(std::move(error));
        }

        if (!qtTo.isTriviallyCopiable()
                && (!qtFrom.isReference() || !qtTo.isReference())) {
            auto error = std::make_unique<ErrorTypeCastReference>(cast);
            onError(std::move(error));
        }

        if (qtTo.isTriviallyCopiable() && qtTo.isReference()) {
            auto error = std::make_unique<ErrorTypeCastReference>(cast);
            onError(std::move(error));
        }
    }
}
