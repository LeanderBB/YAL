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

#include "yal/frontend/passes/move/astvisitormovecheck.h"
#include "yal/frontend/passes/move/errorspassmove.h"
#include "yal/error/errorreporter.h"
#include "yal/frontend/ast/astvisitorimpl.h"
#include "yal/frontend/module.h"
#include "yal/frontend/passes/passes.h"
#include "yal/frontend/parser/sttype.h"
#include "yal/frontend/types/operators.h"
#include "yal/frontend/types/typebuiltin.h"
#include "yal/frontend/types/typefunction.h"
#include "yal/frontend/types/typestruct.h"

namespace yal::frontend {

    class AstVisitorMoveCheck::DeclScopeGuard {
    public:

        YAL_NO_COPY_CLASS(DeclScopeGuard);

        DeclScopeGuard(AstVisitorMoveCheck& visitor,
                       const DeclScope& scope):
            m_visitor(visitor),
    #if defined(YAL_ASSERT_ENABLED)
            m_scope(scope),
    #endif
            m_prevScope(visitor.m_activeScope){
            YAL_ASSERT(visitor.m_activeScope == scope.getParentScope());
            visitor.m_activeScope = &scope;

            // update move map, register all declaration from the scope
            MoveMap& map = visitor.m_moveMap;
            for (auto& declit : visitor.m_activeScope->getDeclsConst()) {
                const DeclBase* decl =declit.second;
                switch (decl->getAstType()) {

                case AstType::DeclVar: {
                    MoveState state;
                    map.insert(std::make_pair(decl, state));
                    break;
                }
                case AstType::DeclParamVar:
                case AstType::DeclParamVarSelf:{
                    MoveState state;
                    state.moved = false;
                    map.insert(std::make_pair(decl, state));
                    break;
                }
                default:
                    continue;
                }
            }
        }

        ~DeclScopeGuard() {

            // clear all declaration from current scope
            MoveMap& map = m_visitor.m_moveMap;
            for (auto& declit : m_visitor.m_activeScope->getDeclsConst()) {
                const DeclBase* decl =declit.second;
                switch (decl->getAstType()) {
                case AstType::DeclVar:
                case AstType::DeclParamVar:
                case AstType::DeclParamVarSelf:
                    map.erase(decl);
                default:
                    continue;
                }
            }

            YAL_ASSERT(m_visitor.m_activeScope == &m_scope);
            m_visitor.m_activeScope = m_prevScope;
        }

    private:
        AstVisitorMoveCheck& m_visitor;
#if defined(YAL_ASSERT_ENABLED)
        const DeclScope& m_scope;
#endif
        const DeclScope* m_prevScope;
    };


    class AstVisitorMoveCheck::EvalMoveStateScope {
    public:
        EvalMoveStateScope(EvalMoveState& curState,
                           const EvalMoveState& newState):
            m_cur(curState),
            m_prev(curState){
            m_cur = newState;
        }

        ~EvalMoveStateScope() {
            m_cur = m_prev;
        }

        YAL_NO_COPY_CLASS(EvalMoveStateScope);

    private:
        EvalMoveState& m_cur;
        const EvalMoveState m_prev;
    };

    AstVisitorMoveCheck::AstVisitorMoveCheck(PassOptions& options):
        m_module(options.module),
        m_errReporter(options.errReporter),
        m_activeScope(nullptr),
        m_jump() {

    }

    void
    AstVisitorMoveCheck::execute() {
        DeclModule* modDecl = m_module.getDeclNode();
        m_jump.mark();
        resolve(*modDecl);
    }

    void
    AstVisitorMoveCheck::visit(const DeclModule& node) {
        DeclScopeGuard guard(*this, node.getModuleDeclScope());
        for (auto& decl : node.getDeclarations()) {
            resolve(*decl);
        }
    }

    void
    AstVisitorMoveCheck::visit(const DeclFunction& node) {
        DeclScopeGuard guard(*this, node.getFunctionScope());
        const DeclFunction::Body& body = node.getFunctionBody();
        for (auto& stmt : body) {
            resolve(*stmt);
        }
    }

    void
    AstVisitorMoveCheck::visit(const DeclTypeFunction& node) {
        DeclScopeGuard guard(*this, node.getFunctionScope());
        const DeclFunction::Body& body = node.getFunctionBody();
        for (auto& stmt : body) {
            resolve(*stmt);
        }
    }

    void
    AstVisitorMoveCheck::visit(const DeclTypeFunctions& node) {
        DeclScopeGuard guard(*this, node.getImplScope());
        for (auto& decl : node.getDecls()) {
            resolve(*decl);
        }
    }

    void
    AstVisitorMoveCheck::visit(const DeclStruct& node) {
        DeclScopeGuard guard(*this, node.getStructDeclScope());
        const DeclStruct::Members& members = node.getMembers();
        for (auto& member: members) {
            resolve(*member);
        }

    }

    void
    AstVisitorMoveCheck::visit(const DeclVar& node) {
        auto exprOpt = node.getExpression();
        if (exprOpt.has_value()) {
            const QualType qtDecl = node.getQualType();
            if (!qtDecl.isReference()) {
                EvalMoveState newEvalState;
                newEvalState.enabled = true;
                newEvalState.scope = m_activeScope;
                EvalMoveStateScope evalGuard(m_evalMoveState, newEvalState);

                const StmtExpression* expr = exprOpt.value();
                resolve(*expr);
            }
        }
        MoveState& state = findState(node);
        state.moved = false;
        state.stmtWhereMoved = nullptr;
    }

    void
    AstVisitorMoveCheck::visit(const DeclParamVar&) {
        YAL_ASSERT(false);
    }

    void
    AstVisitorMoveCheck::visit(const DeclStrongAlias&) {}

    void
    AstVisitorMoveCheck::visit(const DeclWeakAlias&) {}

    void
    AstVisitorMoveCheck::visit(const StmtReturn& node) {
        auto exprOpt = node.getExpression();
        if (exprOpt.has_value()) {

            EvalMoveState newEvalState;
            newEvalState.enabled = true;
            newEvalState.scope = m_activeScope;
            newEvalState.exit = true;
            EvalMoveStateScope evalGuard(m_evalMoveState, newEvalState);

            const StmtExpression* expr = exprOpt.value();
            resolve(*expr);
        }
    }

    void
    AstVisitorMoveCheck::visit(const StmtDecl& node) {
        resolve(*node.getDecl());
    }

    void
    AstVisitorMoveCheck::visit(const StmtAssign& node) {
        const StmtExpression* exprLeft = node.getLeftExpr();
        const StmtExpression* exprRight = node.getRightExpr();

        // visit left
        resolve(*exprLeft);
        {
            // visit right
            EvalMoveState newEvalState;
            newEvalState.enabled = true;
            newEvalState.scope = m_activeScope;
            EvalMoveStateScope evalGuard(m_evalMoveState, newEvalState);
            resolve(*exprRight);
        }

        // Todo better way to verify this???
        if (const ExprVarRef* dstExprVarRef = dyn_cast<ExprVarRef>(exprLeft);
                dstExprVarRef != nullptr) {
            MoveState& mvstate = findState(dstExprVarRef->getDeclVar());
            mvstate.moved = false;
            mvstate.stmtWhereMoved = nullptr;
        }
    }

    void
    AstVisitorMoveCheck::visit(const ExprVarRef& node) {
        const QualType qt = node.getQualType();
        if (!qt.isReference() && m_evalMoveState.enabled) {
            MoveState& state = findState(node.getDeclVar());
            if (state.moved) {
                // report error
                auto error = std::make_unique<ErrorMoveUseAfterMove>(node,
                                                                     *state.stmtWhereMoved);
                onError(std::move(error));
            } else {
                state.moved = true;
                state.stmtWhereMoved = &node;
            }
        }
    }

    void
    AstVisitorMoveCheck::visit(const ExprUnaryOperator& node) {
        // disable move tracking, unary operator doesn't move variables
        EvalMoveState newEvalState;
        newEvalState.enabled = false;
        newEvalState.scope = m_activeScope;
        EvalMoveStateScope evalGuard(m_evalMoveState, newEvalState);
        resolve(*node.getExpression());
    }

    void
    AstVisitorMoveCheck::visit(const ExprBinaryOperator& node) {
        // disable move tracking, binary operator doesn't move variables
        EvalMoveState newEvalState;
        newEvalState.enabled = false;
        newEvalState.scope = m_activeScope;
        EvalMoveStateScope evalGuard(m_evalMoveState, newEvalState);
        resolve(*node.getExpressionLeft());
        resolve(*node.getExpressionRight());
    }

    void
    AstVisitorMoveCheck::visit(const ExprBoolLiteral&) {}

    void
    AstVisitorMoveCheck::visit(const ExprIntegerLiteral&) {}

    void
    AstVisitorMoveCheck::visit(const ExprFloatLiteral&) {}

    void
    AstVisitorMoveCheck::visit(const ExprStructVarRef& node) {
        const QualType qt = node.getQualType();

        {
            // For nested struct var refs, we can only verify
            // the move of a member on the root expression,
            // all nested members shouldn't do the check.
            EvalMoveState newEvalState = m_evalMoveState;
            newEvalState.structVarRefRoot = false;
            EvalMoveStateScope evalGuard(m_evalMoveState, newEvalState);
            resolve(*node.getExpression());
        }
        // Check if we are in a movable situation and if we are
        // make sure that we are also in a replace context for struct
        // member
        if (m_evalMoveState.structVarRefRoot
                && m_evalMoveState.enabled
                && !qt.isTriviallyCopiable()
                && !qt.isReference()
                && !m_evalMoveState.structReplace) {
            auto error = std::make_unique<ErrorMoveStructVar>(node);
            onError(std::move(error));
        }
    }

    void
    AstVisitorMoveCheck::visit(const ExprFnCall& node) {
        EvalMoveState newEvalState;
        newEvalState.enabled = true;
        newEvalState.scope = m_activeScope;
        EvalMoveStateScope evalGuard(m_evalMoveState, newEvalState);
        const DeclFunction& decl = node.getFunctionType()->getDecl();
        const DeclFunction::Params& declParams = decl.getParams();
        auto paramIter = declParams.begin();
        for (auto& arg : node.getFunctionArgs()) {
            YAL_ASSERT(paramIter != declParams.end());
            const QualType qtParam = (*paramIter)->getQualType();
            const QualType qtArg = arg->getQualType();
            if (!qtParam.isReference()
                    && !qtParam.isTriviallyCopiable()
                    && qtArg.isRValue()) {
                uint32_t argIndex = uint32_t(std::distance(declParams.begin(), paramIter));
                auto error = std::make_unique<ErrorMoveFnCallRValue>(node, argIndex);
                onError(std::move(error));
            }
            resolve(*arg);
            ++paramIter;
        }
    }

    void
    AstVisitorMoveCheck::visit(const ExprTypeFnCall& node) {
        auto exprOpt = node.getExpression();
        if (exprOpt.has_value()) {
            StmtExpression* expr = exprOpt.value();
            resolve(*expr);
        }

        EvalMoveState newEvalState;
        newEvalState.enabled = true;
        newEvalState.scope = m_activeScope;
        EvalMoveStateScope evalGuard(m_evalMoveState, newEvalState);
        const DeclFunction& decl = node.getFunctionType()->getDecl();
        const DeclFunction::Params& declParams = decl.getParams();
        auto paramIter = declParams.begin();
        if (!node.isStaticCall()) {
            ++paramIter;
        }
        for (auto& arg : node.getFunctionArgs()) {
            YAL_ASSERT(paramIter != declParams.end());
            const QualType qtParam = (*paramIter)->getQualType();
            const QualType qtArg = arg->getQualType();
            if (!qtParam.isReference()
                    && !qtParam.isTriviallyCopiable()
                    && qtArg.isRValue()) {
                uint32_t argIndex = uint32_t(std::distance(declParams.begin(), paramIter));
                if (!node.isStaticCall()) {
                    argIndex -= 1;
                }
                auto error = std::make_unique<ErrorMoveFnCallRValue>(node, argIndex);
                onError(std::move(error));
            }

            resolve(*arg);
            ++paramIter;
        }
    }

    void
    AstVisitorMoveCheck::visit(const DeclParamVarSelf&) {
        YAL_ASSERT(false);
    }

    void
    AstVisitorMoveCheck::visit(const ExprVarRefSelf&) {

    }

    void
    AstVisitorMoveCheck::visit(const ExprCast&) {

    }

    void
    AstVisitorMoveCheck::visit(const ExprStructInit& node) {
        EvalMoveState newEvalState;
        newEvalState.enabled = true;
        newEvalState.scope = m_activeScope;
        EvalMoveStateScope evalGuard(m_evalMoveState, newEvalState);
        for (auto member: node.getMemberInitExprList()) {
            resolve(*member->getInitExpr());
        }
    }

    void
    AstVisitorMoveCheck::visit(const StmtListScoped& node) {
        DeclScopeGuard guard(*this, node.getListScope());
        for (auto& stmt : node.getStatements()) {
            resolve(*stmt);
        }
    }

    void
    AstVisitorMoveCheck::onError(std::unique_ptr<Error>&& error) {
        const bool isFatal = error->isFatal();
        m_errReporter.report(std::move(error));
        if (isFatal) {
            m_jump.trigger();
        }
    }

    AstVisitorMoveCheck::MoveState&
    AstVisitorMoveCheck::findState(const DeclBase& node) {
        auto it = m_moveMap.find(&node);
        YAL_ASSERT_MESSAGE(it != m_moveMap.end(),
                           "We should have all decls available during this stage");
        return it->second;
    }

}
