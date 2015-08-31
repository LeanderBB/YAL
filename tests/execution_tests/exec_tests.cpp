#include <yal/yal.h>
#include <yal/compiler/compiler.h>
#include <yal/util/filesink.h>
#include <yal/util/sinkerrorhandler.h>
#include <yal/util/stringinputsink.h>
#include <yal/util/memorysink.h>
#include <cstring>
#include <string>
#include <stdarg.h>
#include <gtest/gtest.h>
#include <yalvm/yalvm_ctx.h>
#include <yalvm/yalvm_error.h>

static const std::string s_test_path = YAL_TEST_SOURCES_PATH;

// yalvm external function that need to be implemented
extern "C"
{

void*
yalvm_malloc(yalvm_size size)
{
    return malloc(size);
}

void
yalvm_free(void* ptr)
{
    return free(ptr);
}

void
yalvm_memset(void* ptr, int val, yalvm_size size)
{
    memset(ptr, val, size);
}

void
yalvm_memcpy(void* dst, const void* src, yalvm_size size)
{
    memcpy(dst, src, size);
}

void
yalvm_print(yalvm_ctx *,
            const char *)
{
    // do nothing
}

int
yalvm_snprintf(char *str,
               yalvm_size size,
               const char *format,
               ...)
{
    int result = 0;
    va_list args;
    va_start(args, format);
    result = vsnprintf(str, size, format, args);
    va_end(args);
    return result;
}

void*
yalvm_realloc(void* ptr, yalvm_size size)
{
    return realloc(ptr, size);
}

void*
yalvm_memmove(void *dest,
              const void *src,
              yalvm_size n)
{
    return memmove(dest, src, n);
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
    // output sink
    yal::NullOutputSink io_output;

    // code output sink,

    // error sink
    yal::FileOutputSink err_output(stdout);
    yal::SinkErrorHandler err_handler(err_output);
    yal::Compiler cl(io_input, io_output, code_output, err_handler);

    return cl.compile();
}

class TestVM
{
public:
    TestVM(const char* fileName):
        _filePath(s_test_path + "/" + fileName),
        _memorySink(1024*1024*10),
        _vm()
    {
        yalvm_ctx_create(&_vm, stack, 1024 * 1024);
        memset(&_funcHdl, 0, sizeof(_funcHdl));
    }

    ~TestVM()
    {
        yalvm_ctx_release_function(&_funcHdl);
        yalvm_ctx_destroy(&_vm);
    }

    bool compile()
    {
        const bool compile_result = CompileCode(_filePath.c_str(), _memorySink);
        EXPECT_EQ(true, compile_result);

        if (compile_result)
        {
            if (yalvm_ctx_set_binary(&_vm, _memorySink.buffer(),
                                     _memorySink.bufferSize()) != yalvm_true)
            {
                return false;
            }
        }
        return true;
    }

    bool loadFunction(const char* functionName)
    {
        yalvm_ctx_release_function(&_funcHdl);
        if (yalvm_ctx_acquire_function(&_vm, &_funcHdl, functionName) != yalvm_true)
        {
            return false;
        }
        return true;
    }

    yalvm_func_hdl_t* hdl()
    {
        return &_funcHdl;
    }

protected:
    const std::string _filePath;
    yal::MemoryOutputSink  _memorySink;
    yalvm_ctx  _vm;
    char stack[1024 * 1024];
    yalvm_func_hdl_t _funcHdl;
};



TEST(ExecutionTest, WhileLoop)
{
    TestVM tvm("while_loop.yal");

    const bool compile_result = tvm.compile();

    EXPECT_EQ(compile_result, true);
    if (!compile_result)
    {
        return;
    }

    const bool setup_result = tvm.loadFunction(yalvm_func_global_name());
    EXPECT_EQ(setup_result, true);

    if (setup_result)
    {
        yalvm_func_hdl_t* func_hdl = tvm.hdl();

        const yalvm_u32 exec_val = yalvm_func_hdl_execute(func_hdl);
        EXPECT_EQ(exec_val, YALVM_ERROR_NONE);

        const yalvm_bin_global32_t* global = yalvm_ctx_globals32_by_name(func_hdl->ctx,
                                                                         "i");
        EXPECT_NE(global, nullptr);

        if (global)
        {
            EXPECT_EQ(global->val, 10u);
        }
    }
}


TEST(ExecutionTest, CompareOperators)
{
    TestVM tvm("compare_operators_test.yal");

    const bool compile_result = tvm.compile();

    EXPECT_EQ(compile_result, true);
    if (!compile_result)
    {
        return;
    }

    bool setup_result = tvm.loadFunction("cmp_eq");
    EXPECT_EQ(setup_result, true);

    if (setup_result)
    {
        yalvm_func_hdl_t* func_hdl = tvm.hdl();

        yalvm_register_t reg1;
        reg1.reg32.value = 10;

        yalvm_register_t reg2;
        reg2.reg32.value = 10;

        EXPECT_EQ(yalvm_func_hdl_push_arg(func_hdl, &reg1), yalvm_true);
        EXPECT_EQ(yalvm_func_hdl_push_arg(func_hdl, &reg2), yalvm_true);

        const yalvm_u32 exec_val = yalvm_func_hdl_execute(func_hdl);
        EXPECT_EQ(exec_val, YALVM_ERROR_NONE);

        EXPECT_EQ(func_hdl->return_register.reg32.value, 1u);

    }

    setup_result = tvm.loadFunction("cmp_eq");
    EXPECT_EQ(setup_result, true);

    if (setup_result)
    {
        yalvm_func_hdl_t* func_hdl = tvm.hdl();

        yalvm_register_t reg1;
        reg1.reg32.value = 200;

        yalvm_register_t reg2;
        reg2.reg32.value = 10;

        EXPECT_EQ(yalvm_func_hdl_push_arg(func_hdl, &reg1), yalvm_true);
        EXPECT_EQ(yalvm_func_hdl_push_arg(func_hdl, &reg2), yalvm_true);

        const yalvm_u32 exec_val = yalvm_func_hdl_execute(func_hdl);
        EXPECT_EQ(exec_val, YALVM_ERROR_NONE);

        EXPECT_EQ(func_hdl->return_register.reg32.value, 0u);

    }


    setup_result = tvm.loadFunction("cmp_ne");
    EXPECT_EQ(setup_result, true);

    if (setup_result)
    {
        yalvm_func_hdl_t* func_hdl = tvm.hdl();

        yalvm_register_t reg1;
        reg1.reg32.value = 10;

        yalvm_register_t reg2;
        reg2.reg32.value = 10;

        EXPECT_EQ(yalvm_func_hdl_push_arg(func_hdl, &reg1), yalvm_true);
        EXPECT_EQ(yalvm_func_hdl_push_arg(func_hdl, &reg2), yalvm_true);

        const yalvm_u32 exec_val = yalvm_func_hdl_execute(func_hdl);
        EXPECT_EQ(exec_val, YALVM_ERROR_NONE);

        EXPECT_EQ(func_hdl->return_register.reg32.value, 0u);

    }

    setup_result = tvm.loadFunction("cmp_ne");
    EXPECT_EQ(setup_result, true);

    if (setup_result)
    {
        yalvm_func_hdl_t* func_hdl = tvm.hdl();

        yalvm_register_t reg1;
        reg1.reg32.value = 200;

        yalvm_register_t reg2;
        reg2.reg32.value = 10;

        EXPECT_EQ(yalvm_func_hdl_push_arg(func_hdl, &reg1), yalvm_true);
        EXPECT_EQ(yalvm_func_hdl_push_arg(func_hdl, &reg2), yalvm_true);

        const yalvm_u32 exec_val = yalvm_func_hdl_execute(func_hdl);
        EXPECT_EQ(exec_val, YALVM_ERROR_NONE);

        EXPECT_EQ(func_hdl->return_register.reg32.value, 1u);

    }

    setup_result = tvm.loadFunction("cmp_le");
    EXPECT_EQ(setup_result, true);

    if (setup_result)
    {
        yalvm_func_hdl_t* func_hdl = tvm.hdl();

        yalvm_register_t reg1;
        reg1.reg32.value = 10;

        yalvm_register_t reg2;
        reg2.reg32.value = 10;

        EXPECT_EQ(yalvm_func_hdl_push_arg(func_hdl, &reg1), yalvm_true);
        EXPECT_EQ(yalvm_func_hdl_push_arg(func_hdl, &reg2), yalvm_true);

        const yalvm_u32 exec_val = yalvm_func_hdl_execute(func_hdl);
        EXPECT_EQ(exec_val, YALVM_ERROR_NONE);

        EXPECT_EQ(func_hdl->return_register.reg32.value, 1u);

    }

    setup_result = tvm.loadFunction("cmp_le");
    EXPECT_EQ(setup_result, true);

    if (setup_result)
    {
        yalvm_func_hdl_t* func_hdl = tvm.hdl();

        yalvm_register_t reg1;
        reg1.reg32.value = 200;

        yalvm_register_t reg2;
        reg2.reg32.value = 10;

        EXPECT_EQ(yalvm_func_hdl_push_arg(func_hdl, &reg1), yalvm_true);
        EXPECT_EQ(yalvm_func_hdl_push_arg(func_hdl, &reg2), yalvm_true);

        const yalvm_u32 exec_val = yalvm_func_hdl_execute(func_hdl);
        EXPECT_EQ(exec_val, YALVM_ERROR_NONE);

        EXPECT_EQ(func_hdl->return_register.reg32.value, 0u);

    }

    setup_result = tvm.loadFunction("cmp_lt");
    EXPECT_EQ(setup_result, true);

    if (setup_result)
    {
        yalvm_func_hdl_t* func_hdl = tvm.hdl();

        yalvm_register_t reg1;
        reg1.reg32.value = 10;

        yalvm_register_t reg2;
        reg2.reg32.value = 10;

        EXPECT_EQ(yalvm_func_hdl_push_arg(func_hdl, &reg1), yalvm_true);
        EXPECT_EQ(yalvm_func_hdl_push_arg(func_hdl, &reg2), yalvm_true);

        const yalvm_u32 exec_val = yalvm_func_hdl_execute(func_hdl);
        EXPECT_EQ(exec_val, YALVM_ERROR_NONE);

        EXPECT_EQ(func_hdl->return_register.reg32.value, 0u);

    }

    setup_result = tvm.loadFunction("cmp_lt");
    EXPECT_EQ(setup_result, true);

    if (setup_result)
    {
        yalvm_func_hdl_t* func_hdl = tvm.hdl();

        yalvm_register_t reg1;
        reg1.reg32.value = 9;

        yalvm_register_t reg2;
        reg2.reg32.value = 10;

        EXPECT_EQ(yalvm_func_hdl_push_arg(func_hdl, &reg1), yalvm_true);
        EXPECT_EQ(yalvm_func_hdl_push_arg(func_hdl, &reg2), yalvm_true);

        const yalvm_u32 exec_val = yalvm_func_hdl_execute(func_hdl);
        EXPECT_EQ(exec_val, YALVM_ERROR_NONE);

        EXPECT_EQ(func_hdl->return_register.reg32.value, 1u);

    }

    setup_result = tvm.loadFunction("cmp_lt");
    EXPECT_EQ(setup_result, true);

    if (setup_result)
    {
        yalvm_func_hdl_t* func_hdl = tvm.hdl();

        yalvm_register_t reg1;
        reg1.reg32.value = 200;

        yalvm_register_t reg2;
        reg2.reg32.value = 10;

        EXPECT_EQ(yalvm_func_hdl_push_arg(func_hdl, &reg1), yalvm_true);
        EXPECT_EQ(yalvm_func_hdl_push_arg(func_hdl, &reg2), yalvm_true);

        const yalvm_u32 exec_val = yalvm_func_hdl_execute(func_hdl);
        EXPECT_EQ(exec_val, YALVM_ERROR_NONE);

        EXPECT_EQ(func_hdl->return_register.reg32.value, 0u);

    }

    setup_result = tvm.loadFunction("cmp_ge");
    EXPECT_EQ(setup_result, true);

    if (setup_result)
    {
        yalvm_func_hdl_t* func_hdl = tvm.hdl();

        yalvm_register_t reg1;
        reg1.reg32.value = 10;

        yalvm_register_t reg2;
        reg2.reg32.value = 10;

        EXPECT_EQ(yalvm_func_hdl_push_arg(func_hdl, &reg1), yalvm_true);
        EXPECT_EQ(yalvm_func_hdl_push_arg(func_hdl, &reg2), yalvm_true);

        const yalvm_u32 exec_val = yalvm_func_hdl_execute(func_hdl);
        EXPECT_EQ(exec_val, YALVM_ERROR_NONE);

        EXPECT_EQ(func_hdl->return_register.reg32.value, 1u);

    }

    setup_result = tvm.loadFunction("cmp_ge");
    EXPECT_EQ(setup_result, true);

    if (setup_result)
    {
        yalvm_func_hdl_t* func_hdl = tvm.hdl();

        yalvm_register_t reg1;
        reg1.reg32.value = 10;

        yalvm_register_t reg2;
        reg2.reg32.value = 200;

        EXPECT_EQ(yalvm_func_hdl_push_arg(func_hdl, &reg1), yalvm_true);
        EXPECT_EQ(yalvm_func_hdl_push_arg(func_hdl, &reg2), yalvm_true);

        const yalvm_u32 exec_val = yalvm_func_hdl_execute(func_hdl);
        EXPECT_EQ(exec_val, YALVM_ERROR_NONE);

        EXPECT_EQ(func_hdl->return_register.reg32.value, 0u);

    }

    setup_result = tvm.loadFunction("cmp_gt");
    EXPECT_EQ(setup_result, true);

    if (setup_result)
    {
        yalvm_func_hdl_t* func_hdl = tvm.hdl();

        yalvm_register_t reg1;
        reg1.reg32.value = 10;

        yalvm_register_t reg2;
        reg2.reg32.value = 10;

        EXPECT_EQ(yalvm_func_hdl_push_arg(func_hdl, &reg1), yalvm_true);
        EXPECT_EQ(yalvm_func_hdl_push_arg(func_hdl, &reg2), yalvm_true);

        const yalvm_u32 exec_val = yalvm_func_hdl_execute(func_hdl);
        EXPECT_EQ(exec_val, YALVM_ERROR_NONE);

        EXPECT_EQ(func_hdl->return_register.reg32.value, 0u);

    }

    setup_result = tvm.loadFunction("cmp_gt");
    EXPECT_EQ(setup_result, true);

    if (setup_result)
    {
        yalvm_func_hdl_t* func_hdl = tvm.hdl();

        yalvm_register_t reg1;
        reg1.reg32.value = 10;

        yalvm_register_t reg2;
        reg2.reg32.value = 9;

        EXPECT_EQ(yalvm_func_hdl_push_arg(func_hdl, &reg1), yalvm_true);
        EXPECT_EQ(yalvm_func_hdl_push_arg(func_hdl, &reg2), yalvm_true);

        const yalvm_u32 exec_val = yalvm_func_hdl_execute(func_hdl);
        EXPECT_EQ(exec_val, YALVM_ERROR_NONE);

        EXPECT_EQ(func_hdl->return_register.reg32.value, 1u);

    }

    setup_result = tvm.loadFunction("cmp_gt");
    EXPECT_EQ(setup_result, true);

    if (setup_result)
    {
        yalvm_func_hdl_t* func_hdl = tvm.hdl();

        yalvm_register_t reg1;
        reg1.reg32.value = 10;

        yalvm_register_t reg2;
        reg2.reg32.value = 200;

        EXPECT_EQ(yalvm_func_hdl_push_arg(func_hdl, &reg1), yalvm_true);
        EXPECT_EQ(yalvm_func_hdl_push_arg(func_hdl, &reg2), yalvm_true);

        const yalvm_u32 exec_val = yalvm_func_hdl_execute(func_hdl);
        EXPECT_EQ(exec_val, YALVM_ERROR_NONE);

        EXPECT_EQ(func_hdl->return_register.reg32.value, 0u);

    }
}

TEST(ExecutionTest, GlobalAccessOptimization1)
{
    TestVM tvm("global_access_optimization1.yal");

    const bool compile_result = tvm.compile();

    EXPECT_EQ(compile_result, true);
    if (!compile_result)
    {
        return;
    }

    const bool setup_result = tvm.loadFunction(yalvm_func_global_name());
    EXPECT_EQ(setup_result, true);

    if (setup_result)
    {
        yalvm_func_hdl_t* func_hdl = tvm.hdl();

        const yalvm_u32 exec_val = yalvm_func_hdl_execute(func_hdl);
        EXPECT_EQ(exec_val, YALVM_ERROR_NONE);

        const yalvm_bin_global32_t* global = yalvm_ctx_globals32_by_name(func_hdl->ctx,
                                                                         "x");
        EXPECT_NE(global, nullptr);

        if (global)
        {
            EXPECT_EQ(global->val, 3u);
        }
    }
}

TEST(ExecutionTest, ArithemticTest1)
{
    TestVM tvm("arithemtic_test_1.yal");

    const bool compile_result = tvm.compile();

    EXPECT_EQ(compile_result, true);
    if (!compile_result)
    {
        return;
    }

    bool setup_result = tvm.loadFunction("test");
    EXPECT_EQ(setup_result, true);

    if (setup_result)
    {
        yalvm_func_hdl_t* func_hdl = tvm.hdl();

        yalvm_register_t reg1;
        reg1.reg32.value = 0;

        EXPECT_EQ(yalvm_func_hdl_push_arg(func_hdl, &reg1), yalvm_true);

        const yalvm_u32 exec_val = yalvm_func_hdl_execute(func_hdl);
        EXPECT_EQ(exec_val, YALVM_ERROR_NONE);

        EXPECT_EQ(func_hdl->return_register.reg32.value, 1u);

    }

    setup_result = tvm.loadFunction("test");
    EXPECT_EQ(setup_result, true);

    if (setup_result)
    {
        yalvm_func_hdl_t* func_hdl = tvm.hdl();

        yalvm_register_t reg1;
        reg1.reg32.value = 4;

        EXPECT_EQ(yalvm_func_hdl_push_arg(func_hdl, &reg1), yalvm_true);

        const yalvm_u32 exec_val = yalvm_func_hdl_execute(func_hdl);
        EXPECT_EQ(exec_val, YALVM_ERROR_NONE);

        EXPECT_EQ(func_hdl->return_register.reg32.value, 10u);

    }
}

int
main(int argc,
     char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
