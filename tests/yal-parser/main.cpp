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


TEST_F(CompileFixture, function_no_params_no_return) {
    const char* str = R"R(
 fn Foo() {

}
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_sourceManager, m_moduleManager);
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
    yal::Compiler compiler(*m_log, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_NE(module, nullptr);
}


TEST_F(CompileFixture, function_params_no_return) {
    const char* str = R"R(
 fn Foo(a:bool, b:mut& i32) {
}
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_sourceManager, m_moduleManager);
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
    yal::Compiler compiler(*m_log, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_NE(module, nullptr);
}


TEST_F(CompileFixture, type_function_unknown_type) {
    const char* str = R"R(
 fn Foo::Bar() : bool {
}
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_EQ(module, nullptr);
}


TEST_F(CompileFixture, function_unknown_type) {
    const char* str = R"R(
 fn Bar(a:Foo) : bool {
}
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_EQ(module, nullptr);
}

TEST_F(CompileFixture, function_self_use) {
    const char* str = R"R(
 fn Bar(a:float) : bool {
            return self.b == a;
}
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_EQ(module, nullptr);
}

TEST_F(CompileFixture, struct) {
    const char* str = R"R(
     type Foo : struct {
        bar: i32 = 0;
        other: bool = false;
     }
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_NE(module, nullptr);
}

TEST_F(CompileFixture, struct_unknown_member_type) {
    const char* str = R"R(
     type Foo : struct {
        bar: i32 = 0;
        other: Bar = Bar();
     }
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_EQ(module, nullptr);
}

TEST_F(CompileFixture, type_function_static) {
    const char* str = R"R(
     type Foo : struct {
        bar: i32 = 0;
        other: bool = false;
     }

     fn Foo::static(instance:&Foo) : bool{
        return instance.bar != 0;
    }
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_NE(module, nullptr);
}


TEST_F(CompileFixture, type_function_instance) {
    const char* str = R"R(
     type Foo : struct {
        bar: i32 = 0;
        other: bool = false;
     }

     fn Foo::static(&self) : bool{
        return self.bar >= 0;
    }
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_NE(module, nullptr);
}



TEST_F(CompileFixture, function_local_vars) {
    const char* str = R"R(
     fn static(b:i32, c:double) : double{
        var local:i32 = 20;
        local = local + b;
        return c * 2.0;
    }
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_NE(module, nullptr);
}


TEST_F(CompileFixture, function_local_vars_undefined) {
    const char* str = R"R(
     fn static(b:i32, c:double) : double{
        var local:i32 = 20;
        local = local + something_else;
        return c * local;
    }
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_EQ(module, nullptr);
}

TEST_F(CompileFixture, function_duplicate) {
    const char* str = R"R(
     fn static(b:i32, c:double) : double{
        var local:i32 = 20;
        local = local + b;
        return c * local;
    }

    fn static() {

    }
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_EQ(module, nullptr);
}

TEST_F(CompileFixture, type_function_duplicate) {
    const char* str = R"R(

     type Foo:struct {
            bar:i32 = 0;
     }

     fn Foo::static(c:double) : double{
        return c;
    }

    fn Foo::static() {

    }
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_EQ(module, nullptr);
}

TEST_F(CompileFixture, struct_member_init_missing) {
    const char* str = R"R(

     type Foo:struct {
            bar:i32 = 0;
            other: bool;
     }

    fn Foo::static() : Foo {
        var test:Foo = Foo {};
    }
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_EQ(module, nullptr);
}


TEST_F(CompileFixture, reference_assign_fail) {
    const char* str = R"R(
      type Bar : struct {
          x : i32;
      }

      type Foo : struct {
          b: Bar;
      }

      fn assign(f: mut& Foo) {
         f = Foo{b:Bar{x:20}};
      }
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_EQ(module, nullptr);
}


TEST_F(CompileFixture, reference_missing_cast) {
    const char* str = R"R(
      type Bar : struct {
          x : i32;
      }

      type Foo : struct {
          b: Bar;
      }

      fn test(f: mut& Foo) {
          f.b = Bar { x: 30 };
      }

      fn main() {
         var f:mut Foo = Foo{b:Bar{x:20}};
         test(f);
         f.b = Bar { x:10};

         var other:Bar = f.b;
      }
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_EQ(module, nullptr);
}


TEST_F(CompileFixture, reference_with_cast) {
    const char* str = R"R(
      type Bar : struct {
          x : i32;
      }

      type Foo : struct {
          b: Bar;
      }

      fn test(f: mut& Foo) {
          f.b = Bar { x: 30 };
      }

      fn main() {
         var f:mut Foo = Foo{b:Bar{x:20}};
         test(&f);
         f.b = Bar { x:10};
      }
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_NE(module, nullptr);
}


TEST_F(CompileFixture, reference_replace_required) {
    const char* str = R"R(
      type Bar : struct {
          x : i32;
      }

      type Foo : struct {
          b: Bar;
      }

      fn main() {
         var f:mut Foo = Foo{b:Bar{x:20}};
         f.b = Bar { x:10};
         var other:Bar = f.b;
      }
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_EQ(module, nullptr);
}

TEST_F(CompileFixture, fntype_function_static_call) {
    const char* str = R"R(
      type Bar : struct {
          x : i32;
      }

      type Foo : struct {
          b: Bar;
      }

      fn Foo::create(i:i32) : mut Foo {
          return Foo { b:Bar{ x:i}};
      }

      fn main() {
         var f:mut Foo = Foo::create(20);
      }
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_NE(module, nullptr);
}

TEST_F(CompileFixture, fntype_function_static_call_on_instance) {
    const char* str = R"R(
      type Bar : struct {
          x : i32;
      }

      type Foo : struct {
          b: Bar;
      }

      fn Foo::create(i:i32) : mut Foo {
          return Foo { b:Bar{ x:i}};
      }

      fn main() {
         var f:mut Foo = Foo::create(20);
         var o:Foo = f.create(40);
      }
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_EQ(module, nullptr);
}

TEST_F(CompileFixture, fntype_function_instance_call_as_static) {
    const char* str = R"R(
      type Bar : struct {
          x : i32;
      }

      type Foo : struct {
          b: Bar;
      }

      fn Foo::create(i:i32) : mut Foo {
          return Foo { b:Bar{ x:i}};
      }

      fn Foo::setVal(mut& self, v:i32) {
           self.b.x = v;
      }

      fn main() {
         var f:mut Foo = Foo::create(20);
         f.setVal(30);
         Foo::setVal(40);
      }
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_EQ(module, nullptr);
}

TEST_F(CompileFixture, fntype_instance_call) {
    const char* str = R"R(
      type Bar : struct {
          x : i32;
      }

      type Foo : struct {
          b: Bar;
      }

      fn Foo::create(i:i32) : mut Foo {
          return Foo { b:Bar{ x:i}};
      }

      fn Foo::setVal(mut& self, v:i32) {
           self.b.x = v;
      }

      fn main() {
         var f:mut Foo = Foo::create(20);
         f.setVal(30);
      }
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_NE(module, nullptr);
}

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


TEST_F(CompileFixture, declscope_check_for_declvar) {
    const char* str = R"R(
      type Bar : struct {
          x : i32;
      }

      type Foo : struct {
          b: Bar;
      }

      fn main() {
         var f:Foo = Foo{b:Bar{x:f.b.x}};
      }
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_EQ(module, nullptr);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
