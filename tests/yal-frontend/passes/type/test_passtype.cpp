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


class PassType : public CompileFixture {

};


TEST_F(PassType, StructMemberInitMissing) {
    const char* input =
R"R(
    type Foo:struct {
           bar:i32 ,
           other:bool
    }

    impl Foo {
       fn static() : Foo {
           var test:Foo = Foo {};
           return test;
       }
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
    EXPECT_EQ(yal::frontend::ErrorTypeStructInitNumMembers::kCode, err->getCode());
}

TEST_F(PassType, RefAssignFail) {
    const char* input =
R"R(
    type Foo:struct {
           other:bool
    }

   fn assign(f: mut& Foo) {
       f = Foo {other: false};
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
    EXPECT_EQ(yal::frontend::ErrorTypeReference::kCode, err->getCode());
}

TEST_F(PassType, RefMissingCast) {
    const char* input =
R"R(
    type Foo:struct {
           other:bool
    }

   fn test(f: mut& Foo) {
       f.other = true;
   }

   fn main() {
      var f:mut Foo = Foo{other:false};
      test(f);
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
    const yal::Error* err = m_errorReporter.getFirstError();
    EXPECT_EQ(yal::frontend::ErrorTypeReference::kCode, err->getCode());
}

TEST_F(PassType, FnTypeStaticCallOnInstance) {
    const char* input =
R"R(
    type Foo:struct {
           x:i32
    }

   impl Foo {
       fn test(i:i32) : mut Foo {
           return Foo {x:i};
       }
    }

   fn main() {
      var f:mut Foo = Foo::test(40);
      var o:Foo = f.test(30);
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
    EXPECT_EQ(yal::frontend::ErrorTypeIsNotTypeFunction::kCode, err->getCode());
}

TEST_F(PassType, FnTypeInstanceCallOnStatic) {
    const char* input =
R"R(
    type Foo:struct {
           x:i32
    }

    impl Foo {
       fn test(&self, i:i32) : bool {
           return self.x == i;
       }

       fn create(i:i32) : mut Foo {
           return Foo {x: i};
       }
    }

   fn main() {
      var f:mut Foo = Foo::create(40);
      f.test(30);
      Foo::test(40);
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
    EXPECT_EQ(yal::frontend::ErrorTypeFunctionIsNotStatic::kCode, err->getCode());
}

TEST_F(PassType, LValueAssignCheck) {
    const char* input =
R"R(
   fn main() {
      4 = 20;
    }
)R";

    auto handle = createSourceHandle(input);
    FrontendOptionsType options;
    const ModuleType* module = m_frontEnd.compile(handle, options);
    EXPECT_EQ(module, nullptr);
    EXPECT_TRUE(m_errorReporter.hasErrors());
    const yal::Error* err = m_errorReporter.getLastError();
    if (!m_errorReporter.hasErrors()) {
        return;
    }
    EXPECT_EQ(yal::frontend::ErrorTypeExprUnassignable::kCode, err->getCode());
}

TEST_F(PassType, LValueReturnAssign) {
    const char* input =
R"R(
   fn test() : u32 {
      return 20;
   }
   fn main() {
      test() = 30;
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
    const yal::Error* err = m_errorReporter.getFirstError();
    EXPECT_EQ(yal::frontend::ErrorTypeExprUnassignable::kCode, err->getCode());
}

TEST_F(PassType, LValueMakeRef) {
    const char* input =
R"R(
    fn main() {
       var i:mut& u32 = &30;
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
    EXPECT_EQ(yal::frontend::ErrorTypeUnaryOpRefNonLValue::kCode, err->getCode());
}

// TODO: Revise!!!
TEST_F(PassType, LValueFnReturnRefCheck) {
    const char* input =
R"R(
    fn test(x:mut& i32) : &i32 {
         return x;
    }

    fn main() {
       var i: mut i32 = 40i32;
       test(&i) = 456464i32;
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
    const yal::Error* err = m_errorReporter.getFirstError();
    EXPECT_EQ(yal::frontend::ErrorTypeExprUnassignable::kCode, err->getCode());
}

TEST_F(PassType, AssignToImmutable) {
    const char* input =
R"R(
    fn main() {
       let i: i32 = 40i32;
       i = 50i32;
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
    const yal::Error* err = m_errorReporter.getFirstError();
    EXPECT_EQ(yal::frontend::ErrorTypeAssignToImmutable::kCode, err->getCode());
}


TEST_F(PassType, SelfVarRef) {
    const char* input =
R"R(
    type Foo : struct {
          x:i32
    }

    impl Foo {
        fn test(&self) : bool {
            return self.x == 30i32;
        }

        fn stuff(&self) : bool {
            return self.test();
        }
    }

    fn main() {
       var f:Foo = Foo{x:20i32};
       f.stuff();
    }
)R";

    auto handle = createSourceHandle(input);
    FrontendOptionsType options;
    const ModuleType* module = m_frontEnd.compile(handle, options);
    EXPECT_NE(module, nullptr);
    EXPECT_FALSE(m_errorReporter.hasErrors());
}

TEST_F(PassType, SelfVarRefScoped) {
    const char* input =
R"R(
    type Foo : struct {
          x:i32
    }

    impl Foo {
        fn test(&self) : bool {
            {
                return self.x == 30i32;
            }
        }

        fn stuff(&self) : bool {
            {
                return self.test();
            }
        }
    }

    fn main() {
       var f:Foo = Foo{x:20i32};
       f.stuff();
    }
)R";

    auto handle = createSourceHandle(input);
    FrontendOptionsType options;
    const ModuleType* module = m_frontEnd.compile(handle, options);
    EXPECT_NE(module, nullptr);
    EXPECT_FALSE(m_errorReporter.hasErrors());
}

TEST_F(PassType, SelfVarRefScopedIndirect) {
    const char* input =
R"R(
    type Foo : struct {
          x:i32
    }

    impl Foo {
        fn test(&self) : bool {
            {
                let f:&Foo  = self;
                return f.x == 30i32;
            }
        }

        fn stuff(&self) : bool {
            {
                let f:&Foo  = self;
                return f.test();
            }
        }
    }

    fn main() {
       var f:Foo = Foo{x:20i32};
       f.stuff();
    }
)R";

    auto handle = createSourceHandle(input);
    FrontendOptionsType options;
    const ModuleType* module = m_frontEnd.compile(handle, options);
    EXPECT_NE(module, nullptr);
    EXPECT_FALSE(m_errorReporter.hasErrors());
}


TEST_F(PassType, TypeFnCallMutability) {
    const char* input =
R"R(
    type Foo : struct {
          x:i32
    }

    impl Foo {
        fn stuff(&self) : bool {
            return self.x != 0i32;
        }

        fn setX(mut& self, x:i32) {
            self.x = x;
        }
    }
    fn main() {
       let f:Foo = Foo{x:20i32};
       f.stuff();
       f.setX(40i32);
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
    const yal::Error* err = m_errorReporter.getFirstError();
    EXPECT_EQ(yal::frontend::ErrorTypeTypeFnCallImmutable::kCode, err->getCode());
}


TEST_F(PassType, TypeCast) {
    const char* input =
R"R(
    fn main() {
       var x:u32 = 20u32;
       var z:i32 = x as i32;
    }
)R";

    auto handle = createSourceHandle(input);
    FrontendOptionsType options;
    const ModuleType* module = m_frontEnd.compile(handle, options);
    EXPECT_NE(module, nullptr);
    EXPECT_FALSE(m_errorReporter.hasErrors());;
}

TEST_F(PassType, TypeCastFail) {
    const char* input =
R"R(
    type Foo : struct {
          x:i32
    }

    fn main() {
       var x:u32 = 20u32;
       var z:i32 = x as Foo;
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
    const yal::Error* err = m_errorReporter.getFirstError();
    EXPECT_EQ(yal::frontend::ErrorTypeIncompatibleCast::kCode, err->getCode());
}


TEST_F(PassType, CastNonTrivialType) {
    const char* input =
R"R(
    type Foo : struct {
        x : u32
    }

    fn main() {
        let f:Foo = Foo{x:20};
        let b:Foo = f as Foo;
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
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorTypeCastReference::kCode);
}

TEST_F(PassType, CastNonTrivialTypeAsReference) {
    const char* input =
R"R(
    type Foo : struct {
        x : u32
    }

    fn main() {
        let f:Foo = Foo{x:20};
        let b:&Foo = &f as &Foo;
    }

)R";
    auto handle = createSourceHandle(input);
    FrontendOptionsType options;
    const ModuleType* module = m_frontEnd.compile(handle, options);
    EXPECT_NE(module, nullptr);
    EXPECT_FALSE(m_errorReporter.hasErrors());
}

TEST_F(PassType, CastTrivialTypeAsReference) {
    const char* input =
R"R(
    type Foo : struct {
        x : i32
    }

    fn main() {
        var x:u32 = 20;
        var b:&u32 = &x as &u32;
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
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorTypeCastReference::kCode);
}

TEST_F(PassType, TypeInferenceImutable) {
    const char* input =
R"R(
    fn main() {
        let x = 20;
        x = 30;
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
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorTypeAssignToImmutable::kCode);
}


TEST_F(PassType, TypeInferenceMutable) {
    const char* input =
R"R(
    fn main() {
        var x = 20;
        x = 30;
    }
)R";
    auto handle = createSourceHandle(input);
    FrontendOptionsType options;
    const ModuleType* module = m_frontEnd.compile(handle, options);
    EXPECT_NE(module, nullptr);
    EXPECT_FALSE(m_errorReporter.hasErrors());
}

TEST_F(PassType, DerefOfNonReference) {
    const char* input =
R"R(
    fn main() {
        var x = 20;
        var z = *x;
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
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorTypeUnaryOpDerefNonRef::kCode);
}

TEST_F(PassType, DerefOfNonReferenceLiteral) {
    const char* input =
R"R(
    fn main() {
        var z = *30;
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
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorTypeUnaryOpDerefNonRef::kCode);
}

