#include "yal/parser/parser.h"
#include "yal/parser/parserincludes.h"
#include "yal/parser/parserimpl.h"
#include "yal/lexer/lexer.h"
#include "yal/lexer/tokens.h"
#include <iostream>
namespace yal{

    static int TokenToParserToken(const Token token) {
        static const int sLookupTable[static_cast<uint32_t>(Token::TokenCount)] ={
                -1,
                -1,
                -1,
                -1,
                -1,
                -1,
                -1,
                YAL_TOKEN_AND,
                YAL_TOKEN_OR,
                YAL_TOKEN_NOT,
                -1,
                -1,
                -1,
                -1,
                -1,
                -1,
                -1,
                -1,
                -1,
                -1,
                -1,
                YAL_TOKEN_MOD,
                YAL_TOKEN_DOT,
                -1,
                -1,
                YAL_TOKEN_SCOPE_BEGIN,
                YAL_TOKEN_SCOPE_END,
                -1,
                -1,
                YAL_TOKEN_BIT_XOR,
                -1,
                YAL_TOKEN_BIT_OR,
                YAL_TOKEN_GE,
                YAL_TOKEN_GT,
                YAL_TOKEN_LE,
                YAL_TOKEN_LT,
                YAL_TOKEN_EQ,
                YAL_TOKEN_NE,
                YAL_TOKEN_ASSIGN,
                -1,
                -1,
                -1,
                -1,
                -1,
                -1,
                -1,
                -1,
                -1,
                YAL_TOKEN_PLUS,
                YAL_TOKEN_MINUS,
                YAL_TOKEN_MULT,
                YAL_TOKEN_DIV,
                YAL_TOKEN_NAME,
                YAL_TOKEN_COLON,
                YAL_TOKEN_SEMI_COLON,
                YAL_TOKEN_TYPE
    };

        static_assert(sizeof(sLookupTable)/sizeof(sLookupTable[0]) ==
                static_cast<uint32_t>(Token::TokenCount),
                "Table size must match Token::TokenCount value");

        return sLookupTable[static_cast<uint32_t>(token)];
    }

    static void ParserDtor(void* ptr) {
        if (ptr != nullptr) {
            YALParserFree(ptr, ::free);
        }
    }

    Parser::Parser(Lexer& lexer):
        m_parserImpl(YALParserAlloc(::malloc), ParserDtor),
        m_lexer(lexer)
    {

    }

    bool
    Parser::run() {
        Lexer::Status status = Lexer::Status::Ok;
        while(true){
            status = m_lexer.scan();
            if (status == Lexer::Status::Ok) {
                const Lexer::TokenInfo& ti = m_lexer.getLastToken();
                const int parserToken = TokenToParserToken(ti.token);
                /*const char* strToken = TokenToString(ti.token);
                std::cout << "Token: " << strToken
                          << " c: " << ti.columnStart
                          << " l: " << ti.lineEnd
                          << std::endl;*/

                YALParser(m_parserImpl.get(),
                          parserToken,
                          nullptr);
            } else if (status == Lexer::Status::EOS) {
                YALParser(m_parserImpl.get(),
                          YAL_TOKEN_END,
                          nullptr);
                return true;
            } else {
                return false;
            }
        }
    }
}
