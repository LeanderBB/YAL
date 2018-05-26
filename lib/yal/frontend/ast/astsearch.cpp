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

#include "yal/frontend/ast/astsearch.h"
#include "yal/frontend/ast/astnodes.h"

namespace yal::frontend {

    void
    AstExprLeafSearchBase::visit(DeclFunction&) {
        YAL_ASSERT(false);
    }

    void
    AstExprLeafSearchBase::visit(DeclTypeFunction&) {
        YAL_ASSERT(false);
    }

    void
    AstExprLeafSearchBase::visit(DeclStruct&) {
        YAL_ASSERT(false); // Maybe??
    }

    void
    AstExprLeafSearchBase::visit(DeclVar&) {
        YAL_ASSERT(false);
    }

    void
    AstExprLeafSearchBase::visit(DeclParamVar&) {
        YAL_ASSERT(false);
    }

    void
    AstExprLeafSearchBase::visit(DeclStrongAlias&) {
        YAL_ASSERT(false);
    }

    void
    AstExprLeafSearchBase::visit(DeclWeakAlias&) {
        YAL_ASSERT(false);
    }

    void
    AstExprLeafSearchBase::visit(StmtReturn&) {
        YAL_ASSERT(false);
    }

    void
    AstExprLeafSearchBase::visit(StmtDecl&) {
        YAL_ASSERT(false);
    }

    void
    AstExprLeafSearchBase::visit(StmtAssign&) {
        YAL_ASSERT(false);
    }

    void
    AstExprLeafSearchBase::visit(ExprVarRef& node) {
        if (node.getAstType() == m_nodeType) {
            m_searchNode = &node;
        } else {
            m_searchNode = nullptr;
        }
    }

    void
    AstExprLeafSearchBase::visit(ExprUnaryOperator& node) {
        node.getExpression()->acceptVisitor(*this);
        m_searchNode = nullptr;
    }

    void
    AstExprLeafSearchBase::visit(ExprBinaryOperator& node) {
        node.getExpressionLeft()->acceptVisitor(*this);
        node.getExpressionRight()->acceptVisitor(*this);
        m_searchNode = nullptr;
    }

    void
    AstExprLeafSearchBase::visit(ExprBoolLiteral& node) {
        if (node.getAstType() == m_nodeType) {
            m_searchNode = &node;
        } else {
            m_searchNode = nullptr;
        }
    }

    void
    AstExprLeafSearchBase::visit(ExprIntegerLiteral& node) {
        if (node.getAstType() == m_nodeType) {
            m_searchNode = &node;
        } else {
            m_searchNode = nullptr;
        }
    }

    void
    AstExprLeafSearchBase::visit(ExprFloatLiteral& node) {
        if (node.getAstType() == m_nodeType) {
            m_searchNode = &node;
        } else {
            m_searchNode = nullptr;
        }
    }

    void
    AstExprLeafSearchBase::visit(ExprStructVarRef& node) {
        if (node.getAstType() == m_nodeType) {
            m_searchNode = &node;
        } else {
            m_searchNode = nullptr;
        }
    }

    void
    AstExprLeafSearchBase::visit(ExprFnCall& node) {
        if (node.getAstType() == m_nodeType) {
            m_searchNode = &node;
            return;
        }

        if (!node.getFunctionArgs().empty()) {
            m_searchNode = nullptr;
            for (auto& arg : node.getFunctionArgs()) {
                arg->acceptVisitor(*this);
            }
        }
    }

    void
    AstExprLeafSearchBase::visit(ExprTypeFnCall& node) {
        if (node.getAstType() == m_nodeType) {
            m_searchNode = &node;
            return;
        }

        auto exprOpt = node.getExpression();
        if (exprOpt.has_value()) {
            m_searchNode = nullptr;
            StmtExpression* expr = exprOpt.value();
            expr->acceptVisitor(*this);
        }

        if (!node.getFunctionArgs().empty()) {
            m_searchNode = nullptr;
            for (auto& arg : node.getFunctionArgs()) {
                arg->acceptVisitor(*this);
            }
        }
    }

    void
    AstExprLeafSearchBase::visit(DeclParamVarSelf& node) {
        if (node.getAstType() == m_nodeType) {
            m_searchNode = &node;
        }
    }

    void
    AstExprLeafSearchBase::visit(ExprVarRefSelf& node) {
        if (node.getAstType() == m_nodeType) {
            m_searchNode = &node;
        }
    }

    void
    AstExprLeafSearchBase::visit(ExprRangeCast& node) {
        node.getExpression()->acceptVisitor(*this);
    }

    void
    AstExprLeafSearchBase::visit(ExprStructInit& node) {
        auto& memberInitList = node.getMemberInitExprList();
        if (!memberInitList.empty()) {
            for (auto& member: memberInitList) {
                member->acceptVisitor(*this);
            }
        }
    }

    void
    AstExprLeafSearchBase::visit(StructMemberInit& node) {
        m_searchNode = nullptr;
        node.getInitExpr()->acceptVisitor(*this);
    }
}
