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

class PassDecl_AstBuilder : public CompileFixture {

};

TEST_F(PassDecl_AstBuilder, DeclStructUndefinedType) {
    const char* input =
R"R(
    type Foo : struct {
    y: Bar
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
    EXPECT_EQ(yal::frontend::ErrorUndefinedTypeRef::kCode, err->getCode());

}

TEST_F(PassDecl_AstBuilder, DeclStructDuplicateSym) {
    const char* input =
R"R(
    type Foo : struct {
    y: i8,
    y: bool
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
    EXPECT_EQ(yal::frontend::ErrorDuplicateSymbol::kCode, err->getCode());
    if (err->getCode() == yal::frontend::ErrorDuplicateSymbol::kCode) {
        const yal::frontend::ErrorDuplicateSymbol* errImpl =
                static_cast<const yal::frontend::ErrorDuplicateSymbol*>(err);
        EXPECT_EQ(errImpl->m_sym1, yal::StringRef("y"));
        EXPECT_EQ(errImpl->m_sym2, yal::StringRef("y"));
    }
}

TEST_F(PassDecl_AstBuilder, DeclFnUndefinedReturnType) {
    const char* input =
R"R(
    fn Foo() : Bar {

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
    EXPECT_EQ(yal::frontend::ErrorUndefinedTypeRef::kCode, err->getCode());
    if (err->getCode() == yal::frontend::ErrorUndefinedTypeRef::kCode) {
        const yal::frontend::ErrorUndefinedTypeRef* errImpl =
                static_cast<const yal::frontend::ErrorUndefinedTypeRef*>(err);
        EXPECT_EQ(errImpl->m_typeName, yal::StringRef("Bar"));
    }
}

TEST_F(PassDecl_AstBuilder, DeclFnUndefinedParamType) {
    const char* input =
R"R(
    fn Foo(x:Bar) {

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
    EXPECT_EQ(yal::frontend::ErrorUndefinedTypeRef::kCode, err->getCode());
    if (err->getCode() == yal::frontend::ErrorUndefinedTypeRef::kCode) {
        const yal::frontend::ErrorUndefinedTypeRef* errImpl =
                static_cast<const yal::frontend::ErrorUndefinedTypeRef*>(err);
        EXPECT_EQ(errImpl->m_typeName, yal::StringRef("Bar"));
    }
}

TEST_F(PassDecl_AstBuilder, DeclFnInvalidUseOfSelf) {
    const char* input =
R"R(
    fn Foo(&self) {

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
    EXPECT_EQ(yal::frontend::ErrorInvalidUseOfSelf::kCode, err->getCode());
}


TEST_F(PassDecl_AstBuilder, DeclFnInvalidUseOfSelfNotFirstParam) {
    const char* input =
R"R(
    type Bar : struct {
         x:bool
    }

    fn Bar::Foo(x: bool, &self) {

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
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorInvalidLocationForSelf::kCode);
}

TEST_F(PassDecl_AstBuilder, DeclFnDuplicateParam) {
    const char* input =
R"R(
    type Bar : struct {
         x:bool
    }

    fn Bar::Foo(x:i8, x:bool) {

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
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorDuplicateSymbol::kCode);
    if (err->getCode() == yal::frontend::ErrorDuplicateSymbol::kCode) {
        const yal::frontend::ErrorDuplicateSymbol* errImpl =
                static_cast<const yal::frontend::ErrorDuplicateSymbol*>(err);
        EXPECT_EQ(errImpl->m_sym1, yal::StringRef("x"));
    }
}

TEST_F(PassDecl_AstBuilder, DeclVarUndefinedType) {
    const char* input =
R"R(
    fn main() {
        var x:Foo = Foo{y:20};
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
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorUndefinedTypeRef::kCode);
    if (err->getCode() == yal::frontend::ErrorUndefinedTypeRef::kCode) {
        const yal::frontend::ErrorUndefinedTypeRef* errImpl =
                static_cast<const yal::frontend::ErrorUndefinedTypeRef*>(err);
        EXPECT_EQ(errImpl->m_typeName, yal::StringRef("Foo"));
    }
}

TEST_F(PassDecl_AstBuilder, DeclVarDuplicateDecl) {
    const char* input =
R"R(
    fn main() {
        var x:i8 = 20;
        var x:i8 = 40;
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
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorDuplicateSymbol::kCode);
    if (err->getCode() == yal::frontend::ErrorDuplicateSymbol::kCode) {
        const yal::frontend::ErrorDuplicateSymbol* errImpl =
                static_cast<const yal::frontend::ErrorDuplicateSymbol*>(err);
        EXPECT_EQ(errImpl->m_sym1, yal::StringRef("x"));
    }
}

TEST_F(PassDecl_AstBuilder, DeclVarDuplicateDeclParam) {
    const char* input =
R"R(
    fn main(x:i8) {
        var x:i8 = 20;
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
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorDuplicateSymbol::kCode);
    if (err->getCode() == yal::frontend::ErrorDuplicateSymbol::kCode) {
        const yal::frontend::ErrorDuplicateSymbol* errImpl =
                static_cast<const yal::frontend::ErrorDuplicateSymbol*>(err);
        EXPECT_EQ(errImpl->m_sym1, yal::StringRef("x"));
    }
}

TEST_F(PassDecl_AstBuilder, ExprVarRefUndefinedRef) {
    const char* input =
R"R(
    fn main() {
        var x:i8 = y + 20;
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
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorUndefinedSymbol::kCode);
    if (err->getCode() == yal::frontend::ErrorUndefinedSymbol::kCode) {
        const yal::frontend::ErrorUndefinedSymbol* errImpl =
                static_cast<const yal::frontend::ErrorUndefinedSymbol*>(err);
        EXPECT_EQ(errImpl->m_sym, yal::StringRef("y"));
    }
}

TEST_F(PassDecl_AstBuilder, InvalidIntegerLiteral) {
    const char* input =
R"R(
    fn main() {
        var x:i8 = 232432432420480294809324809234092340923482340;
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
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorInvalidIntLiteral::kCode);
}

TEST_F(PassDecl_AstBuilder, InvalidIntegerLiterali8) {
    const char* input =
R"R(
    fn main() {
        var x:i8 = 256i8;
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
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorInvalidIntLiteral::kCode);
    const yal::frontend::ErrorInvalidIntLiteral* intError =
            static_cast<const yal::frontend::ErrorInvalidIntLiteral*>(err);
    EXPECT_EQ(yal::StringRef("256"), intError->m_expr.getValue());
}

TEST_F(PassDecl_AstBuilder, InvalidIntegerLiterali8Neg) {
    const char* input =
R"R(
    fn main() {
        var x:i8 = -256i8;
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
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorInvalidIntLiteral::kCode);
    const yal::frontend::ErrorInvalidIntLiteral* intError =
            static_cast<const yal::frontend::ErrorInvalidIntLiteral*>(err);
    EXPECT_EQ(yal::StringRef("-256"), intError->m_expr.getValue());
}


TEST_F(PassDecl_AstBuilder, InvalidIntegerLiterali8Valid) {
    const char* input =
R"R(
    fn main() {
        var x:i8 = -16i8;
    }
)R";
    auto handle = createSourceHandle(input);
    FrontendOptionsType options;
    const ModuleType* module = m_frontEnd.compile(handle, options);
    EXPECT_NE(module, nullptr);
    EXPECT_FALSE(m_errorReporter.hasErrors());
}

TEST_F(PassDecl_AstBuilder, InvalidIntegerLiteralu8Valid) {
    const char* input =
R"R(
    fn main() {
        var x:u8 = 255u8;
    }
)R";
    auto handle = createSourceHandle(input);
    FrontendOptionsType options;
    const ModuleType* module = m_frontEnd.compile(handle, options);
    EXPECT_NE(module, nullptr);
    EXPECT_FALSE(m_errorReporter.hasErrors());
}

TEST_F(PassDecl_AstBuilder, InvalidIntegerLiteralu8InValid) {
    const char* input =
R"R(
    fn main() {
        var x:u8 = -20u8;
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
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorInvalidIntLiteral::kCode);
    const yal::frontend::ErrorInvalidIntLiteral* intError =
            static_cast<const yal::frontend::ErrorInvalidIntLiteral*>(err);
    EXPECT_EQ(yal::StringRef("-20"), intError->m_expr.getValue());
}

TEST_F(PassDecl_AstBuilder, FunctionOnTargetNotDefined) {
    const char* input =
R"R(
    type Foo: struct {
        x:bool
    }

    fn main() {
        var f:Foo = Foo{x:true};
        Foo::bar(&f);
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
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorTypeFunctionUndefined::kCode);
    if (err->getCode() == yal::frontend::ErrorTypeFunctionUndefined::kCode) {
        const yal::frontend::ErrorTypeFunctionUndefined* errImpl =
                static_cast<const yal::frontend::ErrorTypeFunctionUndefined*>(err);
        EXPECT_EQ(errImpl->m_sym, yal::StringRef("bar"));
    }
}

TEST_F(PassDecl_AstBuilder, FunctionOnTargetIsNotStatic) {
    const char* input =
R"R(
    type Foo: struct {
        x:bool
    }

    fn Foo::bar(&self, f:&Foo) {

    }

    fn main() {
        var f:Foo = Foo{x:true};
        Foo::bar(&f);
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
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorTypeFunctionIsNotStatic::kCode);
}

TEST_F(PassDecl_AstBuilder, TypeIsNotFunction) {
    const char* input =
R"R(
    type Foo: struct {
        x:bool
    }

    fn main() {
        Foo(20);
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
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorTypeIsNotFunction::kCode);
}


TEST_F(PassDecl_AstBuilder, StructInitTypeNotStruct) {
    const char* input =
R"R(
    fn Foo() {

    }

    fn main() {
        var f:Foo = Foo {x:true};
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
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorTypeIsNotStruct::kCode);
}

TEST_F(PassDecl_AstBuilder, StructInitUndefinedMember) {
    const char* input =
R"R(
    type Foo : struct {
        y:bool
    }

    fn main() {
        var f:Foo = Foo {x:true};
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
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorUndefinedStructMember::kCode);
}


TEST_F(PassDecl_AstBuilder, StructMemberOrder) {
    const char* input =
R"R(
    type Foo : struct {
        x:bool,
        y:bool,
        z:bool
    }
)R";
    auto handle = createSourceHandle(input);
    FrontendOptionsType options;
    const ModuleType* module = m_frontEnd.compile(handle, options);
    EXPECT_NE(module, nullptr);
    EXPECT_FALSE(m_errorReporter.hasErrors());
    const yal::frontend::TypeContext& typeCtx = module->getTypeContext();

    yal::frontend::Identifier id("Foo",*module);
    const yal::frontend::Type* type = typeCtx.getByIdentifier(id);
    EXPECT_NE(nullptr, type);
    if (type == nullptr) {
        return;
    }

    const yal::frontend::TypeStruct* typeStruct =
            yal::dyn_cast<yal::frontend::TypeStruct>(type);
    EXPECT_NE(nullptr, typeStruct);

    if (typeStruct == nullptr) {
        return;
    }

    const yal::frontend::DeclStruct& decl = typeStruct->getDecl();
    const yal::frontend::DeclStruct::Members& members = decl.getMembers();

    EXPECT_EQ(3, members.size());

    EXPECT_EQ(yal::StringRef("x"), members[0]->getName());
    EXPECT_EQ(yal::StringRef("y"), members[1]->getName());
    EXPECT_EQ(yal::StringRef("z"), members[2]->getName());
}

TEST_F(PassDecl_AstBuilder, FunctionParamOrder) {
    const char* input =
R"R(
    fn foo(x:bool,
        y:bool,
        z:bool) {
    }
)R";
    auto handle = createSourceHandle(input);
    FrontendOptionsType options;
    const ModuleType* module = m_frontEnd.compile(handle, options);
    EXPECT_NE(module, nullptr);
    EXPECT_FALSE(m_errorReporter.hasErrors());
    const yal::frontend::TypeContext& typeCtx = module->getTypeContext();

    yal::frontend::Identifier id("foo",*module);
    const yal::frontend::Type* type = typeCtx.getByIdentifier(id);
    EXPECT_NE(nullptr, type);
    if (type == nullptr) {
        return;
    }

    const yal::frontend::TypeFunction* typeFn =
            yal::dyn_cast<yal::frontend::TypeFunction>(type);
    EXPECT_NE(nullptr, typeFn);

    if (typeFn == nullptr) {
        return;
    }

    const yal::frontend::DeclFunction& decl = typeFn->getDecl();
    const yal::frontend::DeclFunction::Params& params = decl.getParams();

    EXPECT_EQ(3, params.size());

    EXPECT_EQ(yal::StringRef("x"), params[0]->getName());
    EXPECT_EQ(yal::StringRef("y"), params[1]->getName());
    EXPECT_EQ(yal::StringRef("z"), params[2]->getName());
}

TEST_F(PassDecl_AstBuilder, FunctionStmtOrder) {
    const char* input =
R"R(
    fn foo() : bool {
        var y:bool = true;
        var x:bool = false;
        return y and x;
    }
)R";
    auto handle = createSourceHandle(input);
    FrontendOptionsType options;
    const ModuleType* module = m_frontEnd.compile(handle, options);
    EXPECT_NE(module, nullptr);
    EXPECT_FALSE(m_errorReporter.hasErrors());
    const yal::frontend::TypeContext& typeCtx = module->getTypeContext();

    yal::frontend::Identifier id("foo",*module);
    const yal::frontend::Type* type = typeCtx.getByIdentifier(id);
    EXPECT_NE(nullptr, type);
    if (type == nullptr) {
        return;
    }

    const yal::frontend::TypeFunction* typeFn =
            yal::dyn_cast<yal::frontend::TypeFunction>(type);
    EXPECT_NE(nullptr, typeFn);

    if (typeFn == nullptr) {
        return;
    }

    const yal::frontend::DeclFunction& decl = typeFn->getDecl();
    const yal::frontend::DeclFunction::Body& body = decl.getFunctionBody();

    EXPECT_EQ(3, body.size());

    EXPECT_EQ(yal::frontend::AstType::StmtDecl, body[0]->getAstType());
    EXPECT_EQ(yal::frontend::AstType::StmtDecl, body[1]->getAstType());
    EXPECT_EQ(yal::frontend::AstType::StmtReturn, body[2]->getAstType());

    if (const yal::frontend::StmtDecl* stmt =
            yal::dyn_cast<yal::frontend::StmtDecl>(body[0]);
            stmt != nullptr) {
        EXPECT_EQ(yal::StringRef("y"), stmt->getDecl()->getName());
    }

    if (const yal::frontend::StmtDecl* stmt =
            yal::dyn_cast<yal::frontend::StmtDecl>(body[1]);
            stmt != nullptr) {
        EXPECT_EQ(yal::StringRef("x"), stmt->getDecl()->getName());
    }
}


TEST_F(PassDecl_AstBuilder, VarRefOutsideScope) {
    const char* input =
R"R(
    fn main() {
        {
           var x:i32 = 20;
        }

        var y:i32 = x + 10;
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
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorUndefinedSymbol::kCode);
    if (err->getCode() == yal::frontend::ErrorUndefinedSymbol::kCode) {
        const yal::frontend::ErrorUndefinedSymbol* errImpl =
                static_cast<const yal::frontend::ErrorUndefinedSymbol*>(err);
        EXPECT_EQ(errImpl->m_sym, yal::StringRef("x"));
    }
}


TEST_F(PassDecl_AstBuilder, AssignRefOutOfScopeVar) {
    const char* input =
R"R(
    fn main() {
        var z:u32 = 0;
        var y:&u32 = &z;
        {
           var x:u32 = 20;
           y = &x;
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
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorAssignRefWithInvalidScope::kCode);
}

TEST_F(PassDecl_AstBuilder, AssignRefOutOfScopeStruct) {
    const char* input =
R"R(
    type Foo : struct {
        x : i32
    }

    fn main() {

        var z:i32 = 0;
        var y:&i32 = &z;
        {
           var f:Foo = Foo {x:20};
           y = &f.x;
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
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorAssignRefWithInvalidScope::kCode);
}


/* TODO: Can't handle hidden reference chains, improve this?
TEST_F(PassDecl_AstBuilder, AssignRefOutOfScopeVarRefChained) {
    const char* input =
R"R(
    fn main() {
        var z: mut i32 = 0;
        var y: mut& i32 = &z;
        {
           var x: mut i32 = 20;
           var w: mut& i32 = &x;
           y = w;
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
    EXPECT_EQ(err->getCode(), yal::frontend::ErrorAssignRefWithInvalidScope::kCode);
}
*/
