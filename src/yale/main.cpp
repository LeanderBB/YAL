#include <yal/yal.h>
#include <cstdio>
#include <cstring>
#include <yalvm/yalvm_ctx.h>
#include <yalvm/yalvm_error.h>
#include <stdarg.h>


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
yalvm_print(yalvm_ctx *ctx,
            const char *string)
{
    (void) *ctx;
    printf("%s", string);
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

}

static void
printRegister(yalvm_register_t* reg)
{
    printf("  hex32: 0x%08" YALVM_PRI_HEX32 "\n",reg->reg32.value);
    printf("  hex64: 0x%016" YALVM_PRI_HEX64 "\n", reg->reg64.value);
    printf("  i32  : %" YALVM_PRIi32 "\n", *yalvm_register_to_i32(reg));
    printf("  i64  : %" YALVM_PRIi64 "\n", *yalvm_register_to_i64(reg));
    printf("  u32  : %" YALVM_PRIu32 "\n", *yalvm_register_to_u32(reg));
    printf("  u64  : %" YALVM_PRIu64 "\n", *yalvm_register_to_u64(reg));
    printf("  f32  : %f\n", *yalvm_register_to_f32(reg));
    printf("  f64  : %lf\n", *yalvm_register_to_f64(reg));
}


static void
printGlobals(const yalvm_ctx_t* ctx)
{
    yalvm_register_t tmp_reg;

    const yalvm_u32 n_globlas32 = yalvm_ctx_num_globals32(ctx);
    const yalvm_u32 n_globlas64 = yalvm_ctx_num_globals64(ctx);

    const yalvm_bin_global32_t* globals_32 = yalvm_ctx_globals32(ctx);
    const yalvm_bin_global64_t* globals_64 = yalvm_ctx_globals64(ctx);

    if (n_globlas32)
    {
        printf("Globals 32\n");

        for (yalvm_u32 i = 0; i < n_globlas32; ++i)
        {
            const yalvm_bin_global32_t* cur_global = globals_32 + i;
            printf("> %08x\n", cur_global->hash);
            tmp_reg.reg32.value = cur_global->val;
            printRegister(&tmp_reg);
        }
    }

    if (n_globlas64)
    {
        printf("Globals 64\n");
        for (yalvm_u32 i = 0; i < n_globlas64; ++i)
        {
            const yalvm_bin_global64_t* cur_global = globals_64 + i;
            printf("> %08x\n", cur_global->hash);
            tmp_reg.reg64.value = cur_global->val;
            printRegister(&tmp_reg);
        }
    }
}

int main(const int argc,
         const char** argv)
{

    FILE* input = nullptr;
    if (argc >1)
    {
        input = fopen(argv[1],"r");
        if (!input)
        {
            fprintf(stderr, "Could not open '%s'\n", argv[1]);
            return EXIT_FAILURE;
        }
    }
    else
    {
        printf("yale - 0.2.0\n");
        printf("yalvm %d.%d.%d\n\n",
               YALVM_VERSION_MAJOR,
               YALVM_VERSION_MINOR,
               YALVM_VERSION_PATCH);
        printf("yalvm binary executor\n\n");
        printf("Usage: %s <binary file>\n\n", argv[0]);
        return EXIT_SUCCESS;
    }

    fseek(input, 0, SEEK_END);
    const size_t file_size = ftell(input);
    fseek(input, 0, SEEK_SET);

    char buffer[file_size];

    const size_t bytes_read = fread(buffer, 1, file_size, input);

    fclose(input);

    if (bytes_read != file_size)
    {
        fprintf(stderr, "Failed to read '%s' into memory\n", argv[1]);
        return EXIT_FAILURE;
    }

    char stack[1024 * 1024];

    yalvm_ctx_t vm;

    yalvm_ctx_create(&vm, stack, 1024 * 1024);

    if (yalvm_ctx_set_binary(&vm, buffer, file_size) != yalvm_true)
    {
        fprintf(stderr, "Failed to load '%s' into context\n", argv[1]);
        return EXIT_FAILURE;
    }

    yalvm_u32 exec_val = yalvm_ctx_execute(&vm);

    if (exec_val == YALVM_ERROR_NONE)
    {
        printGlobals(&vm);
    }
    else
    {
        fprintf(stderr, "Execution error: %s\n", yalvm_error_str(exec_val));
    }

    yalvm_ctx_destroy(&vm);

    return (exec_val == YALVM_ERROR_NONE) ? EXIT_SUCCESS : EXIT_FAILURE;
}
