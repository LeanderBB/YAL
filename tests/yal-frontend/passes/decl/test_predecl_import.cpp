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

class PassDecl_PreDecl_Import : public CompileFixture {

};


TEST_F(PassDecl_PreDecl_Import, ImportFail) {

    const char* input =
R"R(
    import Foo::Bar;
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
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorImportNotFound::kCode);
}


TEST_F(PassDecl_PreDecl_Import, ImportSuccess) {

    const char* inputModuleFoo =
R"R(
    fn foo() {}
)R";

    const char* inputModuleBar =
R"R(
    import Foo;
)R";

    FrontendOptionsType options;

    auto handleFoo = createSourceHandle(inputModuleFoo, "Foo");
    const ModuleType* moduleFoo = m_frontEnd.compile(handleFoo, options);
    EXPECT_NE(nullptr, moduleFoo);
    EXPECT_FALSE(m_errorReporter.hasErrors());

    auto handleBar = createSourceHandle(inputModuleBar,"Bar");
    const ModuleType* moduleBar = m_frontEnd.compile(handleBar, options);
    EXPECT_NE(nullptr, moduleBar);
    EXPECT_FALSE(m_errorReporter.hasErrors());
}

TEST_F(PassDecl_PreDecl_Import, CallImportedFn) {

    const char* inputModuleFoo =
R"R(
    fn foo() {}
)R";

    const char* inputModuleBar =
R"R(
    import Foo;

    fn main() {
       Foo::foo();
    }
)R";

    FrontendOptionsType options;

    auto handleFoo = createSourceHandle(inputModuleFoo, "Foo");
    const ModuleType* moduleFoo = m_frontEnd.compile(handleFoo, options);
    EXPECT_NE(nullptr, moduleFoo);
    EXPECT_FALSE(m_errorReporter.hasErrors());

    auto handleBar = createSourceHandle(inputModuleBar,"Bar");
    const ModuleType* moduleBar = m_frontEnd.compile(handleBar, options);
    EXPECT_NE(nullptr, moduleBar);
    EXPECT_FALSE(m_errorReporter.hasErrors());
}


TEST_F(PassDecl_PreDecl_Import, ImportedType) {

    const char* inputModuleFoo =
R"R(
    type Foo struct {
        x : u32
    }
)R";

    const char* inputModuleBar =
R"R(
    import Foo;

    fn main() {
       var x = Foo::Foo{ x: 30 };
    }
)R";

    FrontendOptionsType options;

    auto handleFoo = createSourceHandle(inputModuleFoo, "Foo");
    const ModuleType* moduleFoo = m_frontEnd.compile(handleFoo, options);
    EXPECT_NE(nullptr, moduleFoo);
    EXPECT_FALSE(m_errorReporter.hasErrors());

    auto handleBar = createSourceHandle(inputModuleBar,"Bar");
    const ModuleType* moduleBar = m_frontEnd.compile(handleBar, options);
    EXPECT_NE(nullptr, moduleBar);
    EXPECT_FALSE(m_errorReporter.hasErrors());
}
