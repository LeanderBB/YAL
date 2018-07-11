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
#include <yal/frontend/ast/astnodes.h>
#include <yal/frontend/parser/stexprliterals.h>
#include <yal/frontend/types/typecontext.h>
#include <yal/frontend/types/typefunction.h>
#include <yal/frontend/types/typestruct.h>

class PassDecl_AstBuilder_Import : public CompileFixture {

};

TEST_F(PassDecl_AstBuilder_Import, ImportedStructMember) {

    const char* inputModuleFoo =
R"R(
    type Foo struct {
       x:u32
    }

    impl Foo {
        fn Test(&self){
        }
    }
)R";

    const char* inputModuleBar =
R"R(
    import Foo;

    type Bar struct {
       f:Foo::Foo
    }

    fn bar_main() {
       let b = Bar{f:Foo::Foo{ x: 30 }};
       b.f.Test();
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
