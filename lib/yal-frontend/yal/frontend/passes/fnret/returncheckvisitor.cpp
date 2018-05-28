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
#include "yal/frontend/ast/astnodes.h"
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
        m_jump.mark();
        modDecl->acceptVisitor(*this);
    }

    void
    ReturnCheckVisitor::visit(DeclModule& node) {
        for (auto& decl : node.getDeclarations()) {
            decl->acceptVisitor(*this);
        }
    }

    void
    ReturnCheckVisitor::visit(DeclFunction& node) {
        DeclFunction::Body& body = node.getFunctionBody();
        if (!body.empty() && node.getReturnType().isValid()) {
            for (auto& stmt : body) {
                m_hasReturnStmt = false;
                stmt->acceptVisitor(*this);
            }
        }

        if (!m_hasReturnStmt && node.getReturnType().isValid()) {
            auto error = std::make_unique<ErrorFnNotAllStmtReturn>(node);
            m_errReporter.report(std::move(error));
            m_jump.trigger();
        }
    }

    void
    ReturnCheckVisitor::visit(DeclTypeFunction& node) {
        DeclFunction::Body& body = node.getFunctionBody();
        if (!body.empty() && node.getReturnType().isValid()) {
            for (auto& stmt : body) {
                m_hasReturnStmt = false;
                stmt->acceptVisitor(*this);
            }
        }

        if (!m_hasReturnStmt && node.getReturnType().isValid()) {
            auto error = std::make_unique<ErrorFnNotAllStmtReturn>(node);
            m_errReporter.report(std::move(error));
            m_jump.trigger();
        }
    }

    void
    ReturnCheckVisitor::visit(DeclStruct&) {}

    void
    ReturnCheckVisitor::visit(DeclVar&) {}

    void
    ReturnCheckVisitor::visit(DeclParamVar&) {}

    void
    ReturnCheckVisitor::visit(DeclStrongAlias&) {}

    void
    ReturnCheckVisitor::visit(DeclWeakAlias&) {}

    void
    ReturnCheckVisitor::visit(StmtReturn&) {
      // TODO: revise when conditional statements and loops are introduced!
       m_hasReturnStmt = true;
    }

    void
    ReturnCheckVisitor::visit(StmtDecl&) {}

    void
    ReturnCheckVisitor::visit(StmtAssign&) {}

    void
    ReturnCheckVisitor::visit(ExprVarRef&) {}

    void
    ReturnCheckVisitor::visit(ExprUnaryOperator&) {}

    void
    ReturnCheckVisitor::visit(ExprBinaryOperator&) {}

    void
    ReturnCheckVisitor::visit(ExprBoolLiteral&) {}

    void
    ReturnCheckVisitor::visit(ExprIntegerLiteral&) {}

    void
    ReturnCheckVisitor::visit(ExprFloatLiteral&) {}

    void
    ReturnCheckVisitor::visit(ExprStructVarRef&) {}

    void
    ReturnCheckVisitor::visit(ExprFnCall&) {}

    void
    ReturnCheckVisitor::visit(ExprTypeFnCall&) {}

    void
    ReturnCheckVisitor::visit(DeclParamVarSelf&) {}

    void
    ReturnCheckVisitor::visit(ExprVarRefSelf&) {}

    void
    ReturnCheckVisitor::visit(ExprRangeCast&) {}

    void
    ReturnCheckVisitor::visit(ExprStructInit&) {}

    void
    ReturnCheckVisitor::visit(StructMemberInit&) {}

    void
    ReturnCheckVisitor::visit(StmtListScoped& node) {
        for (auto& stmt : node.getStatements()) {
            m_hasReturnStmt = false;
            stmt->acceptVisitor(*this);
        }
    }

    void
    ReturnCheckVisitor::fnReturnCheck(DeclFunction& decl) {
        const QualType returnType = decl.getReturnType();
        if (returnType.isValid() && decl.getFunctionBody().empty()) {

        }
    }


}
