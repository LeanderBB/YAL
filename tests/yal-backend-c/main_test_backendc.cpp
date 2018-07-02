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

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


class General : public CompileFixture {

};

TEST_F(General, StructAccess) {
    const char* input =
R"R(
    type Bar struct {
        x : u32
    }

    type Foo struct {
        b: Bar
    }

    impl Foo {

        fn static(x:u32) {

        }

        fn instance(&self) : bool {
            return self.b.x == 20;
        }

        fn setX(mut& self, newX:u32) {
            self.b.x = newX;
        }
    }

    fn other(x: mut Foo) {
        var z:u32 = x.b.x;
    }

    fn main() {
        var f:mut Foo = Foo{b:Bar{x:20}};
        f.b = Bar { x:10};
        f.setX(40);
        other(f);
    }
)R";
    compile(input,"general/structaccess.yal");
    EXPECT_FALSE(m_errorReporter.hasErrors());
}


TEST_F(General, RefTest) {
    const char* input =
R"R(
    type Bar struct {
        x : u32
    }


    type Foo struct {
        b: mut Bar
    }



    fn test(f: mut& Foo) {
        f.b = Bar { x: 30 };
    }

    fn assign(f: mut& Foo) {
       // Should not be allowed
       //f = Foo{b:Bar{x:20}};
    }

    impl Foo {
        fn create(i:u32) : mut Foo{
            return Foo { b:Bar{ x:i}};
        }

        fn createWithBar(b: mut Bar) : mut Foo{
            return Foo { b:b};
        }
    }

    fn moveHere(f:mut Foo) {

    }

    fn move(f:mut Foo) {
      test(&f);
      moveHere(f);
    }



    fn main() {
       var f = Foo{b:Bar{x:20}};
       test(&f);
       f.b = Bar { x:10};

       var f2:Foo = Foo::create(30);
       var o:&Foo = &f2;

       move(f);
       var other:mut Bar = Bar { x: 20 };
       f = Foo::createWithBar(other);
    }
)R";
    compile(input,"general/reftest.yal");
    EXPECT_FALSE(m_errorReporter.hasErrors());
}

TEST_F(General, Cast) {
    const char* input =
R"R(
    fn main() {
        var x = 20;
        var z:u32 = x as u32;
    }
)R";
    compile(input,"general/cast.yal");
    EXPECT_FALSE(m_errorReporter.hasErrors());
}

TEST_F(General, RefAndDeref) {
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
    compile(input,"general/refAndDeref.yal");
    EXPECT_FALSE(m_errorReporter.hasErrors());
}
