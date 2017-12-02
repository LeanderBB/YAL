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
#include "yal/ast/reftype.h"
#include "yal/ast/stmtassign.h"
#include "yal/ast/declmodule.h"
#include "yal/ast/declstruct.h"
#include "yal/ast/exprstructvarref.h"
#include "yal/ast/exprstructfncall.h"
#include "yal/ast/exprlist.h"
#include "yal/ast/exprdecimalliteral.h"

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
        print("DeclTypeFunction %: static:%\n",
              node.getName(), node.isStatic());

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
    AstPrinter::visit(DeclStruct& node) {
        print("DeclStruct: %\n", node.getName());
        if (node.getMembers() != nullptr) {
            scopeBegin();
            node.getMembers()->acceptVisitor(*this);
            scopeEnd();
        }
    }

    void
    AstPrinter::visit(DeclStructMembers& node) {
        print("DeclStructMembers\n");
        for (auto it = node.childBegin(); it != node.childEnd(); ++it) {
            scopeBegin(it + 1 == node.childEnd());
            (*it)->acceptVisitor(*this);
            scopeEnd();
        }
    }

    void
    AstPrinter::visit(DeclVar& node) {
        print("DeclVar %\n", node.getName());
        if (node.getExpression() != nullptr) {
            scopeBegin();
            node.getExpression()->acceptVisitor(*this);
            scopeEnd();
        }
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
        scopeBegin();
        node.getVarType()->acceptVisitor(*this);
        scopeEnd();
    }

    void
    AstPrinter::visit(DeclParamVarSelf& node) {
        print("DeclParamVarSelf");
        printQualifier(node.getQualifier());
        print();

        scopeBegin();
        if (node.getVarType() != nullptr) {
            node.getVarType()->acceptVisitor(*this);
        } else {
            print("Self type not yet defined\n");
        }
        scopeEnd();
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
    AstPrinter::visit(ExprList& node) {
        print("ExprList\n");
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
    AstPrinter::visit(RefTypeResolved& node) {
        print("RefTypeResolved %", node.getResolvedType()->getName());
        printQualifier(node.getQualifier());
        print();
    }

    void
    AstPrinter::visit(RefTypeUnresolved& node) {
        print("RefTypeUnresolved %", node.getTypeName());
        printQualifier(node.getQualifier());
        print();
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
    AstPrinter::visit(ExprVarRefSelf&) {
        print("ExprVarRefSelf\n");
    }

    void
    AstPrinter::visit(ExprStructVarRef& node) {
        print("ExprStructVarRef %\n", node.getVariableName());
        scopeBegin();
        {
            node.getExpression()->acceptVisitor(*this);
        }
        scopeEnd();
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
        print("ExprDecimalLiteral\n");
    }

    void
    AstPrinter::visit(ExprFnCall& node) {
        print("ExprFnCall\n");
        const bool hasFunctionArgs = node.getFunctionArgs() != nullptr;
        scopeBegin(!hasFunctionArgs);
        node.getFunctionType()->acceptVisitor(*this);
        scopeEnd();
        if (hasFunctionArgs) {
            scopeBegin();
            node.getFunctionArgs()->acceptVisitor(*this);
            scopeEnd();
        }
    }

    void
    AstPrinter::visit(ExprStructFnCall& node) {
        print("ExprStructFnCall\n");
        scopeBegin();
        node.getExpression()->acceptVisitor(*this);
        scopeEnd();
        const bool hasFunctionArgs = node.getFunctionArgs() != nullptr;
        scopeBegin(!hasFunctionArgs);
        node.getFunctionType()->acceptVisitor(*this);
        scopeEnd();
        if (hasFunctionArgs) {
            scopeBegin();
            node.getFunctionArgs()->acceptVisitor(*this);
            scopeEnd();
        }
    }

    void
    AstPrinter::print() {
        const char newLine= '\n';
        m_stream.write(&newLine, 1);
    }

    void
    AstPrinter::printToStream() {
        if (!m_identationChars.empty()) {
            m_stream.write(&m_identationChars[0], m_identationChars.size());
        }
        m_stream.write(m_formater.buffer, m_formater.bufferPos);
    }

    void
    AstPrinter::printToStreamNoPrefix() {
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

    void
    AstPrinter::printQualifier(const Qualifier& qualifier){
        if (qualifier.isMutable()) {
            printOnLine(" mut");
        }
        if (qualifier.isReference()) {
            printOnLine(" &");
        }
    }
}
