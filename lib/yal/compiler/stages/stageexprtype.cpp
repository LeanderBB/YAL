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
#include "yal/ast/declmodule.h"
#include "yal/ast/declfunction.h"
#include "yal/ast/decltypefunction.h"
#include "yal/ast/declstruct.h"
#include "yal/ast/astvisitor.h"
#include "yal/ast/stmtassign.h"
#include "yal/ast/exprbinaryoperator.h"
#include "yal/ast/exprunaryoperator.h"
#include "yal/ast/exprvarref.h"
#include "yal/ast/declparamvar.h"
#include "yal/ast/exprfncall.h"
#include "yal/ast/exprstructfncall.h"
#include "yal/ast/exprstructvarref.h"
#include "yal/ast/stmtdecl.h"
#include "yal/ast/stmtreturn.h"
#include "yal/ast/statementlist.h"
#include "yal/ast/exprlist.h"
#include "yal/ast/reftype.h"
#include "yal/util/prettyprint.h"
#include "yal/util/log.h"
#include "yal/util/stackjump.h"
namespace yal {


    class ExprTypeAstVisitor : public RecursiveAstVisitor {

    public:

        ExprTypeAstVisitor(Compiler& compiler,
                           StackJump& jump):
            m_compiler(compiler),
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
            /*
        DeclFunctionBase* functionDecl
                = static_cast<DeclFunctionBase*>(m_activeScope->getScopeDecl());
            if(expr->getQualType().getType() != functionDecl->getReturnType()->getType()) {
                Log& log = m_compiler.getLog();
                PrettyPrint::SourceErrorPrint(log,
                                              node.getSourceInfo(),
                                              m_compiler.getSourceManager());
                log.error("Return statement has type '%', but function '%s' expects return type '%'.\n",
                          expr->getQualType().getType()->getIdentifier().getAsString(),
                          functionDecl->getIdentifier().getAsString(),
                          functionDecl->getReturnType()->getIdentitfier().getAsString());
                onError();
            }
            */
        }
    }

    void
    ExprTypeAstVisitor::visit(StmtDecl& node) {
        node.getDecl()->acceptVisitor(*this);
    }

    void
    ExprTypeAstVisitor::visit(StmtAssign& node) {
        node.getDestExpr()->acceptVisitor(*this);
        node.getValueExpr()->acceptVisitor(*this);
    }

    void
    ExprTypeAstVisitor::visit(ExprVarRef& node) {
        (void) node;
        YAL_ASSERT(node.getQualType().isValid());
    }

    void
    ExprTypeAstVisitor::visit(ExprUnaryOperator& node) {
        node.getExpression()->acceptVisitor(*this);
    }

    void
    ExprTypeAstVisitor::visit(ExprBinaryOperator& node) {
        node.getExpressionLeft()->acceptVisitor(*this);
        node.getExpressionRight()->acceptVisitor(*this);
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
        node.getExpression()->acceptVisitor(*this);
        // TODO: check if experession is a valid type
    }

    void
    ExprTypeAstVisitor::visit(ExprFnCall& node) {
        node.getFunctionArgs()->acceptVisitor(*this);
    }

    void
    ExprTypeAstVisitor::visit(ExprStructFnCall& node) {
        node.getExpression()->acceptVisitor(*this);
        // TODO: check if experession is a valid type
    }


    void
    ExprTypeAstVisitor::visit(DeclParamVarSelf&) {

    }


    void
    ExprTypeAstVisitor::visit(ExprVarRefSelf& node) {
        YAL_ASSERT(node.getQualType().getType() != nullptr);
    }

    StageExprType::StageExprType(Compiler& compiler):
        m_compiler(compiler) {
    }


    bool
    StageExprType::execute(DeclBase* decl) {
        StackJump jmp;
        ExprTypeAstVisitor visitor(m_compiler, jmp);
        jmp.mark();
        decl->acceptVisitor(visitor);
        return !visitor.didError();
    }




}
