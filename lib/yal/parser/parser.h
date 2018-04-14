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
#include <memory>
#include "yal/frontend/module.h"
#include "yal/frontend/lexer/tokens.h"
#include "yal/io/sourcemanager.h"
namespace yal {
    class Log;
    class Lexer;
    class DeclModule;
#define YAL_AST_NODE_TYPE(N) class N;
#include "yal/ast/astnodes.def"
#undef YAL_AST_NODE_TYPE
    class StringRef;
    class DeclScope;
    class StageDecls;
    class Parser
    {
    public:

        enum class Result
        {
            Ok,
            SyntaxError,
            LexerError,
            TypeError,
            DeclError
        };


        Parser(Lexer& lexer,
               Log& log,
               Module& context,
               StageDecls& stageDecls);


        Result run();

        Log& getLog() {
            return m_log;
        }

        Module& getModule() {
            return m_module;
        }

        void logParseFailure();

        void onDeclBegin(DeclFunction* decl);

        void onDeclBegin(DeclTypeFunction* decl);

        void onDeclBegin(DeclStruct* decl);

        bool onDecl(DeclVar* decl);

        bool onDecl(DeclParamVar* decl);

        void onDeclEnd();

        const Type* resolveType(const TokenInfo& ti);

        const Type* resolveSelfType() const;

        DeclVar* resolveVarRef(const TokenInfo& ti);

        DeclVar* resolveVarRefSelf(const TokenInfo& ti);

        const DeclScope* getActiveScope() const;

        template <typename T, typename... ARGS>
        T* newAstNode(ARGS&& ...args) {
            return m_module.newASTNode<T>(std::forward<ARGS>(args)...);
        }

        ExprIntegerLiteral* newIntegerLiteral(const TokenInfo& ti);

        ExprFloatLiteral* newFloatLiteral(const TokenInfo& ti);

        SourceInfo createSourceInfo(const TokenInfo& start,
                                    const TokenInfo& end) const;

        SourceInfo createSourceInfo(const TokenInfo& start,
                                    const SourceInfo& end) const;

        SourceInfo createSourceInfo(const SourceInfo& start,
                                    const TokenInfo& end) const;

        SourceInfo createSourceInfo(const SourceInfo& start,
                                    const SourceInfo& end) const;

    private:
        std::unique_ptr<void, void(*)(void*)> m_parserImpl;
        Lexer& m_lexer;
        Log& m_log;
        Module& m_module;
        StageDecls& m_stageDecls;
        Result m_status;
        DeclBase* m_activeDecl;
    };

}
