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

namespace yal {

    class Compiler;
    class Module;
    class SourceItem;
    class Type;
    class Identifier;
    struct TokenInfo;
    class DeclScope;
    class DeclFunction;
    class DeclTypeFunction;
    class DeclStruct;
    class DeclBase;
    class DeclVar;
    class DeclParamVar;
    class DeclParamVarSelf;

    class StageDecls {
    public:
        StageDecls(Compiler& compiler,
                   SourceItem &sourceItem,
                   Module& module);

        bool execute();

        Type* resolveType(const TokenInfo& ti);

        void pushDeclScope(DeclScope* scope);

        void popDeclScope();

        bool onDecl(DeclFunction* decl);

        bool onDecl(DeclTypeFunction* decl);

        bool onDecl(DeclStruct* decl);

        bool onDecl(DeclVar* decl);

        bool onDecl(DeclParamVar* decl);

    private:

        void onDuplicateDecl(const DeclBase* oldDecl,
                             const DeclBase* newDecl);

    public:
        Compiler& m_compiler;
        SourceItem& m_sourceItem;
        Module& m_module;
        DeclScope* m_activeScope;
    };
}
