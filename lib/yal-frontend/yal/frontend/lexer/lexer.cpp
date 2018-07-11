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
#include "yal/frontend/lexer/lexer.h"
#include "yal/io/memorystream.h"
#include "yal/frontend/lexer/tokens.h"
#include "yal/frontend/lexer/lexer.h"
#include "yal/util/stringpool.h"

namespace yal::frontend {

    bool
    Lexer::LexerState::fill(const size_t size,
                            ByteStream& stream) {

        if (eof) {
            return false;
        }
        const size_t free = token - buffer;
        if (free < size) {
            return false;
        }
        streamReadOffset = static_cast<uintptr_t>(limit - token) + stream.getPosition();
        ::memmove(buffer, token, limit - token);
        limit -= free;
        current -= free;
        marker -= free;
        token -= free;
        limit += stream.read(limit, free);
        if (limit < buffer + kMaxBufferSize) {
            eof = true;
            memset(limit, 0, kYYMaxFill);
            limit += kYYMaxFill;
        }
        return true;
    }


    size_t
    Lexer::LexerState::getColumnOffsetStart() const {
        if ( streamReadOffset== std::numeric_limits<size_t>::max()) {
            return 1;
        }
        const size_t bufferOffset  = getBufferOffsetTokBegin();
        return streamReadOffset + bufferOffset - lineStartOffset + 1;
    }

    size_t
    Lexer::LexerState::getColumnOffsetEnd() const {
        const size_t bufferOffset  =getBufferOffsetTokEnd();
        return streamReadOffset  + bufferOffset - lineStartOffset;
    }

    size_t
    Lexer::LexerState::getStreamOffset() const {
        const size_t bufferOffset  = getBufferOffsetTokBegin();
        return streamReadOffset + bufferOffset;
    }

    size_t
    Lexer::LexerState::getBufferOffsetTokBegin() const {
        const size_t bufferOffset =
                static_cast<size_t>(reinterpret_cast<uintptr_t>(token)
                                    - reinterpret_cast<uintptr_t>(buffer));
        return bufferOffset;
    }

    size_t
    Lexer::LexerState::getBufferOffsetTokEnd() const {
        const size_t bufferOffset =
                static_cast<size_t>(reinterpret_cast<uintptr_t>(current)
                                    - reinterpret_cast<uintptr_t>(buffer));
        return bufferOffset;
    }


    void
    Lexer::LexerState::updateLineStartOffset() {
        lineNum++;
        const size_t bufferOffset  = getBufferOffsetTokEnd();
        lineStartOffset = streamReadOffset + bufferOffset;
    }

    void
    Lexer::LexerState::markScanBegin() {
        token = current;
        tokenLineStart = lineNum;
        tokenColumnStart = getColumnOffsetStart();
    }

    size_t
    Lexer::LexerState::getTokenLength() const {
        return  static_cast<size_t>(reinterpret_cast<uintptr_t>(current)
                                    - reinterpret_cast<uintptr_t>(token));
    }

    Lexer::Status
    Lexer::scan() {
        return re2cExecute();
    }


    Lexer::Lexer(ByteStream &stream,
                 StringPool& stringPool):
        m_stream(stream),
        m_stringPool(stringPool),
        m_lexerState(),
        m_curToken()
    {
        m_lexerState.limit = m_lexerState.buffer + kMaxBufferSize;
        m_lexerState.current = m_lexerState.limit;
        m_lexerState.token =m_lexerState.limit;
        m_lexerState.marker = m_lexerState.limit;
        m_curToken.token = Token::TokenCount;
    }

    Lexer::~Lexer(){

    }

    void
    Lexer::setToken(const Token token,
                    const LexerState& state) {
        m_curToken.token = token;
        m_curToken.lineStart = state.tokenLineStart;
        m_curToken.lineEnd = state.lineNum;
        m_curToken.columnStart = state.tokenColumnStart;
        m_curToken.columnEnd =state.getColumnOffsetEnd();
        m_curToken.tokenOffsetInStream = state.getStreamOffset();
        StringRef tokenStr((const char*)state.token,
                           uintptr_t(state.current) - uintptr_t(state.token));


        switch(token) {
        case Token::Identifier:
        case Token::IdentifierMulti:
        case Token::IntegerLiteral:
        case Token::IntegerLiteralI8:
        case Token::IntegerLiteralI16:
        case Token::IntegerLiteralI32:
        case Token::IntegerLiteralI64:
        case Token::IntegerLiteralU8:
        case Token::IntegerLiteralU16:
        case Token::IntegerLiteralU32:
        case Token::IntegerLiteralU64:
        case Token::DecimalLiteral:
        case Token::DecimalLiteral32:
        case Token::DecimalLiteral64:
        case Token::True:
        case Token::False:
        case Token::Self:
            tokenStr = m_stringPool.getOrCreate(tokenStr);
            break;
        default:
            tokenStr.clear();
            break;
        }

        m_curToken.tokenStr = StringRefPod{tokenStr.data(), tokenStr.size()};
    }

    void
    Lexer::setLexerError(const LexerState& state) {
        m_curToken.token = Token::TokenCount;
        m_curToken.lineStart = state.tokenLineStart;
        m_curToken.lineEnd = m_curToken.lineEnd;
        m_curToken.columnStart = state.tokenColumnStart;
        m_curToken.columnEnd = m_curToken.columnStart;
        m_curToken.tokenOffsetInStream = state.getStreamOffset();
    }
}
