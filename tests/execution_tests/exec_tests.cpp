#include <yal/yal.h>
#include <yal/compiler/compiler.h>
#include <yal/util/filesink.h>
#include <yal/util/stringinputsink.h>
#include <yal/util/memorysink.h>
#include <cstring>
#include <string>
#include <stdarg.h>
#include <gtest/gtest.h>
#include <yalvm/yalvm_ctx.h>
#include <yalvm/yalvm_error.h>

static const std::string s_test_path = YAL_TEST_SOURCES_PATH;

#define EXEC_TEST_MEMCHECK
#if defined(EXEC_TEST_MEMCHECK)
#define EXEC_TEST_MEMCHECK_BEGIN { \
    BytesAllocated = 0;

#define EXEC_TEST_MEMCHECK_END } \
    EXPECT_EQ(BytesAllocated, 0);

#else
#define EXEC_TEST_MEMCHECK_BEGIN

#define EXEC_TEST_MEMCHECK_END
#endif

// yalvm external function that need to be implemented
extern "C"
{
#if defined(EXEC_TEST_MEMCHECK)
static yalvm_size BytesAllocated;
#endif
void*
yalvm_malloc(yalvm_size size)
{
#if defined(EXEC_TEST_MEMCHECK)
    void* ptr = malloc(size + sizeof(yalvm_size));
    if (ptr)
    {
        yalvm_size* sptr = (yalvm_size*)ptr;
        sptr[0] = size;
        BytesAllocated += size;
        return ++sptr;
    }
    return NULL;
#else
    return malloc(size);
#endif

}

void
yalvm_free(void* ptr)
{
#if defined(EXEC_TEST_MEMCHECK)
    if (ptr)
    {
        yalvm_size* sptr = (yalvm_size*)ptr;
        --sptr;
        yalvm_size size = *sptr;
        BytesAllocated-= size;
        free(sptr);
    }
#else
    free(ptr);
#endif
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
#if defined(EXEC_TEST_MEMCHECK)
    if (!ptr)
    {
        return yalvm_malloc(size);
    }
    else
    {
        yalvm_size* sptr = (yalvm_size*)ptr;
        --sptr;
        yalvm_size cur_size = *sptr;
        BytesAllocated += size;
        BytesAllocated -= cur_size;
        sptr = (yalvm_size*) realloc(sptr, size + sizeof(yalvm_size));
        sptr[0] = size;
        return ++sptr;
    }
#else
    return realloc(ptr, size);
#endif
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

    yal::Compiler cl(io_input, code_output);

    return cl.compile();
}

class TestVM
{
public:
    TestVM(const char* fileName):
        _filePath(s_test_path + "/" + fileName),
        _memorySink(1024*1024*10),
        _execCtx()
    {
        yalvm_binary_init(&_vmBinary);
        memset(&_funcHdl, 0, sizeof(_funcHdl));
    }

    ~TestVM()
    {
        yalvm_ctx_release_function(&_funcHdl);
        yalvm_bool result = yalvm_ctx_destroy(&_execCtx);
        EXPECT_EQ(result, yalvm_true);
        yalvm_binary_destroy(&_vmBinary);
    }

    bool compile()
    {
        const bool compile_result = CompileCode(_filePath.c_str(), _memorySink);
        EXPECT_EQ(true, compile_result);

        if (compile_result)
        {
            if (yalvm_binary_load(&_vmBinary,  _memorySink.buffer(),
                                  _memorySink.bufferSize()) != yalvm_true)
            {
                return false;
            }

            if (yalvm_ctx_create(&_execCtx, &_vmBinary, stack, 1024 * 1024) != yalvm_true)
            {
                return false;
            }
        }
        return true;
    }

    bool loadFunction(const char* functionName)
    {
        yalvm_ctx_release_function(&_funcHdl);
        if (yalvm_ctx_acquire_function(&_execCtx, &_funcHdl, functionName) != yalvm_true)
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
    yalvm_binary_t _vmBinary;
    yalvm_ctx  _execCtx;
    char stack[1024 * 1024];
    yalvm_func_hdl_t _funcHdl;
};



TEST(ExecutionTest, WhileLoop)
{
    EXEC_TEST_MEMCHECK_BEGIN

            TestVM tvm("while_loop.yal");

    const bool compile_result = tvm.compile();

    EXPECT_EQ(compile_result, true);
    if (!compile_result)
    {
        return;
    }

    const bool setup_result = tvm.loadFunction("loop");
    EXPECT_EQ(setup_result, true);

    if (setup_result)
    {
        yalvm_func_hdl_t* func_hdl = tvm.hdl();

        const yalvm_u32 exec_val = yalvm_func_hdl_execute(func_hdl);
        EXPECT_EQ(exec_val, YALVM_ERROR_NONE);

        EXPECT_EQ(func_hdl->return_register.reg32.i, 10);
    }

    EXEC_TEST_MEMCHECK_END
}


TEST(ExecutionTest, CompareOperators)
{
    EXEC_TEST_MEMCHECK_BEGIN
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
    EXEC_TEST_MEMCHECK_END
}

TEST(ExecutionTest, GlobalAccessOptimization1)
{
    EXEC_TEST_MEMCHECK_BEGIN
            TestVM tvm("global_access_optimization1.yal");

    const bool compile_result = tvm.compile();

    EXPECT_EQ(compile_result, true);
    if (!compile_result)
    {
        return;
    }

    const bool setup_result = tvm.loadFunction("access");
    EXPECT_EQ(setup_result, true);

    if (setup_result)
    {
        yalvm_func_hdl_t* func_hdl = tvm.hdl();

        const yalvm_u32 exec_val = yalvm_func_hdl_execute(func_hdl);
        EXPECT_EQ(exec_val, YALVM_ERROR_NONE);

        EXPECT_EQ(func_hdl->return_register.reg32.i, 3);
    }
    EXEC_TEST_MEMCHECK_END
}

TEST(ExecutionTest, ArithemticTest1)
{
    EXEC_TEST_MEMCHECK_BEGIN
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
    EXEC_TEST_MEMCHECK_END
}

TEST(ExecutionTest, ArithemticTest2)
{
    EXEC_TEST_MEMCHECK_BEGIN

            TestVM tvm("arithemtic_test_2.yal");

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

        const yalvm_u32 exec_val = yalvm_func_hdl_execute(func_hdl);
        EXPECT_EQ(exec_val, YALVM_ERROR_NONE);

        EXPECT_EQ(func_hdl->return_register.reg32.i, 16);

    }

    setup_result = tvm.loadFunction("test_self");
    EXPECT_EQ(setup_result, true);

    if (setup_result)
    {
        yalvm_func_hdl_t* func_hdl = tvm.hdl();

        const yalvm_u32 exec_val = yalvm_func_hdl_execute(func_hdl);
        EXPECT_EQ(exec_val, YALVM_ERROR_NONE);

        EXPECT_EQ(func_hdl->return_register.reg32.i, 36);

    }

    setup_result = tvm.loadFunction("global_test");
    EXPECT_EQ(setup_result, true);

    if (setup_result)
    {
        yalvm_func_hdl_t* func_hdl = tvm.hdl();

        const yalvm_u32 exec_val = yalvm_func_hdl_execute(func_hdl);
        EXPECT_EQ(exec_val, YALVM_ERROR_NONE);

        EXPECT_EQ(func_hdl->return_register.reg32.i, 36);

    }

    EXEC_TEST_MEMCHECK_END
}

TEST(ExecutionTest, HelloWorld)
{
    EXEC_TEST_MEMCHECK_BEGIN

            TestVM tvm("helloworld.yal");

    const bool compile_result = tvm.compile();

    EXPECT_EQ(compile_result, true);
    if (!compile_result)
    {
        return;
    }

    bool setup_result = tvm.loadFunction(yalvm_func_global_name());
    EXPECT_EQ(setup_result, true);

    if (setup_result)
    {
        yalvm_func_hdl_t* func_hdl = tvm.hdl();

        const yalvm_u32 exec_val = yalvm_func_hdl_execute(func_hdl);
        EXPECT_EQ(exec_val, YALVM_ERROR_NONE);
    }

    EXEC_TEST_MEMCHECK_END
}

TEST(ExecutionTest, ArcObjectScope1)
{
    EXEC_TEST_MEMCHECK_BEGIN

            TestVM tvm("arc_object_scope_1.yal");

    const bool compile_result = tvm.compile();

    EXPECT_EQ(compile_result, true);
    if (!compile_result)
    {
        return;
    }

    bool setup_result = tvm.loadFunction(yalvm_func_global_name());
    EXPECT_EQ(setup_result, true);

    if (setup_result)
    {
        yalvm_func_hdl_t* func_hdl = tvm.hdl();

        const yalvm_u32 exec_val = yalvm_func_hdl_execute(func_hdl);
        EXPECT_EQ(exec_val, YALVM_ERROR_NONE);
    }

    EXEC_TEST_MEMCHECK_END
}


int
main(int argc,
     char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
