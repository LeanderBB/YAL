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

#include "yal/frontend/ast/astvisitor.h"
#include "yal/util/stackjump.h"

namespace yal {
     class ErrorReporter;
}

namespace yal::frontend {

    struct PassOptions;
    class DeclFunction;
    class Module;
    class ReturnCheckVisitor final : public AstVisitorRecursive<ReturnCheckVisitor, true>{
    public:
        ReturnCheckVisitor(PassOptions& options);

        void execute();

#define YAL_AST_NODE_TYPE(TYPE) void visit(const TYPE&);
#include "yal/frontend/ast/astnodes.def"
#undef YAL_AST_NODE_TYPE

    private:

        void fnReturnCheck(const DeclFunction& decl);

    protected:
        Module& m_module;
        ErrorReporter& m_errReporter;
        StackJump m_jump;
        bool m_hasReturnStmt;
    };

}
