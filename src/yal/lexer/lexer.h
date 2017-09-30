#pragma once
#include <yal/yal.h>
namespace yal{

    enum class Token;
    class ByteStream;
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
        };

        enum class Status
        {
            Ok,
            Error,
            EOS
        };

        Lexer(ByteStream& stream);

        ~Lexer();

        Status scan();

        const TokenInfo& getLastToken() const {
            return m_curToken;
        }

    private:

        struct LexerState;

        static std::unique_ptr<LexerState> createLexerState();

        Status re2cExecute();

        void setToken(const Token token,
                      const LexerState& state);

        ByteStream& m_stream;
        std::unique_ptr<LexerState> m_lexerState;
        TokenInfo m_curToken;
    };
}
