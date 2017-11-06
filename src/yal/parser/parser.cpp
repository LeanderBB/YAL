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
#include "yal/parser/parser.h"
#include "yal/parser/parserimpl.h"
#include "yal/lexer/lexer.h"
#include "yal/lexer/tokens.h"
#include "yal/util/log.h"
#include "yal/io/memorystream.h"
#include "yal/util/prettyprint.h"
#include "yal/ast/declfunction.h"
#include "yal/ast/reftypebuiltin.h"
#include "yal/ast/reftypeidentifier.h"
namespace yal{

    static int TokenToParserToken(const Token token) {

        // TODO: Optimize out later when all tokens are known
        switch(token)
        {
        case Token::And:
            return YAL_TOKEN_AND;
        case Token::Or:
            return YAL_TOKEN_OR;
        case Token::Not:
            return YAL_TOKEN_NOT;
        case Token::TypeBool:
            return YAL_TOKEN_TYPE_BOOL;
        case Token::TypeInt8:
            return YAL_TOKEN_TYPE_INT8;
        case Token::TypeUInt8:
            return YAL_TOKEN_TYPE_UINT8;
        case Token::TypeInt16:
            return YAL_TOKEN_TYPE_INT16;
        case Token::TypeUInt16:
            return YAL_TOKEN_TYPE_UINT16;
        case Token::TypeInt32:
            return YAL_TOKEN_TYPE_INT32;
        case Token::TypeUInt32:
            return YAL_TOKEN_TYPE_UINT32;
        case Token::TypeInt64:
            return YAL_TOKEN_TYPE_INT64;
        case Token::TypeUInt64:
            return YAL_TOKEN_TYPE_UINT64;
        case Token::TypeFloat:
            return YAL_TOKEN_TYPE_FLOAT;
        case Token::TypeDouble:
            return YAL_TOKEN_TYPE_DOUBLE;
        case Token::Mod:
            return YAL_TOKEN_MOD;
        case Token::Dot:
            return YAL_TOKEN_DOT;
        case Token::BitXor:
            return YAL_TOKEN_BIT_XOR;
        case Token::BitOr:
            return YAL_TOKEN_BIT_OR;
        case Token::CompareGe:
            return YAL_TOKEN_GE;
        case Token::CompareGt:
            return YAL_TOKEN_GT;
        case Token::CompareLe:
            return YAL_TOKEN_LE;
        case Token::CompareLt:
            return YAL_TOKEN_LT;
        case Token::CompareEq:
            return YAL_TOKEN_EQ;
        case Token::CompareNe:
            return YAL_TOKEN_NE;
        case Token::Assign:
            return YAL_TOKEN_ASSIGN;
        case Token::Plus:
            return YAL_TOKEN_PLUS;
        case Token::Minus:
            return YAL_TOKEN_MINUS;
        case Token::Mult:
            return YAL_TOKEN_MULT;
        case Token::Div:
            return YAL_TOKEN_DIV;
        case Token::Identifier:
            return YAL_TOKEN_IDENTIFIER;
        case Token::Colon:
            return YAL_TOKEN_COLON;
        case Token::SemiColon:
            return YAL_TOKEN_SEMI_COLON;
        case Token::Type:
            return YAL_TOKEN_TYPE;
        case Token::BeginScope:
            return YAL_TOKEN_SCOPE_BEGIN;
        case Token::EndScope:
            return YAL_TOKEN_SCOPE_END;
        case Token::BeginPar:
            return YAL_TOKEN_PAR_BEGIN;
        case Token::EndPar:
            return YAL_TOKEN_PAR_END;
        case Token::IntegerLiteral:
            return YAL_TOKEN_INTEGER_LITERAL;
        case Token::DecimalLiteral:
            return YAL_TOKEN_DECIMAL_LITERAL;
        case Token::Function:
            return YAL_TOKEN_FUNCTION;
        case Token::Comma:
            return YAL_TOKEN_COMMA;
        case Token::Var:
            return YAL_TOKEN_VAR;
        case Token::Let:
            return YAL_TOKEN_LET;
        case Token::Struct:
            return YAL_TOKEN_STRUCT;
        default:
            YAL_ASSERT_MESSAGE(false, "Shouldn't be reached!");
            return -1;
        }
    }

    static void ParserDtor(void* ptr) {
        if (ptr != nullptr) {
            YALParserFree(ptr, ::free);
        }
    }

    Parser::Parser(Lexer& lexer,
                   Log &log,
                   Module& context):
        m_parserImpl(YALParserAlloc(::malloc), ParserDtor),
        m_lexer(lexer),
        m_log(log),
        m_module(context),
        m_status(Result::SyntaxError) {

    }

    Parser::Result
    Parser::run() {
        m_status = Result::Ok;
        Lexer::Status status = Lexer::Status::Ok;
        while(m_status == Result::Ok){
            status = m_lexer.scan();
            if (status == Lexer::Status::Ok) {
                const Lexer::TokenInfo& ti = m_lexer.getLastToken();
                const int parserToken = TokenToParserToken(ti.token);
#if defined(YAL_PARSER_DEBUG_DUMP_TOKENS)
                m_log.debug("Token(%s) l:%04d c:%03u\n",
                            TokenToString(ti.token),
                            ti.lineStart, ti.columnStart);
#endif
                YALParser(m_parserImpl.get(),
                          parserToken,
                          ti.tokenStr.toStringRefPod(),
                          this);
            } else if (status == Lexer::Status::EOS) {
                YALParser(m_parserImpl.get(),
                          YAL_TOKEN_END,
                          StringRefPod{nullptr, 0},
                          this);
                break;
            } else {
                MemoryStream& stream = m_lexer.getStream();
                const Lexer::TokenInfo& ti = m_lexer.getLastToken();

                PrettyPrint::SourceErrorPrint(stream,
                                              m_log,
                                              ti.lineStart,
                                              ti.lineEnd);
                m_log.error("Unknown token: %0004u  column:%03u\n",
                            ti.lineStart, ti.columnStart);
                m_status = Result::LexerError;
            }
        }

        return m_status;
    }

     void
     Parser::logParseFailure()  {
        ByteStream& stream = m_lexer.getStream();
        const Lexer::TokenInfo& ti = m_lexer.getLastToken();
        PrettyPrint::SourceErrorPrint(stream,
                                      m_log,
                                      ti.lineStart,
                                      ti.lineEnd);
        m_log.error("Syntax Error at line: %0004u  column:%03u\n",
                    ti.lineStart, ti.columnStart);
        m_status = Result::SyntaxError;
     }
}
