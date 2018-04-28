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
#pragma once

#include "yal/frontend/ast/astvisitor.h"
#include "yal/frontend/ast/stmtexpression.h"

namespace yal::frontend {

    class AstExprLeafSearchBase: public RecursiveAstVisitor {

    public:
        AstExprLeafSearchBase(const AstType nodeType):
            m_nodeType(nodeType),
            m_searchNode(nullptr) {
        }


#define YAL_AST_SKIP_NODE_CONTAINERS
#define YAL_AST_NODE_TYPE(TYPE) virtual void visit(TYPE&) override final;
#include "yal/frontend/ast/astnodes.def"
#undef YAL_AST_NODE_TYPE
#undef YAL_AST_SKIP_NODE_CONTAINERS

    protected:
        const AstType m_nodeType;
        const void* m_searchNode;
    };

    template <typename T>
    class AstExprLeafSearch : protected AstExprLeafSearchBase {
    public:
        AstExprLeafSearch():
            AstExprLeafSearchBase(get_typeid<T>()) {
        }

        const T* search(StmtExpression* expr)  {
            expr->acceptVisitor(*this);
            return reinterpret_cast<const T*>(m_searchNode);
        }
    };

}
