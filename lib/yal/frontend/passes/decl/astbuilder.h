/*
 *  Copyright 2018 by Leander Beernaert (leanderbb@gmail.com)
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
    class Error;
    class ErrorReporter;
}

namespace yal::frontend {

    class Module;
    class STType;
    class STDecl;
    class STIdentifier;
    class DeclBase;
    class Type;

    class AstBuilder final : public SyntaxTreeVisitor
    {
    public:

        AstBuilder(ErrorReporter& errReporter,
                   Module& module);

#define YAL_ST_NODE_TYPE(type) void visit(const type&) override final;
#include "yal/frontend/parser/syntaxtreetypes.def"
#undef YAL_ST_NODE_TYPE

        bool execute();

    private:
        struct State;
        State& getState();

        void onUndefinedType(const STType& type);
        void onUndefinedType(const STDecl& type);
        void onDuplicateSymbol(const STDecl& decl,
                               const DeclBase& existing);
        void onDuplicateSymbol(const STIdentifier& id,
                               const DeclBase& existing);
        void onUndefinedSymbol(const STIdentifier& id);
        void onSymbolNotDeclVar(const STIdentifier& id,
                                const DeclBase& decl);

        Type* resolveType(const STType& type);

    private:
        ErrorReporter& m_errReporter;
        Module& m_module;
        State* m_state;
    };


}
