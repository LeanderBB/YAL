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

TEST_F(CompileFixture, function_self_use) {
    const char* str = R"R(
 fn Bar(a:float) : bool {
            return self.b == a;
}
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_errorReporter, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_EQ(module, nullptr);
}


TEST_F(CompileFixture, declscope_check_for_declvar) {
    const char* str = R"R(
      type Bar : struct {
          x : i32
      }

      type Foo : struct {
          b: Bar
      }

      fn main() {
         var f:Foo = Foo{b:Bar{x:f.b.x}};
      }
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_errorReporter, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_EQ(module, nullptr);
}

TEST_F(CompileFixture, reference_sum_of_primitive_val) {
    const char* str = R"R(
 fn test(a:&i32) {
    var b:i32 = a + 1;
}
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_errorReporter, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_NE(module, nullptr);
}

TEST_F(CompileFixture, reference_assing_to_const) {
    const char* str = R"R(
 fn test(a:&i32) {
    var i:i32 = 20;
    a = &i;
}
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_errorReporter, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_EQ(module, nullptr);
}

