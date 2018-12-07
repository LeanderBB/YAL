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
#include <yal/frontend/parser/errorparser.h>

class PassDecl_PreDecl_AliasWeak : public CompileFixture {

};
class PassDecl_PreDecl_AliasStrong : public CompileFixture {

};


TEST_F(PassDecl_PreDecl_AliasWeak, AliasDuplicate) {

    const char* input =
R"R(
    type Foo struct { x:u32}
    type Foo alias Foo;
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
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorDuplicateTypeDecl::kCode);
}

TEST_F(PassDecl_PreDecl_AliasWeak, AliasUndefined) {

    const char* input =
R"R(
    type Bar alias Foo;
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
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorUndefinedTypeRef::kCode);
}

TEST_F(PassDecl_PreDecl_AliasWeak, AliasOnFunction) {

    const char* input =
R"R(
    fn foo() {}

    type Bar alias foo;
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
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorAliasOfFunction::kCode);
}

TEST_F(PassDecl_PreDecl_AliasWeak, AliasOnTypeFunction) {

    const char* input =
R"R(
    type foo struct { b: bool }
    impl foo {
        fn Test() {}
    }
    type Bar alias foo::Test;
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
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorAliasOfFunction::kCode);
}

TEST_F(PassDecl_PreDecl_AliasWeak, AliasImplTypeFunction) {

    const char* input =
R"R(
    type foo struct { b: bool }
    type Bar alias foo;

    impl Bar {
        fn Test() {}
    }
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
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorFnImplOnAlias::kCode);
}


TEST_F(PassDecl_PreDecl_AliasStrong, DuplicateFunction) {

    const char* input =
R"R(
    type Foo struct { b: bool }
    type Bar from Foo;

    impl Foo {
        fn Test() {}
    }

    impl Bar {
        fn Test() {}
    }
)R";

    auto handle = createSourceHandle(input);
    FrontendOptionsType options;
    const ModuleType* module = m_frontEnd.compile(handle, options);
    EXPECT_EQ(nullptr, module);
    EXPECT_TRUE(m_errorReporter.hasErrors());
    if (!m_errorReporter.hasErrors()) {
        return;
    }
    const yal::Error* err = m_errorReporter.getLastError();
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorDuplicateTypeDecl::kCode);
}
