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


#include "fixture.h"


TEST_F(CompileFixture, move_use_after_assign) {
    const char* str = R"R(
      type Bar : struct {
          x : i32;
      }

      type Foo : struct {
          b: Bar;
      }

      fn Foo::create(i:i32) : Foo{
          return Foo { b:Bar{ x:i}};
      }

      fn main() {
         var f:Foo = Foo{b:Bar{x:20}};
         var f2:Foo = f;
         var other:Bar = f.b;
      }

)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_EQ(module, nullptr);
}

TEST_F(CompileFixture, move_use_after_function_call) {
    const char* str = R"R(
      type Bar : struct {
          x : i32;
      }

      type Foo : struct {
          b: Bar;
      }

      fn Foo::create(i:i32) : Foo{
          return Foo { b:Bar{ x:i}};
      }

      fn doSomething(f:Foo) {

      }

      fn main() {
         var f:Foo = Foo{b:Bar{x:20}};

         var f2:Foo = f;

         f = Foo::create(40);

         doSomething(f);

         var f3:Foo = f;
      }
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_EQ(module, nullptr);
}

TEST_F(CompileFixture, move_use_after_assign_paramvar) {
    const char* str = R"R(
      type Bar : struct {
          x : i32;
      }

      type Foo : struct {
          b: Bar;
      }

      fn Foo::create(i:i32) : Foo{
          return Foo { b:Bar{ x:i}};
      }

      fn doSomething(f:Foo) {
          var o:Foo = f;
          var i:i32 = f.b.x;
      }

      fn main() {
         var f:Foo = Foo{b:Bar{x:20}};
         doSomething(f);
      }
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_EQ(module, nullptr);
}

TEST_F(CompileFixture, move_use_after_assign_struct) {
    const char* str = R"R(
      type Bar : struct {
          x : i32;
      }

      type Foo : struct {
          b: Bar;
      }

      fn main() {
         var b:Bar = Bar{x:4};

         var f:Foo = Foo{b:b};

         var i:i32 = b.x;
      }

)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_EQ(module, nullptr);
}
