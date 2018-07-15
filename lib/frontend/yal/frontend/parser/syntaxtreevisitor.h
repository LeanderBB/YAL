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

#pragma once

#include <type_traits>

namespace yal::frontend {

#define YAL_ST_NODE_TYPE(type) class type;
#include "yal/frontend/parser/syntaxtreetypes.def"
#undef YAL_ST_NODE_TYPE

    class SyntaxTree;
    template<typename Derived, bool Const>
    class SyntaxTreeVisitorRecursive {
    public:
        template<typename Node>
        using NodeType = typename std::conditional<Const, const Node, Node>::type;

        Derived& getDerived() {
            return static_cast<Derived&>(*this);
        }

        void resolve(NodeType<SyntaxTree>& node);

    };
}
