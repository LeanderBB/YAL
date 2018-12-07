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

#include "yal/yal.h"
#include "yal/frontend/ast/asttypes.h"
namespace yal::frontend {

    class DeclModule;
    class DeclBase;
    class Statement;

    template<typename Derived, bool Const>
    class AstVisitorRecursive {
    public:
        template<typename Node>
        using NodeType = typename std::conditional<Const, const Node, Node>::type;

        Derived& getDerived() {
            return static_cast<Derived&>(*this);
        }

        void resolve(NodeType<DeclBase>& node);

        void resolve(NodeType<Statement>& node);
    };
}

