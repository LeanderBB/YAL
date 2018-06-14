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
#include "yal/frontend/lexer/tokens.h"
#include "yal/util/stringref.h"

namespace yal {

    enum class Token;
    class ByteStream;
    class StringPool;
}

namespace yal::frontend {
    class Lexer {
    public:

        enum {
            kYYMaxFill = 11
        };

        enum class Status
        {
            Ok,
            Error,
            EOS
        };

        Lexer(ByteStream& stream,
              StringPool& stringPool);

        ~Lexer();

        Status scan();

        const TokenInfo& getLastToken() const {
            return m_curToken;
        }

        ByteStream& getStream() const {
            return m_stream;
        }

    private:

        enum {
            kMaxBufferSize = 1024
        };

        struct LexerState {
            uint8_t buffer[kMaxBufferSize + kYYMaxFill];
            uint8_t* limit = nullptr;
            uint8_t* current = nullptr;
            uint8_t* marker = nullptr;
            uint8_t* token = nullptr;
            size_t lineNum = 1;
            size_t streamReadOffset = std::numeric_limits<size_t>::max();
            size_t lineStartOffset = 0;
            size_t tokenLineStart =0;
            size_t tokenColumnStart = 0;
            bool eof = false;

            bool fill(const size_t size,
                      ByteStream& stream);
            size_t getColumnOffsetStart() const;
            size_t getColumnOffsetEnd() const;
            size_t getStreamOffset() const;
            size_t getBufferOffsetTokBegin() const;
            size_t getBufferOffsetTokEnd() const ;
            void updateLineStartOffset();
            void markScanBegin();
            size_t getTokenLength() const;
        };

        Status re2cExecute();

        void setToken(const Token token,
                      const LexerState& state);

        void setLexerError(const LexerState& state);

        ByteStream& m_stream;
        StringPool& m_stringPool;
        LexerState m_lexerState;
        TokenInfo m_curToken;
    };
}
