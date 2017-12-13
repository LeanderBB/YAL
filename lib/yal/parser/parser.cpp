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
#define YYMALLOCARGTYPE size_t
#include "yal/lexer/lexer.h"
#include "yal/parser/parserimpl.h"
#include "yal/lexer/lexer.h"
#include "yal/lexer/tokens.h"
#include "yal/util/log.h"
#include "yal/io/memorystream.h"
#include "yal/util/prettyprint.h"
#include "yal/ast/declfunction.h"
#include "yal/ast/reftype.h"
#include "yal/ast/exprintegerliteral.h"
#include "yal/ast/exprdecimalliteral.h"
#include "yal/util/strconversions.h"
#include "yal/ast/declmodule.h"
#include "yal/ast/decltypefunction.h"
#include "yal/ast/declfunction.h"
#include "yal/ast/declstruct.h"
#include "yal/compiler/stages/stagedecls.h"
#include <cstdlib>
#include <limits>
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
                   Module& context,
                   StageDecls &stageDecls):
        m_parserImpl(nullptr, ParserDtor),
        m_lexer(lexer),
        m_log(log),
        m_module(context),
        m_stageDecls(stageDecls),
        m_status(Result::SyntaxError) {
        void*(*fnAlloc)(size_t) = ::malloc;
        void* ptr = YALParserAlloc(fnAlloc);
        m_parserImpl.reset(ptr);
    }

    Parser::Result
    Parser::run() {
        m_status = Result::Ok;
        Lexer::Status status = Lexer::Status::Ok;
        m_activeDecl = m_module.getDeclNode();
        while(m_status == Result::Ok){
            status = m_lexer.scan();
            if (status == Lexer::Status::Ok) {
                const TokenInfo& ti = m_lexer.getLastToken();
                const int parserToken = TokenToParserToken(ti.token);
#if defined(YAL_PARSER_DEBUG_DUMP_TOKENS)
                m_log.debug("Token(%s) l:%04d c:%03u\n",
                            TokenToString(ti.token),
                            ti.lineStart, ti.columnStart);
#endif
                YALParser(m_parserImpl.get(),
                          parserToken,
                          ti,
                          this);
            } else if (status == Lexer::Status::EOS) {
                const TokenInfo& ti = m_lexer.getLastToken();
                YALParser(m_parserImpl.get(),
                          YAL_TOKEN_END,
                          ti,
                          this);
                break;
            } else {
                const TokenInfo& ti = m_lexer.getLastToken();

                PrettyPrint::SourceErrorPrint(m_stageDecls.m_sourceItem,
                                              m_log,
                                              ti.lineStart,
                                              ti.lineEnd,
                                              ti.columnStart,
                                              ti.columnEnd);
                m_log.error("Unknown token at line:%  column:%\n",
                            ti.lineStart, ti.columnStart);
                m_status = Result::LexerError;
            }
        }

        return m_status;
    }

    void
    Parser::logParseFailure()  {
        const TokenInfo& ti = m_lexer.getLastToken();
        PrettyPrint::SourceErrorPrint(m_stageDecls.m_sourceItem,
                                      m_log,
                                      ti.lineStart,
                                      ti.lineEnd,
                                      ti.columnStart,
                                      ti.columnEnd);
        m_log.error("Syntax Error\n");
        m_status = Result::SyntaxError;
    }

    void
    Parser::onDeclBegin(DeclFunction* decl) {
        if (!m_stageDecls.onDecl(decl)) {
            m_status = Result::DeclError;
        }
        m_activeDecl = decl;
    }

    void
    Parser::onDeclBegin(DeclTypeFunction* decl) {
        if (!m_stageDecls.onDecl(decl)) {
            m_status = Result::DeclError;
        }
         m_activeDecl = decl;
    }

    void
    Parser::onDeclBegin(DeclStruct* decl) {
        if (!m_stageDecls.onDecl(decl)) {
            m_status = Result::DeclError;
        }
        m_activeDecl = decl;
    }

    bool
    Parser::onDecl(DeclVar* decl) {
        if (!m_stageDecls.onDecl(decl)) {
            m_status = Result::DeclError;
            return false;
        }
        return true;
    }

    bool
    Parser::onDecl(DeclParamVar* decl) {
        if (!m_stageDecls.onDecl(decl)) {
            m_status = Result::DeclError;
            return false;
        }
        return true;
    }

    void
    Parser::onDeclEnd() {
        m_stageDecls.popDeclScope();
        m_activeDecl = nullptr;
    }

    const Type*
    Parser::resolveType(const TokenInfo& ti) {
        Type* resolvedType = m_stageDecls.resolveType(ti);
        if (resolvedType == nullptr) {
            m_status = Result::TypeError;
        }
        return resolvedType;
    }

    const Type*
    Parser::resolveSelfType() const {
        YAL_ASSERT(m_activeDecl != nullptr);
        DeclTypeFunction* typefn = dyn_cast<DeclTypeFunction>(m_activeDecl);
        YAL_ASSERT(typefn != nullptr);
        return typefn->getTargetType()->getType();
    }


    DeclVar*
    Parser::resolveVarRef(const TokenInfo& ti) {
        DeclVar* result = m_stageDecls.resolveVarRef(ti);
        if (result == nullptr) {
            m_status = Result::DeclError;
        }
        return result;
    }

    DeclVar*
    Parser::resolveVarRefSelf(const TokenInfo& ti) {
        DeclVar* result = m_stageDecls.resolveVarRefSelf(ti);
        if (result == nullptr) {
            m_status = Result::DeclError;
        }
        return result;
    }

    ExprIntegerLiteral*
    Parser::newIntegerLiteral(const TokenInfo& ti) {
        const StringRef& str = ti.tokenStr;
        YAL_ASSERT(ti.token == Token::IntegerLiteral);

        uint64_t value = 0;
        IntegerType intType = IntegerType::U64;
        bool negative = false;
        if (StringRefToInteger(value, str, negative)) {
            if (negative) {
                const int64_t& negValue = reinterpret_cast<int64_t&>(value);
                if (negValue >= static_cast<int64_t>(std::numeric_limits<int8_t>::lowest())) {
                    intType = IntegerType::I8;
                } else if (negValue >= static_cast<int64_t>(std::numeric_limits<int16_t>::lowest())) {
                    intType = IntegerType::I16;
                } else if (negValue >= static_cast<int64_t>(std::numeric_limits<int32_t>::lowest())) {
                    intType = IntegerType::I32;
                } else {
                    intType = IntegerType::I64;
                }
            } else {
                if (value <= static_cast<uint64_t>(std::numeric_limits<int8_t>::max())) {
                    intType = IntegerType::I8;
                } else if (value <= static_cast<uint64_t>(std::numeric_limits<uint8_t>::max())) {
                    intType = IntegerType::U8;
                } else if (value <= static_cast<uint64_t>(std::numeric_limits<int16_t>::max())) {
                    intType = IntegerType::I16;
                } else if (value <= static_cast<uint64_t>(std::numeric_limits<uint16_t>::max())) {
                    intType = IntegerType::U16;
                } else if (value <= static_cast<uint64_t>(std::numeric_limits<int32_t>::max())) {
                    intType = IntegerType::I32;
                } else if (value <= static_cast<uint64_t>(std::numeric_limits<uint32_t>::max())) {
                    intType = IntegerType::U32;
                } else if (value <= static_cast<uint64_t>(std::numeric_limits<int64_t>::max())) {
                    intType = IntegerType::I64;
                } else {
                    intType = IntegerType::U64;
                }
            }
            return newAstNode<ExprIntegerLiteral>(intType, value);
        }

        PrettyPrint::SourceErrorPrint(m_stageDecls.m_sourceItem,
                                      m_log,
                                      ti.lineStart,
                                      ti.lineEnd,
                                      ti.columnStart,
                                      ti.columnEnd);
        m_log.error("Integer Literal conversion error, ");
        m_log.error("%", "Value is not a valid integer literal.\n");
        m_status = Result::TypeError;
        return nullptr;
    }


    ExprDecimalLiteral*
    Parser::newDecimalLiteral(const TokenInfo& ti) {
        const StringRef& str = ti.tokenStr;
        YAL_ASSERT(ti.token == Token::IntegerLiteral);

        double value = 0;

        if (StringRefToDecimal(value, str)) {
            return newAstNode<ExprDecimalLiteral>(value);
        }

        PrettyPrint::SourceErrorPrint(m_stageDecls.m_sourceItem,
                                      m_log,
                                      ti.lineStart,
                                      ti.lineEnd,
                                      ti.columnStart,
                                      ti.columnEnd);
        m_log.error("Decimal Literal conversion error, ");
        m_log.error("%", "Value is not a valid decimal literal.\n");
        m_status = Result::TypeError;
        return nullptr;
    }

    SourceInfo
    Parser::createSourceInfo(const TokenInfo& start,
                             const TokenInfo& end) const{
        SourceInfo srcInfo;
        srcInfo.begin.line = start.lineStart;
        srcInfo.begin.column = start.columnStart;
        srcInfo.begin.streamOffset = start.tokenOffsetInStream;
        srcInfo.end.line = end.lineEnd;
        srcInfo.end.column = end.columnEnd;
        srcInfo.end.streamOffset = end.tokenOffsetInStream + end.tokenStr.size;
        srcInfo.handle = m_module.getSourceHandle();
        return srcInfo;
    }

    SourceInfo
    Parser::createSourceInfo(const TokenInfo& start,
                             const SourceInfo& end) const {
        SourceInfo srcInfo;
        srcInfo.begin.line = start.lineStart;
        srcInfo.begin.column = start.columnStart;
        srcInfo.begin.streamOffset = start.tokenOffsetInStream;
        srcInfo.end.line = end.end.line;
        srcInfo.end.column = end.end.column;
        srcInfo.end.streamOffset = end.end.streamOffset;
        srcInfo.handle = m_module.getSourceHandle();
        return srcInfo;
    }

    SourceInfo
    Parser::createSourceInfo(const SourceInfo& start,
                             const TokenInfo& end) const {
        SourceInfo srcInfo;
        srcInfo.begin.line = start.begin.line;
        srcInfo.begin.column = start.begin.column;
        srcInfo.begin.streamOffset = start.begin.streamOffset;
        srcInfo.end.line = end.lineEnd;
        srcInfo.end.column = end.columnEnd;
        srcInfo.end.streamOffset = end.tokenOffsetInStream + end.tokenStr.size;
        srcInfo.handle = m_module.getSourceHandle();
        return srcInfo;
    }

    SourceInfo
    Parser::createSourceInfo(const SourceInfo& start,
                             const SourceInfo& end) const{
        SourceInfo srcInfo;
        srcInfo.begin.line = start.begin.line;
        srcInfo.begin.column = start.begin.column;
        srcInfo.begin.streamOffset = start.begin.streamOffset;
        srcInfo.end.line = end.end.line;
        srcInfo.end.column = end.end.column;
        srcInfo.end.streamOffset = end.end.streamOffset;
        return srcInfo;
    }
}
