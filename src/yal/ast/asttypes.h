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

#include "yal/util/cast.h"

namespace yal {

    enum class ASTType {
#define YAL_AST_NODE_TYPE(type) type,
#include "yal/ast/astnodes.def"
#undef YAL_AST_NODE_TYPE
        TypeInvalid,
    };

#define YAL_AST_NODE_TYPE(CLASS) \
    class CLASS; \
    template<> struct cast_typeid< CLASS >{typedef ASTType type;}; \
    template<> constexpr cast_typeid< CLASS >::type get_typeid< CLASS >() {return ASTType::CLASS ;}
#include "yal/ast/astnodes.def"
#undef YAL_AST_NODE_TYPE
}
