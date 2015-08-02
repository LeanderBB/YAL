#include <yal/yal.h>
#include <yal/compiler/compiler.h>
#include <cstdio>
#include <yal/util/filesink.h>
#include <yal/util/sinkerrorhandler.h>
#include <yal/bytecode/bytecodeprinter.h>


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
        printf("yaldisass - 0.1.0\n");
        printf("yalvm binary disassembler\n\n");
        printf("Usage: %s <binary file>\n\n", argv[0]);
        return EXIT_SUCCESS;
    }

    // input sink
    yal::FileInputSink io_input(input);
    // output sink
    yal::FileOutputSink io_output(stdout);


    // error sink
    //yal::FileOutputSink err_output(stderr);
    //yal::SinkErrorHandler err_handler(err_output);

    yal::ByteCodePrinter printer(io_input, io_output);
    return printer.process() ? EXIT_SUCCESS : EXIT_FAILURE;
}
