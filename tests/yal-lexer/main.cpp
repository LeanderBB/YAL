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
#include <gtest/gtest.h>
#include <yal/yal.h>
#include <yal/io/memorystream.h>
#include <yal/lexer/lexer.h>
#include <yal/lexer/tokens.h>
#include <string>
#include <iostream>

/*
TEST(Lexer, BasicTokenError) {
    const std::string input ="let var \n + false \nthis and";
    yal::MemoryStream stream;
    stream.attach(input.c_str(),input.size(), false);

    yal::Lexer lexer(stream);
    yal::Lexer::Status status = lexer.scan();
    EXPECT_EQ(status, yal::Lexer::Status::Ok);
    status = lexer.scan();
    EXPECT_EQ(status, yal::Lexer::Status::Ok);
    status = lexer.scan();
    EXPECT_EQ(status, yal::Lexer::Status::Error);
    {
        const yal::Lexer::TokenInfo& tk = lexer.getLastToken();
        EXPECT_EQ(tk.lineStart, 2u);
        EXPECT_EQ(tk.columnStart, 2u);
    }
}*/

TEST(Lexer, BasicTokenRead) {
    const std::string input ="let var \n true false \nthis and varName91";

    yal::MemoryStream stream;

    stream.attach(input.c_str(),input.size(), false);

    yal::Lexer lexer(stream);

    yal::Lexer::Status status = lexer.scan();

    {
        EXPECT_EQ(status, yal::Lexer::Status::Ok);
        const yal::Lexer::TokenInfo& tk = lexer.getLastToken();
        EXPECT_EQ(tk.token, yal::Token::Let);
        EXPECT_EQ(tk.lineStart, 1u);
        EXPECT_EQ(tk.lineEnd, 1u);
        EXPECT_EQ(tk.columnStart, 1u);
        EXPECT_EQ(tk.columnEnd, 3u);
        EXPECT_EQ(tk.tokLen, 3u);
        EXPECT_EQ(tk.tokenOffsetInStream, 0u);
    }

    status = lexer.scan();
    {
        EXPECT_EQ(status, yal::Lexer::Status::Ok);
        const yal::Lexer::TokenInfo& tk = lexer.getLastToken();
        EXPECT_EQ(tk.token, yal::Token::Var);
        EXPECT_EQ(tk.lineStart, 1u);
        EXPECT_EQ(tk.lineEnd, 1u);
        EXPECT_EQ(tk.columnStart, 5u);
        EXPECT_EQ(tk.columnEnd, 7u);
        EXPECT_EQ(tk.tokLen, 3u);
        EXPECT_EQ(tk.tokenOffsetInStream, 4u);
    }

    status = lexer.scan();
    {
        EXPECT_EQ(status, yal::Lexer::Status::Ok);
        const yal::Lexer::TokenInfo& tk = lexer.getLastToken();
        EXPECT_EQ(tk.token, yal::Token::True);
        EXPECT_EQ(tk.lineStart, 2u);
        EXPECT_EQ(tk.lineEnd, 2u);
        EXPECT_EQ(tk.columnStart, 2u);
        EXPECT_EQ(tk.columnEnd, 5u);
        EXPECT_EQ(tk.tokLen, 4u);
        EXPECT_EQ(tk.tokenOffsetInStream, 10u);
    }

    status = lexer.scan();
    {
        EXPECT_EQ(status, yal::Lexer::Status::Ok);
        const yal::Lexer::TokenInfo& tk = lexer.getLastToken();
        EXPECT_EQ(tk.token, yal::Token::False);
        EXPECT_EQ(tk.lineStart, 2u);
        EXPECT_EQ(tk.lineEnd, 2u);
        EXPECT_EQ(tk.columnStart, 7u);
        EXPECT_EQ(tk.columnEnd, 11u);
        EXPECT_EQ(tk.tokLen, 5u);
        EXPECT_EQ(tk.tokenOffsetInStream, 15u);
    }

    status = lexer.scan();
    {
        EXPECT_EQ(status, yal::Lexer::Status::Ok);
        const yal::Lexer::TokenInfo& tk = lexer.getLastToken();
        EXPECT_EQ(tk.token, yal::Token::This);
        EXPECT_EQ(tk.lineStart, 3u);
        EXPECT_EQ(tk.lineEnd, 3u);
        EXPECT_EQ(tk.columnStart, 1u);
        EXPECT_EQ(tk.columnEnd, 4u);
        EXPECT_EQ(tk.tokLen, 4u);
        EXPECT_EQ(tk.tokenOffsetInStream, 22u);
    }

    status = lexer.scan();
    {
        EXPECT_EQ(status, yal::Lexer::Status::Ok);
        const yal::Lexer::TokenInfo& tk = lexer.getLastToken();
        EXPECT_EQ(tk.token, yal::Token::And);
        EXPECT_EQ(tk.lineStart, 3u);
        EXPECT_EQ(tk.lineEnd, 3u);
        EXPECT_EQ(tk.columnStart, 6u);
        EXPECT_EQ(tk.columnEnd, 8u);
        EXPECT_EQ(tk.tokLen, 3u);
        EXPECT_EQ(tk.tokenOffsetInStream, 27u);
    }

    status = lexer.scan();
    {
        EXPECT_EQ(status, yal::Lexer::Status::Ok);
        const yal::Lexer::TokenInfo& tk = lexer.getLastToken();
        EXPECT_EQ(tk.token, yal::Token::Identifier);
    }

    status = lexer.scan();
    EXPECT_EQ(status, yal::Lexer::Status::EOS);
}

TEST(Lexer, TypeDeclTokens) {
    const std::string input ="MyType : type {\n foo : type; \n}\n";
    yal::MemoryStream stream;
    stream.attach(input.c_str(),input.size(), false);

    yal::Lexer lexer(stream);
    yal::Lexer::Status status = lexer.scan();
    {
        EXPECT_EQ(status, yal::Lexer::Status::Ok);
        const yal::Lexer::TokenInfo& tk = lexer.getLastToken();
        EXPECT_EQ(tk.token, yal::Token::Identifier);
    }

    status = lexer.scan();
    {
        EXPECT_EQ(status, yal::Lexer::Status::Ok);
        const yal::Lexer::TokenInfo& tk = lexer.getLastToken();
        EXPECT_EQ(tk.token, yal::Token::Colon);
    }

    status = lexer.scan();
    {
        EXPECT_EQ(status, yal::Lexer::Status::Ok);
        const yal::Lexer::TokenInfo& tk = lexer.getLastToken();
        EXPECT_EQ(tk.token, yal::Token::Type);
    }

    status = lexer.scan();
    {
        EXPECT_EQ(status, yal::Lexer::Status::Ok);
        const yal::Lexer::TokenInfo& tk = lexer.getLastToken();
        EXPECT_EQ(tk.token, yal::Token::BeginScope);
    }

    status = lexer.scan();
    {
        EXPECT_EQ(status, yal::Lexer::Status::Ok);
        const yal::Lexer::TokenInfo& tk = lexer.getLastToken();
        EXPECT_EQ(tk.token, yal::Token::Identifier);
    }

    status = lexer.scan();
    {
        EXPECT_EQ(status, yal::Lexer::Status::Ok);
        const yal::Lexer::TokenInfo& tk = lexer.getLastToken();
        EXPECT_EQ(tk.token, yal::Token::Colon);
    }

    status = lexer.scan();
    {
        EXPECT_EQ(status, yal::Lexer::Status::Ok);
        const yal::Lexer::TokenInfo& tk = lexer.getLastToken();
        EXPECT_EQ(tk.token, yal::Token::Type);
    }

    status = lexer.scan();
    {
        EXPECT_EQ(status, yal::Lexer::Status::Ok);
        const yal::Lexer::TokenInfo& tk = lexer.getLastToken();
        EXPECT_EQ(tk.token, yal::Token::SemiColon);
    }

    status = lexer.scan();
    {
        EXPECT_EQ(status, yal::Lexer::Status::Ok);
        const yal::Lexer::TokenInfo& tk = lexer.getLastToken();
        EXPECT_EQ(tk.token, yal::Token::EndScope);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
