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
