/*
 *  Copyright 2018 by Leander Beernaert (lbb-dev@pm.me)
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
#include <yal/frontend/passes/type/errorspasstype.h>
#include <yal/frontend/ast/astnodes.h>
#include <yal/frontend/types/typecontext.h>
#include <yal/frontend/types/typefunction.h>
#include <yal/frontend/types/typestruct.h>


class PassType_Alias : public CompileFixture {

};


TEST_F(PassType_Alias, AssignToAlias) {
    const char* input =
R"R(
    type MyInt alias u32;

    fn foo() {
        var x = 30;
        var z:MyInt = x;
    }
)R";

    auto handle = createSourceHandle(input);
    FrontendOptionsType options;
    const ModuleType* module = m_frontEnd.compile(handle, options);
    EXPECT_NE(module, nullptr);
    EXPECT_FALSE(m_errorReporter.hasErrors());
}


TEST_F(PassType_Alias, AssignFromAlias) {
    const char* input =
R"R(
    type MyInt alias u32;

    fn foo() {
        var z:MyInt = 40;
        var y:u32 = z;
    }
)R";

    auto handle = createSourceHandle(input);
    FrontendOptionsType options;
    const ModuleType* module = m_frontEnd.compile(handle, options);
    EXPECT_NE(module, nullptr);
    EXPECT_FALSE(m_errorReporter.hasErrors());
}

TEST_F(PassType_Alias, FnCall) {
    const char* input =
R"R(
    type MyInt alias u32;

    fn test(x:MyInt) {
    }

    fn testRef(x:&MyInt) {
    }

    fn foo() {
        var z:MyInt = 40;
        testRef(&z);
        test(z);
        var x = 30;
        testRef(&x);
        test(x);
    }
)R";

    auto handle = createSourceHandle(input);
    FrontendOptionsType options;
    const ModuleType* module = m_frontEnd.compile(handle, options);
    EXPECT_NE(module, nullptr);
    EXPECT_FALSE(m_errorReporter.hasErrors());
}


TEST_F(PassType_Alias, Struct) {
    const char* input =
R"R(

    type Foo struct {
         x:u32
    }

    type Bar alias Foo;

    fn foo() {
       var f = Bar { x:40 };
       var z = f.x + 40;
    }
)R";

    auto handle = createSourceHandle(input);
    FrontendOptionsType options;
    const ModuleType* module = m_frontEnd.compile(handle, options);
    EXPECT_NE(module, nullptr);
    EXPECT_FALSE(m_errorReporter.hasErrors());
}


TEST_F(PassType_Alias, StructStaticCall) {
    const char* input =
R"R(

    type Foo struct {
         x:u32
    }

    impl Foo {
        fn Test(){
        }
    }

    type Bar alias Foo;

    fn foo() {
       Bar::Test();
    }
)R";

    auto handle = createSourceHandle(input);
    FrontendOptionsType options;
    const ModuleType* module = m_frontEnd.compile(handle, options);
    EXPECT_NE(module, nullptr);
    EXPECT_FALSE(m_errorReporter.hasErrors());
}
