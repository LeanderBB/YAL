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

#include "yal/frontend/parser/syntaxtreevisitor.h"

namespace yal {
    class ErrorReporter;
}

namespace yal::frontend {

    class STDeclFunction;
    class STDeclStruct;
    class STDeclModule;
    class Type;
    class STType;
    class Module;

    class STPreDeclVisitor final : public SyntaxTreeVisitorRecursive<STPreDeclVisitor, true>
    {
    public:
        STPreDeclVisitor(ErrorReporter& errReporter,
                         Module& module);

#define YAL_ST_NODE_TYPE(type) void visit(const type&);
#include "yal/frontend/parser/syntaxtreetypes.def"
#undef YAL_ST_NODE_TYPE

    private:
        class ScopeGuard;
        struct ScopeState {
            Type* declimplTarget = nullptr;
        };

        Type* resolveType(const STType& type);

    private:
        ErrorReporter& m_errReporter;
        Module& m_module;
        ScopeState m_state;
    };

}
