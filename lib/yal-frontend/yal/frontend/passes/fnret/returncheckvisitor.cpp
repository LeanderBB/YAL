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

#include "yal/frontend/passes/fnret/returncheckvisitor.h"

#include "yal/error/errorreporter.h"
#include "yal/frontend/ast/astvisitorimpl.h"
#include "yal/frontend/module.h"
#include "yal/frontend/passes/fnret/errorspassfnret.h"
#include "yal/frontend/passes/passes.h"

namespace yal::frontend {

    ReturnCheckVisitor::ReturnCheckVisitor(PassOptions& options):
        m_module(options.module),
        m_errReporter(options.errReporter),
        m_jump(),
        m_hasReturnStmt(false) {

    }

    void
    ReturnCheckVisitor::execute() {
        DeclModule* modDecl = m_module.getDeclNode();
        if (YAL_STACKJUMP_MARK(m_jump)) {
           return;
        }
        resolve(*modDecl);
    }

    void
    ReturnCheckVisitor::visit(const DeclModule& node) {
        for (auto& decl : node.getDeclarations()) {
            resolve(*decl);
        }
    }

    void
    ReturnCheckVisitor::visit(const DeclFunction& node) {
        const DeclFunction::Body& body = node.getFunctionBody();
        if (!body.empty() && node.getReturnType().isValid()) {
            for (auto& stmt : body) {
                m_hasReturnStmt = false;
                resolve(*stmt);
            }
        }

        if (!m_hasReturnStmt && node.getReturnType().isValid()) {
            auto error = std::make_unique<ErrorFnNotAllStmtReturn>(node);
            m_errReporter.report(std::move(error));
            m_jump.trigger();
        }
    }

    void
    ReturnCheckVisitor::visit(const DeclTypeFunction& node) {
        const DeclFunction::Body& body = node.getFunctionBody();
        if (!body.empty() && node.getReturnType().isValid()) {
            for (auto& stmt : body) {
                m_hasReturnStmt = false;
                resolve(*stmt);
            }
        }

        if (!m_hasReturnStmt && node.getReturnType().isValid()) {
            auto error = std::make_unique<ErrorFnNotAllStmtReturn>(node);
            m_errReporter.report(std::move(error));
            m_jump.trigger();
        }
    }

    void
    ReturnCheckVisitor::visit(const DeclTypeFunctions& node) {
        for (auto& decl : node.getDecls()) {
            resolve(*decl);
        }
    }

    void
    ReturnCheckVisitor::visit(const DeclStruct&) {}

    void
    ReturnCheckVisitor::visit(const DeclVar&) {}

    void
    ReturnCheckVisitor::visit(const DeclParamVar&) {}

    void
    ReturnCheckVisitor::visit(const DeclAliasStrong&) {}

    void
    ReturnCheckVisitor::visit(const DeclAliasWeak&) {}

    void
    ReturnCheckVisitor::visit(const StmtReturn&) {
        // TODO: revise when conditional statements and loops are introduced!
        m_hasReturnStmt = true;
    }

    void
    ReturnCheckVisitor::visit(const StmtDecl&) {}

    void
    ReturnCheckVisitor::visit(const StmtAssign&) {}

    void
    ReturnCheckVisitor::visit(const ExprVarRef&) {}

    void
    ReturnCheckVisitor::visit(const ExprUnaryOperator&) {}

    void
    ReturnCheckVisitor::visit(const ExprBinaryOperator&) {}

    void
    ReturnCheckVisitor::visit(const ExprBoolLiteral&) {}

    void
    ReturnCheckVisitor::visit(const ExprIntegerLiteral&) {}

    void
    ReturnCheckVisitor::visit(const ExprFloatLiteral&) {}

    void
    ReturnCheckVisitor::visit(const ExprStructVarRef&) {}

    void
    ReturnCheckVisitor::visit(const ExprFnCall&) {}

    void
    ReturnCheckVisitor::visit(const ExprTypeFnCall&) {}

    void
    ReturnCheckVisitor::visit(const DeclParamVarSelf&) {}

    void
    ReturnCheckVisitor::visit(const ExprVarRefSelf&) {}

    void
    ReturnCheckVisitor::visit(const ExprCast&) {}

    void
    ReturnCheckVisitor::visit(const ExprStructInit&) {}

    void
    ReturnCheckVisitor::visit(const StmtListScoped& node) {
        for (auto& stmt : node.getStatements()) {
            m_hasReturnStmt = false;
            resolve(*stmt);
        }
    }

    void
    ReturnCheckVisitor::fnReturnCheck(const DeclFunction& decl) {
        const QualType returnType = decl.getReturnType();
        if (returnType.isValid() && decl.getFunctionBody().empty()) {

        }
    }


}
