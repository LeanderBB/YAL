/* Generated by re2c 1.0.2 on Thu Nov 16 08:54:13 2017 */
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
#include "yal/yal.h"
#include "yal/io/memorystream.h"
#include "yal/lexer/tokens.h"
#include "yal/lexer/lexer.h"
#include <iostream>
#define YYMAXFILL 7


namespace yal {

    enum {
        kMaxBufferSize = 4096
    };

    struct Lexer::LexerState {
        uint8_t buffer[kMaxBufferSize + YYMAXFILL];
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
                memset(limit, 0, YYMAXFILL);
                limit += YYMAXFILL;
            }
            return true;
        }


        size_t getColumnOffsetStart() const {
            if ( streamReadOffset== std::numeric_limits<size_t>::max()) {
                return 1;
            }

            const size_t bufferOffset  = getBufferOffsetTokBegin();
            return streamReadOffset + bufferOffset - lineStartOffset + 1;
        }

        size_t getColumnOffsetEnd() const {

            const size_t bufferOffset  =getBufferOffsetTokEnd();

            return streamReadOffset  + bufferOffset - lineStartOffset;
        }

        size_t getStreamOffset() const {
            const size_t bufferOffset  = getBufferOffsetTokBegin();
            return streamReadOffset + bufferOffset;
        }

        size_t getBufferOffsetTokBegin() const {
            const size_t bufferOffset =
                    static_cast<size_t>(reinterpret_cast<uintptr_t>(token)
                                        - reinterpret_cast<uintptr_t>(buffer));
            return bufferOffset;
        }

        size_t getBufferOffsetTokEnd() const {
            const size_t bufferOffset =
                    static_cast<size_t>(reinterpret_cast<uintptr_t>(current)
                                        - reinterpret_cast<uintptr_t>(buffer));
            return bufferOffset;
        }


        void updateLineStartOffset() {
            lineNum++;
            const size_t bufferOffset  = getBufferOffsetTokEnd();
            lineStartOffset = streamReadOffset + bufferOffset;
        }

        void markScanBegin() {
            token = current;
            tokenLineStart = lineNum;
            tokenColumnStart = getColumnOffsetStart();
        }

        size_t getTokenLength() const {
            return  static_cast<size_t>(reinterpret_cast<uintptr_t>(current)
                                        - reinterpret_cast<uintptr_t>(token));
        }

    };

    Lexer::Lexer(MemoryStream &stream):
        m_stream(stream),
        m_lexerState(createLexerState()),
        m_curToken()
    {
        m_lexerState->limit = m_lexerState->buffer + kMaxBufferSize;
        m_lexerState->current = m_lexerState->limit;
        m_lexerState->token =m_lexerState->limit;
        m_lexerState->marker = m_lexerState->limit;
        m_curToken.token = Token::TokenCount;
    }

    Lexer::~Lexer(){

    }

    void
    Lexer::setToken(const Token token,
                    const LexerState& state) {
        m_curToken.token = token;
        m_curToken.tokLen = state.getTokenLength();
        m_curToken.lineStart = state.tokenLineStart;
        m_curToken.lineEnd = state.lineNum;
        m_curToken.columnStart = state.tokenColumnStart;
        m_curToken.columnEnd =state.getColumnOffsetEnd();
        m_curToken.tokenOffsetInStream = state.getStreamOffset();
        const char* streamStr =
                static_cast<const char*>(m_stream.getPtrAt(m_curToken.tokenOffsetInStream));
        m_curToken.tokenStr = StringRef(streamStr, m_curToken.tokLen);
    }

    void
    Lexer::setLexerError(const LexerState& state) {
        m_curToken.token = Token::TokenCount;
        m_curToken.tokLen = state.getTokenLength();
        m_curToken.lineStart = state.tokenLineStart;
        m_curToken.lineEnd = m_curToken.lineEnd;
        m_curToken.columnStart = state.tokenColumnStart;
        m_curToken.columnEnd = m_curToken.columnStart;
        m_curToken.tokenOffsetInStream = state.getStreamOffset();
    }

    std::unique_ptr<Lexer::LexerState>
    Lexer::createLexerState(){
        return std::make_unique<Lexer::LexerState>();
    }

    Lexer::Status
    Lexer::re2cExecute() {
        for (;;) {
            m_lexerState->markScanBegin();
            
{
	unsigned char yych;
	unsigned int yyaccept = 0;
	if ((m_lexerState->limit - m_lexerState->current) < 7) if (!m_lexerState->fill(7, m_stream)) return Status::EOS;
	yych = *m_lexerState->current;
	switch (yych) {
	case 0x00:	goto yy2;
	case '\t':
	case ' ':	goto yy6;
	case '\n':	goto yy9;
	case '(':	goto yy11;
	case ')':	goto yy13;
	case '*':	goto yy15;
	case '+':	goto yy17;
	case ',':	goto yy19;
	case '-':	goto yy21;
	case '/':	goto yy23;
	case '0':	goto yy25;
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy26;
	case ':':	goto yy29;
	case ';':	goto yy31;
	case '=':	goto yy33;
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case 'c':
	case 'e':
	case 'g':
	case 'h':
	case 'j':
	case 'k':
	case 'p':
	case 'q':
	case 'r':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy35;
	case '[':	goto yy38;
	case ']':	goto yy40;
	case 'a':	goto yy42;
	case 'b':	goto yy43;
	case 'd':	goto yy44;
	case 'f':	goto yy45;
	case 'i':	goto yy46;
	case 'l':	goto yy47;
	case 'm':	goto yy48;
	case 'n':	goto yy49;
	case 'o':	goto yy50;
	case 's':	goto yy51;
	case 't':	goto yy52;
	case 'u':	goto yy53;
	case 'v':	goto yy54;
	case '{':	goto yy55;
	case '}':	goto yy57;
	default:	goto yy4;
	}
yy2:
	++m_lexerState->current;
	{ if (m_lexerState->limit - m_lexerState->token == YYMAXFILL) {return Status::EOS;} }
yy4:
	++m_lexerState->current;
yy5:
	{ setToken(Token::TokenCount, *m_lexerState); return Status::Error;}
yy6:
	++m_lexerState->current;
	if (m_lexerState->limit <= m_lexerState->current) if (!m_lexerState->fill(1, m_stream)) return Status::EOS;
	yych = *m_lexerState->current;
	switch (yych) {
	case '\t':
	case ' ':	goto yy6;
	default:	goto yy8;
	}
yy8:
	{continue;}
yy9:
	++m_lexerState->current;
	{m_lexerState->updateLineStartOffset(); continue;}
yy11:
	++m_lexerState->current;
	{ setToken(Token::BeginPar, *m_lexerState); return Status::Ok;}
yy13:
	++m_lexerState->current;
	{ setToken(Token::EndPar, *m_lexerState); return Status::Ok;}
yy15:
	++m_lexerState->current;
	{ setToken(Token::Mult, *m_lexerState); return Status::Ok;}
yy17:
	++m_lexerState->current;
	{ setToken(Token::Plus, *m_lexerState); return Status::Ok;}
yy19:
	++m_lexerState->current;
	{ setToken(Token::Comma, *m_lexerState); return Status::Ok;}
yy21:
	++m_lexerState->current;
	{ setToken(Token::Minus, *m_lexerState); return Status::Ok;}
yy23:
	++m_lexerState->current;
	{ setToken(Token::Div, *m_lexerState); return Status::Ok;}
yy25:
	yyaccept = 0;
	yych = *(m_lexerState->marker = ++m_lexerState->current);
	switch (yych) {
	case '.':	goto yy59;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy61;
	default:	goto yy5;
	}
yy26:
	yyaccept = 1;
	m_lexerState->marker = ++m_lexerState->current;
	if ((m_lexerState->limit - m_lexerState->current) < 2) if (!m_lexerState->fill(2, m_stream)) return Status::EOS;
	yych = *m_lexerState->current;
	switch (yych) {
	case '.':	goto yy59;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy26;
	default:	goto yy28;
	}
yy28:
	{ setToken(Token::IntegerLiteral, *m_lexerState); return Status::Ok;}
yy29:
	++m_lexerState->current;
	{ setToken(Token::Colon, *m_lexerState); return Status::Ok;}
yy31:
	++m_lexerState->current;
	{ setToken(Token::SemiColon, *m_lexerState); return Status::Ok;}
yy33:
	++m_lexerState->current;
	{ setToken(Token::Assign, *m_lexerState); return Status::Ok;}
yy35:
	++m_lexerState->current;
	if (m_lexerState->limit <= m_lexerState->current) if (!m_lexerState->fill(1, m_stream)) return Status::EOS;
	yych = *m_lexerState->current;
yy36:
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy35;
	default:	goto yy37;
	}
yy37:
	{ setToken(Token::Identifier, *m_lexerState); return Status::Ok;}
yy38:
	++m_lexerState->current;
	{ setToken(Token::BeginArray, *m_lexerState); return Status::Ok;}
yy40:
	++m_lexerState->current;
	{ setToken(Token::EndArray, *m_lexerState); return Status::Ok;}
yy42:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 'n':	goto yy63;
	default:	goto yy36;
	}
yy43:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 'o':	goto yy64;
	default:	goto yy36;
	}
yy44:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 'o':	goto yy65;
	default:	goto yy36;
	}
yy45:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 'a':	goto yy66;
	case 'l':	goto yy67;
	case 'n':	goto yy68;
	default:	goto yy36;
	}
yy46:
	yych = *++m_lexerState->current;
	switch (yych) {
	case '1':	goto yy70;
	case '3':	goto yy71;
	case '8':	goto yy72;
	default:	goto yy36;
	}
yy47:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 'e':	goto yy74;
	default:	goto yy36;
	}
yy48:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 'o':	goto yy75;
	default:	goto yy36;
	}
yy49:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 'o':	goto yy76;
	default:	goto yy36;
	}
yy50:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 'r':	goto yy77;
	default:	goto yy36;
	}
yy51:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 't':	goto yy79;
	default:	goto yy36;
	}
yy52:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 'h':	goto yy80;
	case 'r':	goto yy81;
	case 'y':	goto yy82;
	default:	goto yy36;
	}
yy53:
	yych = *++m_lexerState->current;
	switch (yych) {
	case '1':	goto yy83;
	case '3':	goto yy84;
	case '8':	goto yy85;
	default:	goto yy36;
	}
yy54:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 'a':	goto yy87;
	default:	goto yy36;
	}
yy55:
	++m_lexerState->current;
	{ setToken(Token::BeginScope, *m_lexerState); return Status::Ok;}
yy57:
	++m_lexerState->current;
	{ setToken(Token::EndScope, *m_lexerState); return Status::Ok;}
yy59:
	yych = *++m_lexerState->current;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy88;
	default:	goto yy60;
	}
yy60:
	m_lexerState->current = m_lexerState->marker;
	if (yyaccept == 0) {
		goto yy5;
	} else {
		goto yy28;
	}
yy61:
	++m_lexerState->current;
	if ((m_lexerState->limit - m_lexerState->current) < 2) if (!m_lexerState->fill(2, m_stream)) return Status::EOS;
	yych = *m_lexerState->current;
	switch (yych) {
	case '.':	goto yy59;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy61;
	default:	goto yy60;
	}
yy63:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 'd':	goto yy91;
	default:	goto yy36;
	}
yy64:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 'o':	goto yy93;
	default:	goto yy36;
	}
yy65:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 'u':	goto yy94;
	default:	goto yy36;
	}
yy66:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 'l':	goto yy95;
	default:	goto yy36;
	}
yy67:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 'o':	goto yy96;
	default:	goto yy36;
	}
yy68:
	yych = *++m_lexerState->current;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy35;
	default:	goto yy69;
	}
yy69:
	{ setToken(Token::Function, *m_lexerState);return Status::Ok; }
yy70:
	yych = *++m_lexerState->current;
	switch (yych) {
	case '6':	goto yy97;
	default:	goto yy36;
	}
yy71:
	yych = *++m_lexerState->current;
	switch (yych) {
	case '2':	goto yy99;
	default:	goto yy36;
	}
yy72:
	yych = *++m_lexerState->current;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy35;
	default:	goto yy73;
	}
yy73:
	{ setToken(Token::TypeInt8, *m_lexerState); return Status::Ok;}
yy74:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 't':	goto yy101;
	default:	goto yy36;
	}
yy75:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 'd':	goto yy103;
	default:	goto yy36;
	}
yy76:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 't':	goto yy105;
	default:	goto yy36;
	}
yy77:
	yych = *++m_lexerState->current;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy35;
	default:	goto yy78;
	}
yy78:
	{ setToken(Token::Or, *m_lexerState); return Status::Ok; }
yy79:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 'r':	goto yy107;
	default:	goto yy36;
	}
yy80:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 'i':	goto yy108;
	default:	goto yy36;
	}
yy81:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 'u':	goto yy109;
	default:	goto yy36;
	}
yy82:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 'p':	goto yy110;
	default:	goto yy36;
	}
yy83:
	yych = *++m_lexerState->current;
	switch (yych) {
	case '6':	goto yy111;
	default:	goto yy36;
	}
yy84:
	yych = *++m_lexerState->current;
	switch (yych) {
	case '2':	goto yy113;
	default:	goto yy36;
	}
yy85:
	yych = *++m_lexerState->current;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy35;
	default:	goto yy86;
	}
yy86:
	{ setToken(Token::TypeUInt8, *m_lexerState); return Status::Ok;}
yy87:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 'r':	goto yy115;
	default:	goto yy36;
	}
yy88:
	++m_lexerState->current;
	if (m_lexerState->limit <= m_lexerState->current) if (!m_lexerState->fill(1, m_stream)) return Status::EOS;
	yych = *m_lexerState->current;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy88;
	default:	goto yy90;
	}
yy90:
	{ setToken(Token::DecimalLiteral, *m_lexerState); return Status::Ok;}
yy91:
	yych = *++m_lexerState->current;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy35;
	default:	goto yy92;
	}
yy92:
	{ setToken(Token::And, *m_lexerState); return Status::Ok; }
yy93:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 'l':	goto yy117;
	default:	goto yy36;
	}
yy94:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 'b':	goto yy119;
	default:	goto yy36;
	}
yy95:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 's':	goto yy120;
	default:	goto yy36;
	}
yy96:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 'a':	goto yy121;
	default:	goto yy36;
	}
yy97:
	yych = *++m_lexerState->current;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy35;
	default:	goto yy98;
	}
yy98:
	{ setToken(Token::TypeInt16, *m_lexerState); return Status::Ok;}
yy99:
	yych = *++m_lexerState->current;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy35;
	default:	goto yy100;
	}
yy100:
	{ setToken(Token::TypeInt32, *m_lexerState); return Status::Ok;}
yy101:
	yych = *++m_lexerState->current;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy35;
	default:	goto yy102;
	}
yy102:
	{ setToken(Token::Let, *m_lexerState);return Status::Ok; }
yy103:
	yych = *++m_lexerState->current;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy35;
	default:	goto yy104;
	}
yy104:
	{ setToken(Token::Mod, *m_lexerState); return Status::Ok;}
yy105:
	yych = *++m_lexerState->current;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy35;
	default:	goto yy106;
	}
yy106:
	{ setToken(Token::Not, *m_lexerState); return Status::Ok; }
yy107:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 'u':	goto yy122;
	default:	goto yy36;
	}
yy108:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 's':	goto yy123;
	default:	goto yy36;
	}
yy109:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 'e':	goto yy125;
	default:	goto yy36;
	}
yy110:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 'e':	goto yy127;
	default:	goto yy36;
	}
yy111:
	yych = *++m_lexerState->current;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy35;
	default:	goto yy112;
	}
yy112:
	{ setToken(Token::TypeUInt16, *m_lexerState); return Status::Ok;}
yy113:
	yych = *++m_lexerState->current;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy35;
	default:	goto yy114;
	}
yy114:
	{ setToken(Token::TypeUInt32, *m_lexerState); return Status::Ok;}
yy115:
	yych = *++m_lexerState->current;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy35;
	default:	goto yy116;
	}
yy116:
	{ setToken(Token::Var, *m_lexerState); return Status::Ok; }
yy117:
	yych = *++m_lexerState->current;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy35;
	default:	goto yy118;
	}
yy118:
	{ setToken(Token::TypeBool, *m_lexerState); return Status::Ok;}
yy119:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 'l':	goto yy129;
	default:	goto yy36;
	}
yy120:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 'e':	goto yy130;
	default:	goto yy36;
	}
yy121:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 't':	goto yy132;
	default:	goto yy36;
	}
yy122:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 'c':	goto yy134;
	default:	goto yy36;
	}
yy123:
	yych = *++m_lexerState->current;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy35;
	default:	goto yy124;
	}
yy124:
	{ setToken(Token::This, *m_lexerState);return Status::Ok; }
yy125:
	yych = *++m_lexerState->current;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy35;
	default:	goto yy126;
	}
yy126:
	{ setToken(Token::True, *m_lexerState); return Status::Ok; }
yy127:
	yych = *++m_lexerState->current;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy35;
	default:	goto yy128;
	}
yy128:
	{ setToken(Token::Type, *m_lexerState); return Status::Ok;}
yy129:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 'e':	goto yy135;
	default:	goto yy36;
	}
yy130:
	yych = *++m_lexerState->current;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy35;
	default:	goto yy131;
	}
yy131:
	{ setToken(Token::False, *m_lexerState); return Status::Ok; }
yy132:
	yych = *++m_lexerState->current;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy35;
	default:	goto yy133;
	}
yy133:
	{ setToken(Token::TypeFloat, *m_lexerState); return Status::Ok;}
yy134:
	yych = *++m_lexerState->current;
	switch (yych) {
	case 't':	goto yy137;
	default:	goto yy36;
	}
yy135:
	yych = *++m_lexerState->current;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy35;
	default:	goto yy136;
	}
yy136:
	{ setToken(Token::TypeDouble, *m_lexerState); return Status::Ok;}
yy137:
	yych = *++m_lexerState->current;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':	goto yy35;
	default:	goto yy138;
	}
yy138:
	{ setToken(Token::Struct, *m_lexerState); return Status::Ok;}
}

        }

    }
}
