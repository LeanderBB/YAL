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
        m_identationChars.reserve(32);
    }

    void
    AstPrinter::visit(DeclFunction& node) {
        print("DeclFunction %\n", node.getName());

        bool hasStatmentList = node.getFunctionBody() != nullptr;

        if (node.getParams() != nullptr) {
            scopeBegin(!hasStatmentList);
            DeclParamVarContainer* params = node.getParams();
            params->acceptVisitor(*this);
            scopeEnd();
        }

        if (hasStatmentList) {
            scopeBegin();
            StatementList* stmts = node.getFunctionBody();
            stmts->acceptVisitor(*this);
            scopeEnd();
        }
    }

    void
    AstPrinter::visit(DeclTypeFunction& node) {
        print("DeclTypeFunction %\n", node.getName());

         bool hasStatmentList = node.getFunctionBody() != nullptr;

        if (node.getParams() != nullptr) {
            scopeBegin(false);
            DeclParamVarContainer* params = node.getParams();
            params->acceptVisitor(*this);
            scopeEnd();
        }

        if (hasStatmentList) {
            scopeBegin();
            StatementList* stmts = node.getFunctionBody();
            stmts->acceptVisitor(*this);
            scopeEnd();
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
        auto& decls = node.getDeclarations();
        for (auto it = decls.begin(); it != decls.end(); ++it) {
            scopeBegin(it + 1 == decls.end());
            (*it)->acceptVisitor(*this);
            scopeEnd();
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
            scopeBegin(it + 1 == node.childEnd());
            (*it)->acceptVisitor(*this);
            scopeEnd();
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
            scopeBegin(it + 1 == node.childEnd());
            (*it)->acceptVisitor(*this);
            scopeEnd();
        }

    }

    void
    AstPrinter::visit(StmtReturn& node) {
        print("StmtReturn\n");
        if (node.hasReturnExpression()) {
            scopeBegin();
            node.getExpression()->acceptVisitor(*this);
            scopeEnd();
        }
    }

    void
    AstPrinter::visit(StmtDecl& node) {
        print("StmtDecl\n");
        scopeBegin();
        {
            node.getDecl()->acceptVisitor(*this);
        }
        scopeEnd();
    }

    void
    AstPrinter::visit(StmtAssign& node) {
        print("StmtAssign\n");
        scopeBegin(false);
        {
            node.getDestExpr()->acceptVisitor(*this);
        }
        scopeEnd();
        scopeBegin();
        {
            node.getValueExpr()->acceptVisitor(*this);
        }
        scopeEnd();
    }

    void
    AstPrinter::visit(ExprVarRef& node) {
        print("ExprVarRef %\n", node.getVariableName());
    }

    void
    AstPrinter::visit(ExprUnaryOperator& node) {
        print("ExpUnaryOperator\n");
        scopeBegin();
        {
            node.getExpression()->acceptVisitor(*this);
        }
        scopeEnd();
    }

    void
    AstPrinter::visit(ExprBinaryOperator& node) {
        print("ExpBinaryOperator\n");
        {
            scopeBegin(false);
            node.getExpressionLeft()->acceptVisitor(*this);
            scopeEnd();
        }
        {
            scopeBegin();
            node.getExpressionRight()->acceptVisitor(*this);
            scopeEnd();
        }
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
        if (!m_identationChars.empty()) {
            m_stream.write(&m_identationChars[0], m_identationChars.size());
        }
        m_stream.write(m_formater.buffer, m_formater.bufferPos);
    }

    void
    AstPrinter::scopeBegin(const bool lastNode)
    {
        if (m_identationChars.size() > 1) {
            auto it = m_identationChars.rbegin();
            *it = ' ';
            ++it;
            if (*it == '`') {
                *it = ' ';
            }
        }
        m_identationChars.push_back(' ');
        m_identationChars.push_back(lastNode ? '`' : '|');
        m_identationChars.push_back('-');
    }

    void
    AstPrinter::scopeEnd() {
        YAL_ASSERT(!m_identationChars.empty());
        const size_t size = m_identationChars.size() - 3;
        m_identationChars.erase(m_identationChars.begin() + size,
                                m_identationChars.end());
    }
}
