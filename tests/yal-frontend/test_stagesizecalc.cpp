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


TEST_F(CompileFixture, struct_size_2_integer) {
    const char* str = R"R(
    type Foo : struct {
         x: i32,
         y: i32
    }
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_errorReporter, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_NE(module, nullptr);
    if (module != nullptr) {

        const yal::Type* structType
                = module->getAstTypeContext().getByIdentifier(yal::Identifier("Foo", *module));
        EXPECT_NE(structType, nullptr);
        if (structType != nullptr) {
            EXPECT_EQ(structType->getSizeBytes(), 8);
        }
    }
}


TEST_F(CompileFixture, struct_size_4i8_integer) {
    const char* str = R"R(
    type Foo : struct {
        x0: i8,
        x1: i8,
        x2: i8,
        x3: i8,
        y: i32
    }
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_errorReporter, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_NE(module, nullptr);
    if (module != nullptr) {

        const yal::Type* structType
                = module->getAstTypeContext().getByIdentifier(yal::Identifier("Foo", *module));
        EXPECT_NE(structType, nullptr);
        if (structType != nullptr) {
            EXPECT_EQ(structType->getSizeBytes(), 8);
        }
    }
}

TEST_F(CompileFixture, struct_size_ref_and_integer) {
    const char* str = R"R(
    type Foo : struct {
         x: &i32,
         y: i32
    }
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_errorReporter, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_NE(module, nullptr);
    if (module != nullptr) {

        const yal::Type* structType
                = module->getAstTypeContext().getByIdentifier(yal::Identifier("Foo", *module));
        EXPECT_NE(structType, nullptr);
        if (structType != nullptr) {
            EXPECT_EQ(structType->getSizeBytes(), 16);
        }
    }
}

TEST_F(CompileFixture, struct_size_integer_and_ref) {
    const char* str = R"R(
    type Foo : struct {
         x: i32,
         y: &i32
    }
)R";

    auto handle = createSourceHanlde(str);
    yal::Compiler compiler(*m_log, m_errorReporter, m_sourceManager, m_moduleManager);
    yal::Module* module = compiler.compile(handle);
    EXPECT_NE(module, nullptr);
    if (module != nullptr) {

        const yal::Type* structType
                = module->getAstTypeContext().getByIdentifier(yal::Identifier("Foo", *module));
        EXPECT_NE(structType, nullptr);
        if (structType != nullptr) {
            EXPECT_EQ(structType->getSizeBytes(), 16);
        }
    }
}

