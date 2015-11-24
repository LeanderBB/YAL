#include <yal/yal.h>
#include <yal/compiler/compiler.h>
#include <yal/util/filesink.h>
#include <yal/util/stringinputsink.h>
#include <yal/util/memorysink.h>
#include <cstring>
#include <string>
#include <gtest/gtest.h>
#include <yal/symbols/semanticexception.h>
#include <yalvm/yalvm.h>

static const std::string s_test_path = YAL_TEST_SOURCES_PATH;

extern "C"
{

void*
yalvm_malloc(size_t)
{
    YAL_ASSERT(false && "Should not be called, resolves linker dependency");
    return NULL;
}

void
yalvm_free(void*)
{
    YAL_ASSERT(false && "Should not be called, resolves linker dependency");
}

void
yalvm_memset(void*, int, yalvm_size)
{
    YAL_ASSERT(false && "Should not be called, resolves linker dependency");
}

void
yalvm_memcpy(void*, const void*, yalvm_size)
{
    YAL_ASSERT(false && "Should not be called, resolves linker dependency");
}

void
yalvm_print(yalvm_ctx *,
            const char *)
{
    YAL_ASSERT(false && "Should not be called, resolves linker dependency");
}

int
yalvm_snprintf(char *,
               yalvm_size,
               const char *
               ...)
{
    YAL_ASSERT(false && "Should not be called, resolves linker dependency");
}

void*
yalvm_realloc(void*, yalvm_size)
{
    YAL_ASSERT(false && "Should not be called, resolves linker dependency");
}

void*
yalvm_memmove(void *,
              const void *,
              yalvm_size)
{
    YAL_ASSERT(false && "Should not be called, resolves linker dependency");
}


}

static bool
CompileCode(const char* code_file,
            yal::OutputSink& code_output)
{
    // input sink

    FILE* input_file = fopen(code_file, "r");
    if (!input_file)
    {
        return false;
    }

    yal::FileInputSink io_input(input_file);

    yal::Compiler cl(io_input, code_output);

    return cl.compile();
}

TEST(CompileCode, GlobalVariableAccesss) {

    yal::MemoryOutputSink mem_sink(1024*1024*10);

    const std::string path(s_test_path + "/global_var_access.yal");
    const bool compile_result = CompileCode(path.c_str(), mem_sink);

    EXPECT_EQ(true, compile_result);
}

TEST(CompileCode, IfElifElseExpression)
{
    yal::MemoryOutputSink mem_sink(1024*1024*10);
    const std::string path(s_test_path + "/if_elif_else_expression.yal");
    const bool compile_result = CompileCode(path.c_str(), mem_sink);
    EXPECT_EQ(true, compile_result);
}


TEST(CompileCode, GlobalVariableExpression)
{
    yal::MemoryOutputSink mem_sink(1024*1024*10);
    const std::string path(s_test_path + "/global_var_expression.yal");
    const bool compile_result = CompileCode(path.c_str(), mem_sink);
    EXPECT_EQ(true, compile_result);
}

TEST(CompileCode, VariableAccessTypeAssertion)
{
    yal::MemoryOutputSink mem_sink(1024*1024*10);
    const std::string path(s_test_path + "/variable_access_type_assertion.yal");
    const bool compile_result = CompileCode(path.c_str(), mem_sink);
    EXPECT_EQ(true, compile_result);
}

TEST(CompileCode, RecursiveFunctionReturnType)
{
    yal::MemoryOutputSink mem_sink(1024*1024*10);
    const std::string path(s_test_path + "/recursive_function_return_type.yal");
    const bool compile_result = CompileCode(path.c_str(), mem_sink);
    EXPECT_EQ(true, compile_result);
}

TEST(CompileCode, VoidReturn)
{
    yal::MemoryOutputSink mem_sink(1024*1024*10);
    const std::string path(s_test_path + "/void_return.yal");
    const bool compile_result = CompileCode(path.c_str(), mem_sink);
    EXPECT_EQ(true, compile_result);
}

TEST(CompileCode, Scoping)
{
    yal::MemoryOutputSink mem_sink(1024*1024*10);
    const std::string path(s_test_path + "/scope_test.yal");
    const bool compile_result = CompileCode(path.c_str(), mem_sink);
    EXPECT_EQ(true, compile_result);
}

TEST(CompileCode, StringConstantTest)
{
    yal::MemoryOutputSink mem_sink(1024*1024*10);
    const std::string path(s_test_path + "/string_constant_test.yal");
    const bool compile_result = CompileCode(path.c_str(), mem_sink);
    EXPECT_EQ(true, compile_result);
}

TEST(CompileCode, HelloWorldTest)
{
    yal::MemoryOutputSink mem_sink(1024*1024*10);
    const std::string path(s_test_path + "/helloworld.yal");
    const bool compile_result = CompileCode(path.c_str(), mem_sink);
    EXPECT_EQ(true, compile_result);
}

TEST(CompileCode, WhileLoop)
{
    yal::MemoryOutputSink mem_sink(1024*1024*10);
    const std::string path(s_test_path + "/while_loop.yal");
    const bool compile_result = CompileCode(path.c_str(), mem_sink);
    EXPECT_EQ(true, compile_result);
}

TEST(CompileCode, FailEmptyReturn)
{
    yal::MemoryOutputSink mem_sink(1024*1024*10);
    const std::string path(s_test_path + "/fail_empty_return.yal");
    EXPECT_THROW(CompileCode(path.c_str(), mem_sink), yal::SemanticException);
}

TEST(CompileCode, ArithemticTest2)
{
    yal::MemoryOutputSink mem_sink(1024*1024*10);
    const std::string path(s_test_path + "/arithemtic_test_2.yal");
    const bool compile_result = CompileCode(path.c_str(), mem_sink);
    EXPECT_EQ(compile_result, true);
}

TEST(CompileCode, ConflictingDefinitionsArray)
{
    yal::MemoryOutputSink mem_sink(1024*1024*10);
    const std::string path(s_test_path + "/error_conflicting_definitions1.yal");
    EXPECT_THROW(CompileCode(path.c_str(), mem_sink), yal::SemanticException);
}

TEST(CompileCode, ConflictingDefinitionsArrayNative)
{
    yal::MemoryOutputSink mem_sink(1024*1024*10);
    const std::string path(s_test_path + "/error_conflicting_definitions2.yal");
    EXPECT_THROW(CompileCode(path.c_str(), mem_sink), yal::SemanticException);
}

int
main(int argc,
     char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
