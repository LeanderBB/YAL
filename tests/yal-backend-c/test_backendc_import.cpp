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


#include "fixture.h"

class BackendC_Import : public CompileFixture {

};

TEST_F(BackendC_Import, BasicImport) {
    const char* inputFoo =
R"R(
    type Foo struct {
       x:u32
    }

    impl Foo {
        fn Test(&self){
        }
    }
)R";
    compile(inputFoo,"import/Foo.yal");
    EXPECT_FALSE(m_errorReporter.hasErrors());
    if (m_errorReporter.hasErrors()) {
        return;
    }
    const char* inputBar =
R"R(
    import import::Foo;

    type Bar struct {
       f:import::Foo::Foo
    }

    fn bar_main() {
       let b = Bar{f:import::Foo::Foo{ x: 30 }};
       b.f.Test();
    }
)R";

    compile(inputBar,"import/Bar.yal");
    EXPECT_FALSE(m_errorReporter.hasErrors());
}
