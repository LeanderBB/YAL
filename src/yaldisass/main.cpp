#include <iostream>
#include <yal/yal.h>
#include <yal/compiler/compiler.h>
#include <yal/util/filesink.h>
#include <yal/bytecode/bytecodeprinter.h>


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
    return -1;
}

void*
yalvm_realloc(void*, yalvm_size)
{
    YAL_ASSERT(false && "Should not be called, resolves linker dependency");
    return NULL;
}

void*
yalvm_memmove(void *,
              const void *,
              yalvm_size)
{
    YAL_ASSERT(false && "Should not be called, resolves linker dependency");
    return NULL;
}


}


int main(const int argc,
         const char** argv)
{

    FILE* input = nullptr;
    if (argc > 1)
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
        printf("yaldisass - 0.2.0\n");
        printf("yalvm binary disassembler\n\n");
        printf("Usage: %s <binary file>\n\n", argv[0]);
        return EXIT_SUCCESS;
    }

    // input sink
    yal::FileInputSink io_input(input);

    try
    {
    yal::ByteCodePrinter printer(io_input, std::cout);
    return printer.process() ? EXIT_SUCCESS : EXIT_FAILURE;
    }
    catch(const std::runtime_error& error)
    {
        std::cerr << error.what() << std::endl;
        return EXIT_FAILURE;
    }
}
