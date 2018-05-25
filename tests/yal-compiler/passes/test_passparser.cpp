/*
 *  Copyright 2018 by Leander Beernaert (leanderbb@gmail.com)
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

#include <fixture.h>

#include <yal/frontend/parser/errorparser.h>

class PassParser : public CompileFixture {

};

TEST_F(PassParser, Unexpected_Token) {

    const char* input =
R"R(
fn foo:(int:)

)R";

    auto handle = createSourceHandle(input);
    FrontendOptionsType options;
    const ModuleType* module = m_frontEnd.compile(handle, options);
    EXPECT_EQ(module, nullptr);
    EXPECT_TRUE(m_errorReporter.hasErrors());
    if (!m_errorReporter.hasErrors()) {
        return;
    }
    const yal::Error* err = m_errorReporter.getLastError();
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorParser::kCode);
}


TEST_F(PassParser, NestedScopes) {

    const char* input =
R"R(
    fn foo() {
       var x:i32 = 0;
       {
          var y:i32 = x;
          {
            var z:i32 = x + 23i32;
          }
       }
    }
)R";

    auto handle = createSourceHandle(input);
    FrontendOptionsType options;
    const ModuleType* module = m_frontEnd.compile(handle, options);
    EXPECT_NE(module, nullptr);
    EXPECT_FALSE(m_errorReporter.hasErrors());
}
