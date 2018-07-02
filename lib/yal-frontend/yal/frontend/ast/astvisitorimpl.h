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

#pragma once

#include "yal/frontend/ast/astnodes.h"

namespace yal::frontend {

#define YAL_DELC_NODES \
    YAL_DECL_NODE_FN(DeclFunction) \
    YAL_DECL_NODE_FN(DeclTypeFunction) \
    YAL_DECL_NODE_FN(DeclTypeFunctions) \
    YAL_DECL_NODE_FN(DeclStruct) \
    YAL_DECL_NODE_FN(DeclVar) \
    YAL_DECL_NODE_FN(DeclParamVar) \
    YAL_DECL_NODE_FN(DeclParamVarSelf) \
    YAL_DECL_NODE_FN(DeclModule) \
    YAL_DECL_NODE_FN(DeclAliasWeak)
//    YAL_DECL_NODE_FN(DeclStrongAlias)
// YAL_DECL_NODE_FN(DeclWeakAlias)

#define YAL_STMT_NODES \
    YAL_STMT_NODE_FN(StmtReturn)\
    YAL_STMT_NODE_FN(StmtDecl)\
    YAL_STMT_NODE_FN(StmtAssign)\
    YAL_STMT_NODE_FN(ExprVarRef)\
    YAL_STMT_NODE_FN(ExprUnaryOperator)\
    YAL_STMT_NODE_FN(ExprBinaryOperator)\
    YAL_STMT_NODE_FN(ExprBoolLiteral)\
    YAL_STMT_NODE_FN(ExprIntegerLiteral)\
    YAL_STMT_NODE_FN(ExprFloatLiteral)\
    YAL_STMT_NODE_FN(ExprStructVarRef)\
    YAL_STMT_NODE_FN(ExprFnCall)\
    YAL_STMT_NODE_FN(ExprTypeFnCall)\
    YAL_STMT_NODE_FN(ExprVarRefSelf)\
    YAL_STMT_NODE_FN(ExprCast)\
    YAL_STMT_NODE_FN(ExprStructInit)\
    YAL_STMT_NODE_FN(StmtListScoped)\

    template<typename Derived, bool Const>
    void AstVisitorRecursive<Derived, Const>::resolve(NodeType<DeclBase>& node) {
        Derived& derived = getDerived();
        switch(node.getAstType()) {
#define YAL_DECL_NODE_FN(type) \
        case AstType::type:\
            derived.visit(static_cast<NodeType<type>&>(node));\
            break;
        YAL_DELC_NODES
#undef YAL_DECL_NODE_FN
        default:
            YAL_ASSERT_MESSAGE(false, "Unkown decl node type");
            break;
        }
    }

    template<typename Derived, bool Const>
    void AstVisitorRecursive<Derived, Const>::resolve(NodeType<Statement>& node) {
        Derived& derived = getDerived();
        switch(node.getAstType()) {
#define YAL_STMT_NODE_FN(type) \
        case AstType::type:\
            derived.visit(static_cast<NodeType<type>&>(node));\
            break;
        YAL_STMT_NODES
#undef YAL_STMT_NODE_FN
        default:
            YAL_ASSERT_MESSAGE(false, "Unkown stmt node type");
            break;
        }
    }

#undef YAL_DELC_NODES
#undef YAL_STMT_NODES
}
