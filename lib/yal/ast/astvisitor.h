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

#include "yal/ast/asttypes.h"

namespace yal {

    class DeclModule;
    class StatementList;
    class DeclParamVarContainer;
    class DeclStructMembers;
    class ExprList;
    class StructMemberInitList;
    class AstVisitor {
    public:

        virtual ~AstVisitor() {}

#define YAL_AST_NODE_TYPE(type) virtual void visit(type&) {}
#include "yal/ast/astnodes.def"
#undef YAL_AST_NODE_TYPE

    };

    class RecursiveAstVisitor : public AstVisitor {
    public:

        virtual void visit(DeclModule& node) override;

        virtual void visit(StatementList& node) override;

        virtual void visit(DeclParamVarContainer& node) override;

        virtual void visit(DeclStructMembers& node) override;

        virtual void visit(ExprList& node) override;

        virtual void visit(StructMemberInitList& node) override;

#define YAL_AST_SKIP_NODE_CONTAINERS
#define YAL_AST_NODE_TYPE(type) virtual void visit(type&) override{}
#include "yal/ast/astnodes.def"
#undef YAL_AST_NODE_TYPE
#undef YAL_AST_SKIP_NODE_CONTAINERS
    };
}

