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



int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
