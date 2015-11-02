#include <yal/yal.h>
#include <iostream>
#include <cstring>
#include <yal/util/filesink.h>
#include <yal/util/argparser.h>
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

void
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
printGlobals(const yalvm_ctx_t*)
{

     /*yalvm_register_t tmp_reg;

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
    } */
}

static const char* sDescription =
        "yale - Yet Anoter Language Executor (0.2.0, yalvm " YALVM_VERSION_STR")\n"
        "usage: yale [options] <input file>\n";

enum Option
{
    kOptionPrintGlobals = yal::KArgsOptionHelp + 1
};


int main(const int argc,
         const char** argv)
{

    // output sink
    yal::FileOutputSink io_output(stdout);
    // error sink
    yal::FileOutputSink err_output(stderr);


    // setup arg parser
    yal::ArgParser arg_parser;
    arg_parser.add(kOptionPrintGlobals, 'g', "print-globals",
                   "Print global variables after execution", 0);

    int parse_result = -1;
    try
    {
        parse_result = arg_parser.parse(argc, argv);
    }
    catch (const yal::ArgParserException ex)
    {
        std::cerr << ex.what() << std::endl;
    }

    if (parse_result < 0)
    {
        arg_parser.printHelp(std::cerr, sDescription);
        return EXIT_FAILURE;
    }

    if (arg_parser.isSet(0))
    {
        arg_parser.printHelp(std::cout, sDescription);
        return EXIT_SUCCESS;
    }


    FILE* input = nullptr;
    const int args_left = argc - parse_result;
    if (args_left > 0)
    {
        input = fopen(argv[parse_result], "r");
        if (!input)
        {
            std::cerr << "Could not open '" << argv[parse_result] << "'" << std::endl;
            return EXIT_FAILURE;
        }
    }
    else
    {
        arg_parser.printHelp(std::cout, sDescription);
        return EXIT_FAILURE;
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

    yalvm_binary_t vm_binary;
    yalvm_binary_init(&vm_binary);

    if (yalvm_binary_load(&vm_binary, buffer, file_size) != yalvm_true)
    {
        fprintf(stderr, "Failed to load binary '%s'\n", argv[1]);
        yalvm_binary_destroy(&vm_binary);
        return EXIT_FAILURE;
    }

    yalvm_ctx_t vm;


    if (yalvm_ctx_create(&vm, &vm_binary, stack, 1024 * 1024) != yalvm_true)
    {
        fprintf(stderr, "Failed to create execution context\n");
        yalvm_binary_destroy(&vm_binary);
        return EXIT_FAILURE;
    }

    yalvm_func_hdl_t func_hdl;

    if (yalvm_ctx_acquire_function(&vm, &func_hdl, yalvm_func_global_name()) != yalvm_true)
    {
        fprintf(stderr, "Failed to load global function into context\n");
        yalvm_binary_destroy(&vm_binary);
        return EXIT_FAILURE;
    }


    yalvm_u32 exec_val = yalvm_func_hdl_execute(&func_hdl);

    if (exec_val == YALVM_ERROR_NONE)
    {
        if (arg_parser.isSet(kOptionPrintGlobals))
        {
            printGlobals(&vm);
        }
    }
    else
    {
        fprintf(stderr, "Execution error: %s\n", yalvm_error_str(exec_val));
    }

    yalvm_ctx_release_function(&func_hdl);
    yalvm_ctx_destroy(&vm);
    yalvm_binary_destroy(&vm_binary);

    return (exec_val == YALVM_ERROR_NONE) ? EXIT_SUCCESS : EXIT_FAILURE;
}
