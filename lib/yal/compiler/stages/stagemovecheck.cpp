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

    class MoveAstVisitor : public RecursiveAstVisitor {
    public:
        MoveAstVisitor(Compiler& compiler,
                       Module& module,
                       StackJump& jump):
            m_compiler(compiler),
            m_module(module),
            m_stackJump(jump),
            m_error(false) {
            m_activeScope = m_module.getDeclNode()->getDeclScope();
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
            Statement* stmtWhenMoved = nullptr;
        };

        void pushScope(DeclScope* scope);

        void popScope();

        void onError();

        MoveState& findState(const DeclBase* decl);

    private:
        using MoveMap = std::unordered_map<const DeclBase*, MoveState>;

        Compiler& m_compiler;
        Module& m_module;
        StackJump& m_stackJump;
        DeclScope* m_activeScope;
        std::unique_ptr<MoveMap> m_moveMap;
        bool m_error;
    };


    void
    MoveAstVisitor::pushScope(DeclScope* scope) {
        m_activeScope = scope;
        for (auto& declit : m_activeScope->getDecls()) {
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

        for (auto& declit : m_activeScope->getDecls()) {
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
        pushScope(node.getDeclScope());
        if (node.getFunctionBody() != nullptr) {
            node.getFunctionBody()->acceptVisitor(*this);
        }
        popScope();
    }

    void
    MoveAstVisitor::visit(DeclTypeFunction& node) {
        pushScope(node.getDeclScope());
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
        const QualType destType = node.getVarType()->getQualType();
        node.getExpression()->acceptVisitor(*this);
        const QualType exprType = node.getExpression()->getQualType();
        if (!CanMove(exprType,
                        node.getExpression()->getSourceInfo(),
                        destType,
                        node.getSourceInfo(),
                        m_compiler)) {
            onError();
        }

        MoveState& state = findState(&node);
        state.moved = false;
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
    }

    void
    MoveAstVisitor::visit(StmtDecl& node) {
        node.getDecl()->acceptVisitor(*this);
    }

    void
    MoveAstVisitor::visit(StmtAssign& node) {
        StmtExpression* destExpr = node.getDestExpr();
        StmtExpression* valueExpr = node.getValueExpr();
        destExpr->acceptVisitor(*this);
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

    }

    void
    MoveAstVisitor::visit(ExprVarRef& node) {
        const MoveState& state = findState(node.getDeclVar());

        if (state.moved) {
            Log& log = m_compiler.getLog();
            PrettyPrint::SourceErrorPrint(log,
                                          node.getSourceInfo(),
                                          m_compiler.getSourceManager());
            log.error("Can not use variable % as it has been moved and is no longer valid.\n",
                      node.getDeclVar()->getIdentifier());
            if (state.stmtWhenMoved != nullptr) {
                PrettyPrint::SourceErrorPrint(log,
                                              state.stmtWhenMoved->getSourceInfo(),
                                              m_compiler.getSourceManager());
                log.error("Variable % was moved here.\n",
                          node.getDeclVar()->getIdentifier());
            }
            onError();
        }
    }

    void
    MoveAstVisitor::visit(ExprUnaryOperator& node) {
        node.getExpression()->acceptVisitor(*this);
    }

    void
    MoveAstVisitor::visit(ExprBinaryOperator& node) {
        node.getExpressionLeft()->acceptVisitor(*this);
        node.getExpressionRight()->acceptVisitor(*this);
    }

    void
    MoveAstVisitor::visit(ExprBoolLiteral&) {

    }

    void
    MoveAstVisitor::visit(ExprIntegerLiteral& ) {

    }

    void
    MoveAstVisitor::visit(ExprDecimalLiteral&) {

    }

    void
    MoveAstVisitor::visit(ExprStructVarRef&) {

    }

    void
    MoveAstVisitor::visit(ExprFnCall& node) {
        if (node.getFunctionArgs() != nullptr) {
            node.getFunctionArgs()->acceptVisitor(*this);
        }
    }

    void
    MoveAstVisitor::visit(ExprTypeFnCall& node) {
        if (node.getFunctionArgs() != nullptr) {
            node.getFunctionArgs()->acceptVisitor(*this);
        }
    }

    void
    MoveAstVisitor::visit(ExprTypeFnCallStatic& node) {
        if (node.getFunctionArgs() != nullptr) {
            node.getFunctionArgs()->acceptVisitor(*this);
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
