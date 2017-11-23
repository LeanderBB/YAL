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

#include "yal/ast/astprinter.h"
#include "yal/io/bytestream.h"
#include "yal/ast/declfunction.h"
#include "yal/ast/decltypefunction.h"
#include "yal/ast/declparamvar.h"
#include "yal/ast/exprbinaryoperator.h"
#include "yal/ast/exprboolliteral.h"
#include "yal/ast/exprunaryoperator.h"
#include "yal/ast/exprintegerliteral.h"
#include "yal/ast/exprvarref.h"
#include "yal/ast/statementlist.h"
#include "yal/ast/stmtreturn.h"
#include "yal/ast/stmtdecl.h"
#include "yal/ast/reftypebuiltin.h"
#include "yal/ast/reftypeidentifier.h"
#include "yal/ast/stmtassign.h"
#include "yal/ast/declmodule.h"

namespace yal {

    AstPrinter::AstPrinter(ByteStream& stream):
        m_stream(stream),
        m_formater() {

    }

    void
    AstPrinter::visit(DeclFunction& node) {
        print("DeclFunction %\n", node.getName());

        if (node.getParams() != nullptr) {
            DeclParamVarContainer* params = node.getParams();
            params->acceptVisitor(*this);
        }

        if (node.getFunctionBody() != nullptr) {
            StatementList* stmts = node.getFunctionBody();
            stmts->acceptVisitor(*this);
        }
    }

    void
    AstPrinter::visit(DeclTypeFunction& node) {
        print("DeclTypeFunction %\n", node.getName());

        if (node.getParams() != nullptr) {
            DeclParamVarContainer* params = node.getParams();
            params->acceptVisitor(*this);
        }

        if (node.getFunctionBody() != nullptr) {
            StatementList* stmts = node.getFunctionBody();
            stmts->acceptVisitor(*this);
        }
    }

    void
    AstPrinter::visit(DeclStruct&) {

    }

    void
    AstPrinter::visit(DeclVar& node) {
        print("DeclVar %\n", node.getName());
    }

    void
    AstPrinter::visit(DeclModule& node) {
        print("DeclModule %\n", node.getName());

        for (auto& v : node.getDeclarations()) {
            v->acceptVisitor(*this);
        }
    }

    void
    AstPrinter::visit(DeclParamVar& node) {
        print("DeclParamVar %\n", node.getName());
    }

    void
    AstPrinter::visit(DeclParamVarContainer& node) {
        print("DeclParamVarContainer\n");
        for (auto it = node.childBegin(); it != node.childEnd(); ++it) {
            (*it)->acceptVisitor(*this);
        }
    }

    void
    AstPrinter::visit(DeclStrongAlias&) {

    }

    void
    AstPrinter::visit(DeclWeakAlias&) {

    }

    void
    AstPrinter::visit(RefTypeBuiltin& node) {
        print("RefTypeBuiltin %\n", node.getType()->getName());
    }

    void
    AstPrinter::visit(RefTypeIdentifier& node) {
        print("RefTypeBuiltin %\n", node.getIdentifier());
    }

    void
    AstPrinter::visit(StatementList& node) {
        print("StatementList\n");
        for (auto it = node.childBegin(); it != node.childEnd(); ++it) {
            (*it)->acceptVisitor(*this);
        }
    }

    void
    AstPrinter::visit(StmtReturn& node) {
        print("StmtReturn\n");
        if (node.hasReturnExpression()) {
            node.getExpression()->acceptVisitor(*this);
        }
    }

    void
    AstPrinter::visit(StmtDecl& node) {
        print("StmtDecl\n");
        node.getDecl()->acceptVisitor(*this);
    }

    void
    AstPrinter::visit(StmtAssign& node) {
        print("StmtAssign\n");
        node.getDestExpr()->acceptVisitor(*this);
        node.getValueExpr()->acceptVisitor(*this);
    }

    void
    AstPrinter::visit(ExprVarRef& node) {
         print("ExprVarRef %\n", node.getVariableName());
    }

    void
    AstPrinter::visit(ExprUnaryOperator& node) {
        print("ExpUnaryOperator\n");
        node.getExpression()->acceptVisitor(*this);
    }

    void
    AstPrinter::visit(ExprBinaryOperator& node) {
        print("ExpBinaryOperator\n");
        node.getExpressionLeft()->acceptVisitor(*this);
        node.getExpressionRight()->acceptVisitor(*this);
    }

    void
    AstPrinter::visit(ExprBoolLiteral& node) {
        print("ExprBoolLiteral %\n", node.getLiteralValue());
    }

    void
    AstPrinter::visit(ExprIntegerLiteral& node) {
        (void) node;
        print("ExprIntegerLiteral\n");
    }

    void
    AstPrinter::visit(ExprDecimalLiteral&) {

    }

    void
    AstPrinter::printToStream() {
        m_stream.write(m_formater.buffer, m_formater.bufferPos);
    }
}
