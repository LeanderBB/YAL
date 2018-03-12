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

#include "yal/compiler/stages/stagefnreturn.h"
#include "yal/compiler/compiler.h"
#include "yal/ast/astnodes.h"
#include "yal/util/prettyprint.h"
#include "yal/util/log.h"
#include "yal/util/stackjump.h"
namespace yal {


    class ReturnCheckVisitor final : public RecursiveAstVisitor {

    public:

        ReturnCheckVisitor(Compiler& compiler,
                           StackJump& jump):
            m_activeFunction(nullptr),
            m_compiler(compiler),
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

        void checkReturnValue(DeclFunctionBase &function);

        void onError();

    public:
        DeclFunctionBase* m_activeFunction;
        Compiler& m_compiler;
        StackJump& m_stackJump;
        bool m_error;
    };

    void
    ReturnCheckVisitor::checkReturnValue(DeclFunctionBase& functionDecl) {
        StatementList* body = functionDecl.getFunctionBody();
        // check for empty body
        if (body == nullptr && functionDecl.hasReturnValue()) {
            Log& log = m_compiler.getLog();
            PrettyPrint::SourceErrorPrint(log,
                                          functionDecl.getSourceInfo(),
                                          m_compiler.getSourceManager());
            log.error("Function '%' expects a return value, but has an empty body.\n",
                      functionDecl.getIdentifier().getAsString());
            onError();
        }
    }

    void
    ReturnCheckVisitor::onError() {
        m_error = true;
        m_stackJump.trigger();
    }

    void
    ReturnCheckVisitor::visit(DeclFunction& node) {
        checkReturnValue(node);
        m_activeFunction = &node;
        if (node.getFunctionBody() != nullptr) {
            node.getFunctionBody()->acceptVisitor(*this);
        }
        m_activeFunction = nullptr;
    }

    void
    ReturnCheckVisitor::visit(DeclTypeFunction& node) {
        checkReturnValue(node);
        m_activeFunction = &node;
        if (node.getFunctionBody() != nullptr) {
            node.getFunctionBody()->acceptVisitor(*this);
        }
        m_activeFunction = nullptr;
    }

    void
    ReturnCheckVisitor::visit(DeclStruct&) {
    }

    void
    ReturnCheckVisitor::visit(DeclVar&) {
    }

    void
    ReturnCheckVisitor::visit(DeclParamVar&) {
    }

    void
    ReturnCheckVisitor::visit(DeclStrongAlias&) {

    }

    void
    ReturnCheckVisitor::visit(DeclWeakAlias&) {

    }

    void
    ReturnCheckVisitor::visit(RefType& ) {
    }

    void
    ReturnCheckVisitor::visit(StmtReturn& node) {
        DeclFunctionBase* functionDecl = m_activeFunction;

        StmtExpression* expr = node.getExpression();
        // check if we have a return type
        if (functionDecl->hasReturnValue() && expr == nullptr) {
            Log& log = m_compiler.getLog();
            PrettyPrint::SourceErrorPrint(log,
                                          node.getSourceInfo(),
                                          m_compiler.getSourceManager());
            log.error("Function '%' has return type '%', can't use empty return statement.\n",
                      functionDecl->getIdentifier().getAsString(),
                      functionDecl->getReturnType()->getIdentitfier().getAsString());
            onError();
        }

        if (!functionDecl->hasReturnValue() && expr != nullptr) {
            Log& log = m_compiler.getLog();
            PrettyPrint::SourceErrorPrint(log,
                                          node.getSourceInfo(),
                                          m_compiler.getSourceManager());
            log.error("Function '%' has no return value, expecting empty return statement.\n",
                      functionDecl->getIdentifier().getAsString());
            onError();
        }
    }

    void
    ReturnCheckVisitor::visit(StmtDecl& ) {

    }

    void
    ReturnCheckVisitor::visit(StmtAssign&) {
    }

    void
    ReturnCheckVisitor::visit(ExprVarRef&) {
    }

    void
    ReturnCheckVisitor::visit(ExprUnaryOperator&) {
    }

    void
    ReturnCheckVisitor::visit(ExprBinaryOperator&) {

    }

    void
    ReturnCheckVisitor::visit(ExprBoolLiteral&) {

    }

    void
    ReturnCheckVisitor::visit(ExprIntegerLiteral& ) {

    }

    void
    ReturnCheckVisitor::visit(ExprFloatLiteral&) {

    }

    void
    ReturnCheckVisitor::visit(ExprStructVarRef&) {
    }

    void
    ReturnCheckVisitor::visit(ExprFnCall&) {
    }

    void
    ReturnCheckVisitor::visit(ExprTypeFnCallStatic&) {
    }


    void
    ReturnCheckVisitor::visit(ExprTypeFnCall&) {
    }


    void
    ReturnCheckVisitor::visit(DeclParamVarSelf&) {

    }


    void
    ReturnCheckVisitor::visit(ExprVarRefSelf&) {
    }

    void
    ReturnCheckVisitor::visit(ExprRangeCast&) {

    }

    void
    ReturnCheckVisitor::visit(ExprStructInit&) {

    }

    void
    ReturnCheckVisitor::visit(StructMemberInit&) {

    }

    StageFnReturn::StageFnReturn(Compiler& compiler):
        m_compiler(compiler) {
    }

    bool
    StageFnReturn::execute(DeclBase* decl) {
        StackJump jmp;
        ReturnCheckVisitor visitor(m_compiler, jmp);
        jmp.mark();
        decl->acceptVisitor(visitor);
        return !visitor.didError();
    }

}
