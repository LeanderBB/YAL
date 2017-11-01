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
#include <yal/yal.h>
#include "yal/util/stringref.h"
namespace yal{

    enum class Token;
    class MemoryStream;
    class Lexer {
    public:

        struct TokenInfo
        {
            Token token;
            size_t tokLen =0;
            size_t lineStart = 0;
            size_t lineEnd = 0;
            size_t columnStart = 0;
            size_t columnEnd = 0;
            size_t tokenOffsetInStream = 0;
            StringRef tokenStr;
        };

        enum class Status
        {
            Ok,
            Error,
            EOS
        };

        Lexer(MemoryStream& stream);

        ~Lexer();

        Status scan();

        const TokenInfo& getLastToken() const {
            return m_curToken;
        }

        MemoryStream& getStream() const {
            return m_stream;
        }

    private:

        struct LexerState;

        static std::unique_ptr<LexerState> createLexerState();

        Status re2cExecute();

        void setToken(const Token token,
                      const LexerState& state);

        void setLexerError(const LexerState& state);

        MemoryStream& m_stream;
        std::unique_ptr<LexerState> m_lexerState;
        TokenInfo m_curToken;
    };
}
