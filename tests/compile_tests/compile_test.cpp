#include <yal/yal.h>
#include <yal/compiler/compiler.h>
#include <yal/util/filesink.h>
#include <yal/util/sinkerrorhandler.h>
#include <yal/util/stringinputsink.h>
#include <yal/util/memorysink.h>
#include <cstring>
#include <string>
#include <gtest/gtest.h>

static const std::string s_test_path = YAL_TEST_SOURCES_PATH;

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
    // output sink
    yal::NullOutputSink io_output;

    // code output sink,

    // error sink
    yal::FileOutputSink err_output(stdout);
    yal::SinkErrorHandler err_handler(err_output);
    yal::Compiler cl(io_input, io_output, code_output, err_handler);

    return cl.compile();
}

TEST(CompileCode, GlobalVariableAccesss) {

    yal::MemoryOuputSink mem_sink(1024*1024*10);

    const std::string path(s_test_path + "/global_var_access.yal");
    const bool compile_result = CompileCode(path.c_str(), mem_sink);

    EXPECT_EQ(true, compile_result);
}

TEST(CompileCode, IfElifElseExpression)
{
    yal::MemoryOuputSink mem_sink(1024*1024*10);
    const std::string path(s_test_path + "/if_elif_else_expression.yal");
    const bool compile_result = CompileCode(path.c_str(), mem_sink);
    EXPECT_EQ(true, compile_result);
}


TEST(CompileCode, GlobalVariableExpression)
{
    yal::MemoryOuputSink mem_sink(1024*1024*10);
    const std::string path(s_test_path + "/global_var_expression.yal");
    const bool compile_result = CompileCode(path.c_str(), mem_sink);
    EXPECT_EQ(true, compile_result);
}

TEST(CompileCode, VariableAccessTypeAssertion)
{
    yal::MemoryOuputSink mem_sink(1024*1024*10);
    const std::string path(s_test_path + "/variable_access_type_assertion.yal");
    const bool compile_result = CompileCode(path.c_str(), mem_sink);
    EXPECT_EQ(true, compile_result);
}

TEST(CompileCode, RecursiveFunctionReturnType)
{
    yal::MemoryOuputSink mem_sink(1024*1024*10);
    const std::string path(s_test_path + "/recursive_function_return_type.yal");
    const bool compile_result = CompileCode(path.c_str(), mem_sink);
    EXPECT_EQ(true, compile_result);
}

TEST(CompileCode, VoidReturn)
{
    yal::MemoryOuputSink mem_sink(1024*1024*10);
    const std::string path(s_test_path + "/void_return.yal");
    const bool compile_result = CompileCode(path.c_str(), mem_sink);
    EXPECT_EQ(true, compile_result);
}

TEST(CompileCode, Scoping)
{
    yal::MemoryOuputSink mem_sink(1024*1024*10);
    const std::string path(s_test_path + "/scope_test.yal");
    const bool compile_result = CompileCode(path.c_str(), mem_sink);
    EXPECT_EQ(true, compile_result);
}


int
main(int argc,
     char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
