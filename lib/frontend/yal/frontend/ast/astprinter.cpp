/*
 *  Copyright 2017 by Leander Beernaert (lbb-dev@pm.me)
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

#include "yal/frontend/ast/astprinter.h"
#include "yal/frontend/ast/astvisitorimpl.h"
#include "yal/frontend/types/qualtype.h"
#include "yal/frontend/types/typealias.h"
#include "yal/frontend/types/typefunction.h"
#include "yal/frontend/types/typestruct.h"
#include "yal/io/bytestream.h"


namespace yal::frontend {

    AstPrinter::AstPrinter(ByteStream& stream):
        m_writer(stream) {
    }

    void
    AstPrinter::visit(const DeclFunction& node) {
        m_writer.write("%",node.getAstTypeName());
        printSourceInfo(node.getSourceInfo());
        m_writer.write(" %\n", node.getName());

        const DeclFunction::Params& params = node.getParams();
        const DeclFunction::Body& body = node.getFunctionBody();
        if (!params.empty()) {
            scopeBegin(body.empty());
            for (const auto& param : params) {
                resolve(*param);
            }
            scopeEnd();
        }

        if (!body.empty()) {
            scopeBegin();
            for (const auto& stmt : body) {
                resolve(*stmt);
            }
            scopeEnd();
        }
    }

    void
    AstPrinter::visit(const DeclTypeFunction& node) {
        m_writer.write("%",node.getAstTypeName());
        printSourceInfo(node.getSourceInfo());
        m_writer.write(" % static:%\n",
                       node.getName(),
                       node.isStatic());

        const DeclFunction::Params& params = node.getParams();
        const DeclFunction::Body& body = node.getFunctionBody();
        if (!params.empty()) {
            scopeBegin(body.empty());
            for (const auto& param : params) {
                resolve(*param);
            }
            scopeEnd();
        }

        if (!body.empty()) {
            scopeBegin();
            for (const auto& stmt : body) {
                resolve(*stmt);
            }
            scopeEnd();
        }
    }

    void
    AstPrinter::visit(const DeclTypeFunctions& node) {
        m_writer.write("%",node.getAstTypeName());
        printSourceInfo(node.getSourceInfo());
        m_writer.write(" %\n", node.getType().getIdentifier());
        scopeBegin();
        for (auto& decl : node.getDecls()) {
            resolve(*decl);
        }
        scopeEnd();
    }

    void
    AstPrinter::visit(const DeclStruct& node) {
        m_writer.write("%",node.getAstTypeName());
        printSourceInfo(node.getSourceInfo());
        m_writer.write(" %\n", node.getName());

        const DeclStruct::Members& members = node.getMembers();

        if (!members.empty()) {
            scopeBegin();
            for (auto& member: members) {
                resolve(*member);
            }
            scopeEnd();
        }
    }

    void
    AstPrinter::visit(const DeclVar& node) {
        m_writer.write("%",node.getAstTypeName());
        printSourceInfo(node.getSourceInfo());
        m_writer.write(" %\n", node.getName());
        if (auto exprOpt = node.getExpression(); exprOpt.has_value()) {
            scopeBegin();
            resolve(*exprOpt.value());
            scopeEnd();
        }
    }


    void
    AstPrinter::visit(const DeclModule& node) {
        m_writer.write("%",node.getAstTypeName());
        printSourceInfo(node.getSourceInfo());
        m_writer.write(" %\n", node.getName());
        auto& decls = node.getDeclarations();
        for (auto it = decls.begin(); it != decls.end(); ++it) {
            auto nextIt = it;
            scopeBegin(++nextIt == decls.end());
            resolve(**it);
            scopeEnd();
        }
    }

    void
    AstPrinter::visit(const DeclImport& node) {
        m_writer.write("%",node.getAstTypeName());
        printSourceInfo(node.getSourceInfo());
        m_writer.write(" %\n", node.getModuleImported().getName());
    }

    void
    AstPrinter::visit(const DeclParamVar& node) {
        m_writer.write("%",node.getAstTypeName());
        printSourceInfo(node.getSourceInfo());
        m_writer.write(" %\n", node.getName());
    }

    void
    AstPrinter::visit(const DeclParamVarSelf& node) {
        m_writer.write("%",node.getAstTypeName());
        printSourceInfo(node.getSourceInfo());
        m_writer.write();
    }

    void
    AstPrinter::visit(const DeclAliasStrong&) {

    }

    void
    AstPrinter::visit(const DeclAliasWeak& node) {
        m_writer.write("%",node.getAstTypeName());
        printSourceInfo(node.getSourceInfo());
        m_writer.write(" % as %\n",
                       node.getIdentifier(),
                       node.getAliasType().getAliasedType().getIdentifier());
    }

    void
    AstPrinter::visit(const StmtReturn& node) {
        m_writer.write("%",node.getAstTypeName());
        printSourceInfo(node.getSourceInfo());
        m_writer.write();
        if (node.hasReturnExpression()) {
            const StmtExpression* expr = node.getExpression().value();
            scopeBegin();
            resolve(*expr);
            scopeEnd();
        }
    }

    void
    AstPrinter::visit(const StmtDecl& node) {
        m_writer.write("%",node.getAstTypeName());
        printSourceInfo(node.getSourceInfo());
        m_writer.write();
        scopeBegin();
        {
            resolve(*node.getDecl());
        }
        scopeEnd();
    }

    void
    AstPrinter::visit(const StmtAssign& node) {
        m_writer.write("%",node.getAstTypeName());
        printSourceInfo(node.getSourceInfo());
        m_writer.write();
        scopeBegin(false);
        {
            resolve(*node.getLeftExpr());
        }
        scopeEnd();
        scopeBegin();
        {
            resolve(*node.getRightExpr());
        }
        scopeEnd();
    }

    void
    AstPrinter::visit(const ExprVarRef& node) {
        m_writer.write("%",node.getAstTypeName());
        printSourceInfo(node.getSourceInfo());
        m_writer.write(" %\n", node.getDeclVar().getName());
    }

    void
    AstPrinter::visit(const ExprVarRefSelf& node) {
        m_writer.write("%",node.getAstTypeName());
        printSourceInfo(node.getSourceInfo());
        m_writer.write();
    }

    void
    AstPrinter::visit(const ExprStructVarRef& node) {
        m_writer.write("%",node.getAstTypeName());
        printSourceInfo(node.getSourceInfo());
        m_writer.write(" %\n", node.getMemberName());
        scopeBegin();
        {
            resolve(*node.getExpression());
        }
        scopeEnd();
    }

    void
    AstPrinter::visit(const ExprUnaryOperator& node) {
        m_writer.write("%",node.getAstTypeName());
        printSourceInfo(node.getSourceInfo());
        m_writer.write();
        scopeBegin();
        {
            resolve(*node.getExpression());
        }
        scopeEnd();
    }

    void
    AstPrinter::visit(const ExprBinaryOperator& node) {
        m_writer.write("%",node.getAstTypeName());
        printSourceInfo(node.getSourceInfo());
        m_writer.write();
        {
            scopeBegin(false);
            resolve(*node.getExpressionLeft());
            scopeEnd();
        }
        {
            scopeBegin();
            resolve(*node.getExpressionRight());
            scopeEnd();
        }
    }

    void
    AstPrinter::visit(const ExprBoolLiteral& node) {
        m_writer.write("%",node.getAstTypeName());
        printSourceInfo(node.getSourceInfo());
        m_writer.write(" %\n", node.getLiteralValue());
    }

    void
    AstPrinter::visit(const ExprIntegerLiteral& node) {
        m_writer.write("%",node.getAstTypeName());
        printSourceInfo(node.getSourceInfo());
        m_writer.write(" %\n", node.getLiteralValue());
    }

    void
    AstPrinter::visit(const ExprFloatLiteral& node) {
        m_writer.write("%",node.getAstTypeName());
        printSourceInfo(node.getSourceInfo());
        m_writer.write(" %\n", node.getLiteralValueAsF64());
    }

    void
    AstPrinter::visit(const ExprFnCall& node) {
        m_writer.write("%",node.getAstTypeName());
        printSourceInfo(node.getSourceInfo());
        m_writer.write(" %\n", node.getFunctionType()->getDecl().getName());

        const ExprList& args = node.getFunctionArgs();
        if (!args.empty()) {
            scopeBegin();
            for (auto& expr : args) {
                resolve(*expr);
            }
            scopeEnd();
        }
    }

    void
    AstPrinter::visit(const ExprTypeFnCall& node) {
        m_writer.write("%",node.getAstTypeName());
        printSourceInfo(node.getSourceInfo());
        m_writer.write(" %\n", node.getFunctionType()->getDecl().getName());
        if (!node.isStaticCall()) {
            scopeBegin(false);
            resolve(*node.getExpression().value());
            scopeEnd();
        }

        const ExprList& args = node.getFunctionArgs();
        if (!args.empty()) {
            scopeBegin();
            for (auto& expr : args) {
                resolve(*expr);
            }
            scopeEnd();
        }
    }

    void
    AstPrinter::visit(const ExprCast& node) {
        m_writer.write("%",node.getAstTypeName());
        printSourceInfo(node.getSourceInfo());
        m_writer.write(" %\n", node.getQualType());
        scopeBegin();
        resolve(node.getExpression());
        scopeEnd();
    }

    void
    AstPrinter::visit(const ExprStructInit& node) {
        m_writer.write("%",node.getAstTypeName());
        printSourceInfo(node.getSourceInfo());
        m_writer.write(" %\n", node.getQualType().getType()->getIdentifier());

        scopeBegin();
        for (auto& initExpr : node.getMemberInitExprList()) {
            m_writer.write("Member Init ");
            printSourceInfo(initExpr->getSourceInfo());
            m_writer.write(" %\n", initExpr->getMemberName());
            scopeBegin();
            resolve(*initExpr->getInitExpr());
            scopeEnd();
        }
        scopeEnd();
    }

    void
    AstPrinter::visit(const StmtListScoped& node) {
        scopeBegin();
        for (auto& stmt : node.getStatements()) {
            resolve(*stmt);
        }
        scopeEnd();
    }

    void
    AstPrinter::scopeBegin(const bool lastNode)
    {
        (void) lastNode;
        m_writer.ident();
        m_writer.write("`-");
    }

    void
    AstPrinter::scopeEnd() {
        m_writer.unident();
    }

    void
    AstPrinter::printQualifier(const Qualifier& qualifier){
        if (qualifier.isMutable()) {
            m_writer.write(" mut");
        }
        if (qualifier.isReference()) {
            m_writer.write(" &");
        }
    }

    void
    AstPrinter::printSourceInfo(const SourceInfo& info) {
        m_writer.write("<l:% c:% - l:% c:%>",
                       info.begin.line,
                       info.begin.column,
                       info.end.line,
                       info.end.column);
    }
}
