/*
 *  Copyright 2017 by Leander Beernaert (lbb-dev@pm.me)
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


#include "fixture.h"


TEST_F(CompileFixture, function_no_params_no_return) {
    const char* str = R"R(
 fn Foo() {

}
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_errorReporter, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_NE(module, nullptr);
}


TEST_F(CompileFixture, function_no_params_return) {
    const char* str = R"R(
 fn Foo() : bool {
    return true;
}
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_errorReporter, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_NE(module, nullptr);
}


TEST_F(CompileFixture, function_params_no_return) {
    const char* str = R"R(
 fn Foo(a:bool, b:mut& i32) {
}
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_errorReporter, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_NE(module, nullptr);
}

TEST_F(CompileFixture, function_params_return) {
    const char* str = R"R(
 fn Foo(a:bool, b:mut& i32) : bool {
    return (b + 1 > 20) and a == true;
}
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_errorReporter, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_NE(module, nullptr);
}
