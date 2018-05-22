/*
 *  Copyright 2018 by Leander Beernaert (leanderbb@gmail.com)
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
#include "yal/frontend/ast/astnodes.h"
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
        modDecl->acceptVisitor(*this);
    }

    void
    AstVisitorMoveCheck::visit(DeclModule& node) {
        DeclScopeGuard guard(*this, node.getModuleDeclScope());
        for (auto& decl : node.getDeclarations()) {
            decl->acceptVisitor(*this);
        }
    }

    void
    AstVisitorMoveCheck::visit(DeclFunction& node) {
        DeclScopeGuard guard(*this, node.getFunctionScope());
        DeclFunction::Body& body = node.getFunctionBody();
        for (auto& stmt : body) {
            stmt->acceptVisitor(*this);
        }
    }

    void
    AstVisitorMoveCheck::visit(DeclTypeFunction& node) {
        DeclScopeGuard guard(*this, node.getFunctionScope());
        DeclFunction::Body& body = node.getFunctionBody();
        for (auto& stmt : body) {
            stmt->acceptVisitor(*this);
        }
    }

    void
    AstVisitorMoveCheck::visit(DeclStruct& node) {
        DeclScopeGuard guard(*this, node.getStructDeclScope());
        DeclStruct::Members& members = node.getMembers();
        for (auto& member: members) {
            member->acceptVisitor(*this);
        }

    }

    void
    AstVisitorMoveCheck::visit(DeclVar& node) {
        auto exprOpt = node.getExpression();
        if (exprOpt.has_value()) {
            const QualType qtDecl = node.getQualType();
            if (!qtDecl.isReference()) {
                EvalMoveState newEvalState;
                newEvalState.enabled = true;
                newEvalState.scope = m_activeScope;
                EvalMoveStateScope evalGuard(m_evalMoveState, newEvalState);

                StmtExpression* expr = exprOpt.value();
                expr->acceptVisitor(*this);
            }
        }
        MoveState& state = findState(node);
        state.moved = false;
        state.stmtWhereMoved = nullptr;
    }

    void
    AstVisitorMoveCheck::visit(DeclParamVar&) {
        YAL_ASSERT(false);
    }

    void
    AstVisitorMoveCheck::visit(DeclStrongAlias&) {}

    void
    AstVisitorMoveCheck::visit(DeclWeakAlias&) {}

    void
    AstVisitorMoveCheck::visit(StmtReturn& node) {
        auto exprOpt = node.getExpression();
        if (exprOpt.has_value()) {

            EvalMoveState newEvalState;
            newEvalState.enabled = true;
            newEvalState.scope = m_activeScope;
            newEvalState.exit = true;
            EvalMoveStateScope evalGuard(m_evalMoveState, newEvalState);

            StmtExpression* expr = exprOpt.value();
            expr->acceptVisitor(*this);
        }
    }

    void
    AstVisitorMoveCheck::visit(StmtDecl& node) {
        node.getDecl()->acceptVisitor(*this);
    }

    void
    AstVisitorMoveCheck::visit(StmtAssign& node) {
        StmtExpression* exprLeft = node.getLeftExpr();
        StmtExpression* exprRight = node.getRightExpr();

        // visit left
        exprLeft->acceptVisitor(*this);
        {
            // visit right
            EvalMoveState newEvalState;
            newEvalState.enabled = true;
            newEvalState.scope = m_activeScope;
            EvalMoveStateScope evalGuard(m_evalMoveState, newEvalState);
            exprRight->acceptVisitor(*this);
        }

        // Todo better way to verify this???
        if (ExprVarRef* dstExprVarRef = dyn_cast<ExprVarRef>(exprLeft);
                dstExprVarRef != nullptr) {
            MoveState& mvstate = findState(dstExprVarRef->getDeclVar());
            mvstate.moved = false;
            mvstate.stmtWhereMoved = nullptr;
        }
    }

    void
    AstVisitorMoveCheck::visit(ExprVarRef& node) {
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
    AstVisitorMoveCheck::visit(ExprUnaryOperator& node) {
        // disable move tracking, unary operator doesn't move variables
        EvalMoveState newEvalState;
        newEvalState.enabled = false;
        newEvalState.scope = m_activeScope;
        EvalMoveStateScope evalGuard(m_evalMoveState, newEvalState);
        node.getExpression()->acceptVisitor(*this);
    }

    void
    AstVisitorMoveCheck::visit(ExprBinaryOperator& node) {
        // disable move tracking, binary operator doesn't move variables
        EvalMoveState newEvalState;
        newEvalState.enabled = false;
        newEvalState.scope = m_activeScope;
        EvalMoveStateScope evalGuard(m_evalMoveState, newEvalState);
        node.getExpressionLeft()->acceptVisitor(*this);
        node.getExpressionRight()->acceptVisitor(*this);
    }

    void
    AstVisitorMoveCheck::visit(ExprBoolLiteral&) {}

    void
    AstVisitorMoveCheck::visit(ExprIntegerLiteral&) {}

    void
    AstVisitorMoveCheck::visit(ExprFloatLiteral&) {}

    void
    AstVisitorMoveCheck::visit(ExprStructVarRef& node) {
        const QualType qt = node.getQualType();

        node.getExpression()->acceptVisitor(*this);

        // Check if we are in a movable situation and if we are
        // make sure that we are also in a replace context for struct
        // member
        if (!qt.isReference()
                && m_evalMoveState.enabled
                && !m_evalMoveState.structReplace) {
            auto error = std::make_unique<ErrorMoveStructVar>(node);
            onError(std::move(error));
        }
    }

    void
    AstVisitorMoveCheck::visit(ExprFnCall& node) {
        EvalMoveState newEvalState;
        newEvalState.enabled = true;
        newEvalState.scope = m_activeScope;
        EvalMoveStateScope evalGuard(m_evalMoveState, newEvalState);
        for (auto& arg : node.getFunctionArgs()) {
            arg->acceptVisitor(*this);
        }
    }

    void
    AstVisitorMoveCheck::visit(ExprTypeFnCall& node) {
        auto exprOpt = node.getExpression();
        if (exprOpt.has_value()) {
            StmtExpression* expr = exprOpt.value();
            expr->acceptVisitor(*this);
        }

        EvalMoveState newEvalState;
        newEvalState.enabled = true;
        newEvalState.scope = m_activeScope;
        EvalMoveStateScope evalGuard(m_evalMoveState, newEvalState);
        for (auto& arg : node.getFunctionArgs()) {
            arg->acceptVisitor(*this);
        }
    }

    void
    AstVisitorMoveCheck::visit(DeclParamVarSelf&) {
        YAL_ASSERT(false);
    }

    void
    AstVisitorMoveCheck::visit(ExprVarRefSelf&) {

    }

    void
    AstVisitorMoveCheck::visit(ExprRangeCast&) {

    }

    void
    AstVisitorMoveCheck::visit(ExprStructInit& node) {
        EvalMoveState newEvalState;
        newEvalState.enabled = true;
        newEvalState.scope = m_activeScope;
        EvalMoveStateScope evalGuard(m_evalMoveState, newEvalState);
        for (auto member: node.getMemberInitExprList()) {
            member->acceptVisitor(*this);
        }
    }

    void
    AstVisitorMoveCheck::visit(StructMemberInit& node) {
        node.getInitExpr()->acceptVisitor(*this);
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
