/*
 *  Copyright 2018 by Leander Beernaert (lbb-dev@pm.me)
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

#include "yal/frontend/parser/stparser.h"

#include "yal/error/errorreporter.h"
#include "yal/frontend/lexer/lexer.h"
#include "yal/frontend/lexer/tokens.h"
#include "yal/frontend/lexer/errorlexer.h"
#include "yal/frontend/parser/errorparser.h"
#define YYMALLOCARGTYPE size_t
#include "yal/frontend/parser/stparserimpl.h"
#include "yal/frontend/parser/syntaxtreenodes.h"
#include "yal/util/log.h"
#include "yal/util/prettyprint.h"

#include <cstdlib>

namespace yal::frontend {

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
        case Token::TypeFloat32:
            return YAL_TOKEN_TYPE_FLOAT;
        case Token::TypeFloat64:
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
            return YAL_TOKEN_FLOAT_LITERAL;
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
        case Token::Return:
            return YAL_TOKEN_RETURN;
        case Token::True:
        case Token::False:
            return YAL_TOKEN_BOOL_LITERAL;
        case Token::Mutable:
            return YAL_TOKEN_MUT;
        case Token::Reference:
            return YAL_TOKEN_REFERENCE;
        case Token::Self:
            return YAL_TOKEN_SELF;
        case Token::As:
            return YAL_TOKEN_AS;
        case Token::IntegerLiteralI8:
            return YAL_TOKEN_INTEGER_LITERAL_I8;
        case Token::IntegerLiteralI16:
            return YAL_TOKEN_INTEGER_LITERAL_U16;
        case Token::IntegerLiteralI32:
            return YAL_TOKEN_INTEGER_LITERAL_U32;
        case Token::IntegerLiteralI64:
            return YAL_TOKEN_INTEGER_LITERAL_U64;
        case Token::IntegerLiteralU8:
            return YAL_TOKEN_INTEGER_LITERAL_U8;
        case Token::IntegerLiteralU16:
            return YAL_TOKEN_INTEGER_LITERAL_U16;
        case Token::IntegerLiteralU32:
            return YAL_TOKEN_INTEGER_LITERAL_U32;
        case Token::IntegerLiteralU64:
            return YAL_TOKEN_INTEGER_LITERAL_U64;
        case Token::DecimalLiteral32:
            return YAL_TOKEN_FLOAT_LITERAL_32;
        case Token::DecimalLiteral64:
            return YAL_TOKEN_FLOAT_LITERAL_64;
        case Token::Impl:
            return YAL_TOKEN_IMPL;
        default:
            YAL_ASSERT_MESSAGE(false, "Shouldn't be reached!");
            return -1;
        }
    }

    static void ParserDtor(void* ptr) {
        if (ptr != nullptr) {
            YALSTParserFree(ptr, ::free);
        }
    }

    void
    STParser::OnParseError(STParser &parser) {
        const TokenInfo& ti = parser.m_lexer.getLastToken();
        auto error = std::make_unique<ErrorParser>(ti, parser.getSourceHandle());
        parser.m_errorReporter.report(std::move(error));
    }

    STParser::STParser(Lexer& lexer,
                       STContext& stcontext,
                       ErrorReporter &reporter,
                       SourceItem& sourceItem):
        m_parserImpl(nullptr, ParserDtor),
        m_stcontext(stcontext),
        m_lexer(lexer),
        m_errorReporter(reporter),
        m_sourceItem(sourceItem){
        void*(*fnAlloc)(size_t) = ::malloc;
        void* ptr = YALSTParserAlloc(fnAlloc);
        m_parserImpl.reset(ptr);
    }


    bool
    STParser::parse() {
        Lexer::Status status = Lexer::Status::Ok;

        while (!m_errorReporter.hasErrors()) {
            status = m_lexer.scan();
            if (status == Lexer::Status::Ok) {
                const TokenInfo& ti = m_lexer.getLastToken();
                const int parserToken = TokenToParserToken(ti.token);
                YALSTParser(m_parserImpl.get(),
                          parserToken,
                          ti,
                          this);
            } else if (status == Lexer::Status::EOS) {
                const TokenInfo& ti = m_lexer.getLastToken();
                YALSTParser(m_parserImpl.get(),
                          YAL_TOKEN_END,
                          ti,
                          this);
                break;
            } else {
                const TokenInfo& ti = m_lexer.getLastToken();
                auto error = std::make_unique<ErrorLexer>(ti, m_sourceItem.getHanlde());
                m_errorReporter.report(std::move(error));
            }
        }

        return !m_errorReporter.hasErrors();
    }

    SourceManager::Handle
    STParser::getSourceHandle() const{
        return m_sourceItem.getHanlde();
    }

    SourceInfo
    CreateSourceInfo(const TokenInfo& start,
                     const TokenInfo& end,
                     const SourceManager::Handle handle) {
        SourceInfo srcInfo;
        srcInfo.begin.line = start.lineStart;
        srcInfo.begin.column = start.columnStart;
        srcInfo.begin.streamOffset = start.tokenOffsetInStream;
        srcInfo.end.line = end.lineEnd;
        srcInfo.end.column = end.columnEnd;
        srcInfo.end.streamOffset = end.tokenOffsetInStream + end.tokenStr.size;
        srcInfo.handle = handle;
        return srcInfo;
    }

    SourceInfo
    CreateSourceInfo(const TokenInfo& start,
                     const SourceInfo& end,
                     const SourceManager::Handle handle) {
        SourceInfo srcInfo;
        srcInfo.begin.line = start.lineStart;
        srcInfo.begin.column = start.columnStart;
        srcInfo.begin.streamOffset = start.tokenOffsetInStream;
        srcInfo.end.line = end.end.line;
        srcInfo.end.column = end.end.column;
        srcInfo.end.streamOffset = end.end.streamOffset;
        srcInfo.handle = handle;
        return srcInfo;
    }

    SourceInfo
    CreateSourceInfo(const SourceInfo& start,
                     const TokenInfo& end,
                     const SourceManager::Handle handle) {
        SourceInfo srcInfo;
        srcInfo.begin.line = start.begin.line;
        srcInfo.begin.column = start.begin.column;
        srcInfo.begin.streamOffset = start.begin.streamOffset;
        srcInfo.end.line = end.lineEnd;
        srcInfo.end.column = end.columnEnd;
        srcInfo.end.streamOffset = end.tokenOffsetInStream + end.tokenStr.size;
        srcInfo.handle = handle;
        return srcInfo;
    }

    SourceInfo
    CreateSourceInfo(const SourceInfo& start,
                     const SourceInfo& end,
                     const SourceManager::Handle handle) {
        SourceInfo srcInfo;
        srcInfo.begin.line = start.begin.line;
        srcInfo.begin.column = start.begin.column;
        srcInfo.begin.streamOffset = start.begin.streamOffset;
        srcInfo.end.line = end.end.line;
        srcInfo.end.column = end.end.column;
        srcInfo.end.streamOffset = end.end.streamOffset;
        srcInfo.handle = handle;
        return srcInfo;
    }
}
