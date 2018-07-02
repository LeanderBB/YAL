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

#include <yal/frontend/passes/move/errorspassmove.h>

class PassMove: public CompileFixture {

};


TEST_F(PassMove, UseStructAfterAssignMove) {
    const char* input =
R"R(
    type Bar : struct {
        x : u32
    }

    type Foo : struct {
        b: Bar
    }

    impl Foo {
        fn create(i:u32) : Foo{
            return Foo { b:Bar{ x:i}};
        }
    }

    fn main() {
       var f:Foo = Foo{b:Bar{x:20}};
       var f2:Foo = f;
       var other:Bar = f.b;
    }
)R";

    auto handle = createSourceHandle(input);
    FrontendOptionsType options;
    const ModuleType* module = m_frontEnd.compile(handle, options);
    EXPECT_EQ(module, nullptr);
    EXPECT_TRUE(m_errorReporter.hasErrors());
    if (m_errorReporter.hasErrors()) {
        const yal::Error* err = m_errorReporter.getLastError();
        EXPECT_EQ(yal::frontend::ErrorMoveUseAfterMove::kCode, err->getCode());
    }
}


TEST_F(PassMove, UseStructAfterFunctionMove) {
    const char* input =
R"R(
    type Foo : struct {
        x: u32
    }

    impl Foo {
        fn create(i:u32) : mut Foo{
            return Foo {x:i};
        }
    }

    fn doSomething(f:Foo) {
    }

    fn main() {
       var f: mut Foo = Foo{x:20};
       var f2:mut Foo = f;
       f = Foo::create(40);
       doSomething(f);
       var f3: mut Foo = f;
    }
)R";

    auto handle = createSourceHandle(input);
    FrontendOptionsType options;
    const ModuleType* module = m_frontEnd.compile(handle, options);
    EXPECT_EQ(module, nullptr);
    EXPECT_TRUE(m_errorReporter.hasErrors());
    if (m_errorReporter.hasErrors()) {
        const yal::Error* err = m_errorReporter.getLastError();
        EXPECT_EQ(yal::frontend::ErrorMoveUseAfterMove::kCode, err->getCode());
    }
}

TEST_F(PassMove, UseStructAfterAssignMoveFnParam) {
    const char* input =
R"R(
    type Bar : struct {
        x : u32
    }

    type Foo : struct {
        b: Bar
    }

    impl Foo {
        fn create(i:u32) : Foo{
            return Foo { b:Bar{ x:i}};
        }
    }

    fn doSomething(f:Foo) {
        let o:Foo = f;
        var i:u32 = f.b.x;
    }

    fn main() {
       var f:Foo = Foo{b:Bar{x:20}};
       doSomething(f);
    }
)R";

    auto handle = createSourceHandle(input);
    FrontendOptionsType options;
    const ModuleType* module = m_frontEnd.compile(handle, options);
    EXPECT_EQ(module, nullptr);
    EXPECT_TRUE(m_errorReporter.hasErrors());
    if (m_errorReporter.hasErrors()) {
        const yal::Error* err = m_errorReporter.getLastError();
        EXPECT_EQ(yal::frontend::ErrorMoveUseAfterMove::kCode, err->getCode());
    }
}

TEST_F(PassMove, UseStructAfterStructInitMove) {
    const char* input =
R"R(
    type Bar : struct {
        x : u32
    }

    type Foo : struct {
        b: Bar
    }

    fn main() {
       var b:mut Bar = Bar{x:4};
       var f:Foo = Foo{b:b};
       var i:u32 = b.x;
    }

)R";

    auto handle = createSourceHandle(input);
    FrontendOptionsType options;
    const ModuleType* module = m_frontEnd.compile(handle, options);
    EXPECT_EQ(module, nullptr);
    EXPECT_TRUE(m_errorReporter.hasErrors());
    if (m_errorReporter.hasErrors()) {
        const yal::Error* err = m_errorReporter.getLastError();
        EXPECT_EQ(yal::frontend::ErrorMoveUseAfterMove::kCode, err->getCode());
    }
}

TEST_F(PassMove, StructMoveNoReplace) {
    const char* input =
R"R(
    type Bar : struct {
        x : u32
    }

    type Foo : struct {
        b: Bar
    }

    fn main() {
       var b:mut Bar = Bar{x:4};
       var f:Foo = Foo{b:b};
       var c:Bar = f.b;
    }

)R";

    auto handle = createSourceHandle(input);
    FrontendOptionsType options;
    const ModuleType* module = m_frontEnd.compile(handle, options);
    EXPECT_EQ(module, nullptr);
    EXPECT_TRUE(m_errorReporter.hasErrors());
    if (m_errorReporter.hasErrors()) {
        const yal::Error* err = m_errorReporter.getLastError();
        EXPECT_EQ(yal::frontend::ErrorMoveStructVar::kCode, err->getCode());
    }
}


TEST_F(PassMove, StructMoveTriviallyCopiable) {
    const char* input =
R"R(
    type Bar : struct {
        x : u32
    }

    type Foo : struct {
        b: Bar
    }

    fn main() {
       var f:Foo = Foo{b:Bar{x:30}};
       var z:u32 = f.b.x;
    }

)R";

    auto handle = createSourceHandle(input);
    FrontendOptionsType options;
    const ModuleType* module = m_frontEnd.compile(handle, options);
    EXPECT_NE(module, nullptr);
    EXPECT_FALSE(m_errorReporter.hasErrors());
}

TEST_F(PassMove, MoveReInitMove) {
    const char* input =
R"R(
    type Foo : struct {
        x:u32
    }

    impl Foo {
        fn create(i:u32) : mut Foo{
            return Foo {x:i};
        }
    }

    fn main() {
       var f:mut Foo = Foo::create(20);
       var f2:mut Foo = f;
       f = Foo::create(40);
       var f3:mut Foo = f;
    }
)R";

    auto handle = createSourceHandle(input);
    FrontendOptionsType options;
    const ModuleType* module = m_frontEnd.compile(handle, options);
    EXPECT_NE(module, nullptr);
    EXPECT_TRUE(!m_errorReporter.hasErrors());
}


TEST_F(PassMove, MoveDerefAssignToRef) {
    const char* input =
R"R(
    fn foo(x:mut& u32) {
        *x = 40;
    }
    fn main() {
       var x = 20;
       foo(&x);
    }
)R";

    auto handle = createSourceHandle(input);
    FrontendOptionsType options;
    const ModuleType* module = m_frontEnd.compile(handle, options);
    EXPECT_NE(module, nullptr);
    EXPECT_TRUE(!m_errorReporter.hasErrors());
}

TEST_F(PassMove, MoveDerefAssingTrivialCopiableRefToLocal) {
    const char* input =
R"R(
    fn foo(x:mut& u32) {
        var z = *x;
    }
    fn main() {
       var x = 20;
       foo(&x);
    }
)R";

    auto handle = createSourceHandle(input);
    FrontendOptionsType options;
    const ModuleType* module = m_frontEnd.compile(handle, options);
    EXPECT_NE(module, nullptr);
    EXPECT_TRUE(!m_errorReporter.hasErrors());
}


TEST_F(PassMove, MoveDerefAssingNonTrivialCopiableRefToLocal) {
    const char* input =
R"R(
    type Foo : struct {
        x:u32
    }

    fn foo(x:mut& Foo) {
        var z:Foo = *x;
    }
    fn main() {
       var f = Foo {x:32};
       foo(&f);
    }
)R";

    auto handle = createSourceHandle(input);
    FrontendOptionsType options;
    const ModuleType* module = m_frontEnd.compile(handle, options);
    EXPECT_EQ(module, nullptr);
    EXPECT_TRUE(m_errorReporter.hasErrors());
    if (m_errorReporter.hasErrors()) {
        const yal::Error* err = m_errorReporter.getLastError();
        EXPECT_EQ(yal::frontend::ErrorMoveDereference::kCode, err->getCode());
    }
}
