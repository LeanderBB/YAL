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

#include <yal/frontend/passes/decl/errorspassdecl.h>

class PassDecl : public CompileFixture {

};

TEST_F(PassDecl, DuplicateType) {

    const char* input =
R"R(
    fn main() {
     var x = 30;

    }

    type main : struct {
        x:bool
    }
)R";

    auto handle = createSourceHandle(input);
    FrontendOptionsType options;
    const ModuleType* module = m_frontEnd.compile(handle, options);
    EXPECT_EQ(module, nullptr);
    EXPECT_TRUE(m_errorReporter.hasErrors());
    const yal::Error* err = m_errorReporter.getLastError();
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorDuplicateDecl::kCode);
}


TEST_F(PassDecl, DuplicateType_TypeFunction) {

    const char* input =
R"R(
    type Foo : struct {
        b: bool
    }


    fn Foo::abcd(&self) {
    }

    fn Foo::abcd(b:bool) {
    }
)R";

    auto handle = createSourceHandle(input);
    FrontendOptionsType options;
    const ModuleType* module = m_frontEnd.compile(handle, options);
    EXPECT_EQ(module, nullptr);
    EXPECT_TRUE(m_errorReporter.hasErrors());
    const yal::Error* err = m_errorReporter.getLastError();
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorDuplicateDecl::kCode);
}

TEST_F(PassDecl, Undefined_FunctionTarget) {

    const char* input =
R"R(
    fn Foo::abcd(b:bool) {
    }
)R";

    auto handle = createSourceHandle(input);
    FrontendOptionsType options;
    const ModuleType* module = m_frontEnd.compile(handle, options);
    EXPECT_EQ(module, nullptr);
    EXPECT_TRUE(m_errorReporter.hasErrors());
    const yal::Error* err = m_errorReporter.getLastError();
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorUndefinedTypeRef::kCode);
}
