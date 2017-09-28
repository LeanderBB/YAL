#include <gtest/gtest.h>
#include <yal/yal.h>
#include <yal/io/memorystream.h>
#include <yal/lexer/lexer.h>
#include <yal/lexer/tokens.h>
#include <string>
#include <iostream>


TEST(Lexer, BasicTokenError) {
    const std::string input ="let var \n class false \nthis and";
    auto stream = yal::MemoryStream::Attach(input.c_str(),
                                            input.size());
    yal::Lexer lexer(*stream);
    yal::Lexer::LexerStatus status = lexer.scan();
    EXPECT_EQ(status, yal::Lexer::LexerStatus::Ok);
    status = lexer.scan();
    EXPECT_EQ(status, yal::Lexer::LexerStatus::Ok);
    status = lexer.scan();
    EXPECT_EQ(status, yal::Lexer::LexerStatus::Error);
    {
        const yal::Lexer::TokenInfo& tk = lexer.getLastToken();
        EXPECT_EQ(tk.lineStart, 2u);
        EXPECT_EQ(tk.columnStart, 1u);
    }
}

TEST(Lexer, BasicTokenRead) {
    const std::string input ="let var \n true false \nthis and";

    auto stream = yal::MemoryStream::Attach(input.c_str(),
                                            input.size());
    yal::Lexer lexer(*stream);

    yal::Lexer::LexerStatus status = lexer.scan();

    {
        EXPECT_EQ(status, yal::Lexer::LexerStatus::Ok);
        const yal::Lexer::TokenInfo& tk = lexer.getLastToken();
        EXPECT_EQ(tk.token, yal::Token::Let);
        EXPECT_EQ(tk.lineStart, 1u);
        EXPECT_EQ(tk.lineEnd, 1u);
        EXPECT_EQ(tk.columnStart, 0u);
        EXPECT_EQ(tk.columnEnd, 3u);
        EXPECT_EQ(tk.tokLen, 3u);
        EXPECT_EQ(tk.tokenOffsetInStream, 0u);
    }

    status = lexer.scan();
    {
        EXPECT_EQ(status, yal::Lexer::LexerStatus::Ok);
        const yal::Lexer::TokenInfo& tk = lexer.getLastToken();
        EXPECT_EQ(tk.token, yal::Token::Var);
        EXPECT_EQ(tk.lineStart, 1u);
        EXPECT_EQ(tk.lineEnd, 1u);
        EXPECT_EQ(tk.columnStart, 4u);
        EXPECT_EQ(tk.columnEnd, 7u);
        EXPECT_EQ(tk.tokLen, 3u);
        EXPECT_EQ(tk.tokenOffsetInStream, 4u);
    }

    status = lexer.scan();
    {
        EXPECT_EQ(status, yal::Lexer::LexerStatus::Ok);
        const yal::Lexer::TokenInfo& tk = lexer.getLastToken();
        EXPECT_EQ(tk.token, yal::Token::True);
        EXPECT_EQ(tk.lineStart, 2u);
        EXPECT_EQ(tk.lineEnd, 2u);
        EXPECT_EQ(tk.columnStart, 1u);
        EXPECT_EQ(tk.columnEnd, 5u);
        EXPECT_EQ(tk.tokLen, 4u);
        EXPECT_EQ(tk.tokenOffsetInStream, 10u);
    }

    status = lexer.scan();
    {
        EXPECT_EQ(status, yal::Lexer::LexerStatus::Ok);
        const yal::Lexer::TokenInfo& tk = lexer.getLastToken();
        EXPECT_EQ(tk.token, yal::Token::False);
        EXPECT_EQ(tk.lineStart, 2u);
        EXPECT_EQ(tk.lineEnd, 2u);
        EXPECT_EQ(tk.columnStart, 6u);
        EXPECT_EQ(tk.columnEnd, 11u);
        EXPECT_EQ(tk.tokLen, 5u);
        EXPECT_EQ(tk.tokenOffsetInStream, 15u);
    }

    status = lexer.scan();
    {
        EXPECT_EQ(status, yal::Lexer::LexerStatus::Ok);
        const yal::Lexer::TokenInfo& tk = lexer.getLastToken();
        EXPECT_EQ(tk.token, yal::Token::This);
        EXPECT_EQ(tk.lineStart, 3u);
        EXPECT_EQ(tk.lineEnd, 3u);
        EXPECT_EQ(tk.columnStart, 0u);
        EXPECT_EQ(tk.columnEnd, 4u);
        EXPECT_EQ(tk.tokLen, 4u);
        EXPECT_EQ(tk.tokenOffsetInStream, 22u);
    }

    status = lexer.scan();
    {
        EXPECT_EQ(status, yal::Lexer::LexerStatus::Ok);
        const yal::Lexer::TokenInfo& tk = lexer.getLastToken();
        EXPECT_EQ(tk.token, yal::Token::And);
        EXPECT_EQ(tk.lineStart, 3u);
        EXPECT_EQ(tk.lineEnd, 3u);
        EXPECT_EQ(tk.columnStart, 5u);
        EXPECT_EQ(tk.columnEnd, 8u);
        EXPECT_EQ(tk.tokLen, 3u);
        EXPECT_EQ(tk.tokenOffsetInStream, 27u);
    }

    status = lexer.scan();
    EXPECT_EQ(status, yal::Lexer::LexerStatus::EOS);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
