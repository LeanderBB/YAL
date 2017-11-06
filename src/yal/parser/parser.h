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
#include <yal/ast/module.h>
namespace yal {
    class Log;
    class Lexer;
    class DeclModule;
#define YAL_AST_NODE_TYPE(N) class N;
#include "yal/ast/astnodes.def"
#undef YAL_AST_NODE_TYPE
    class StringRef;
    class Parser
    {
    public:

        enum class Result
        {
            Ok,
            SyntaxError,
            LexerError,
            TypeError,
        };


        Parser(Lexer& lexer,
               Log& log,
               Module& context);


        Result run();

        Log& getLog() {
            return m_log;
        }

        Module& getModule() {
            return m_module;
        }

        void logParseFailure();

        template <typename T, typename... ARGS>
        T* newASTNode(ARGS&& ...args) {
            auto newNode = new(m_module) T(m_module, std::forward<ARGS>(args)...);
            return newNode;
        }

    private:
        std::unique_ptr<void, void(*)(void*)> m_parserImpl;
        Lexer& m_lexer;
        Log& m_log;
        Module& m_module;
        Result m_status;
    };

}
