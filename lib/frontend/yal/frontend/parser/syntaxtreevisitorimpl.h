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
#include "yal/frontend/parser/syntaxtreenodes.h"

namespace yal::frontend {

    template<typename Derived, bool Const>
    void SyntaxTreeVisitorRecursive<Derived,Const>::resolve(NodeType<SyntaxTree>& node) {
        Derived& derived = getDerived();
        switch(node.getSyntaxTreeType()) {
#define YAL_ST_NODE_TYPE(type) \
        case SyntaxTreeType::type:\
            derived.visit(static_cast<NodeType<type>&>(node)); \
            break;
#include "yal/frontend/parser/syntaxtreetypes.def"
#undef YAL_ST_NODE_TYPE
        default:
            YAL_ASSERT_MESSAGE(false, "Unknown SyntaxTreeType");
            break;
        }
    }

}
