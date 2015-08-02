#include <yal/yal.h>
#include <yal/compiler/compiler.h>
#include <cstdio>
#include <yal/util/filesink.h>
#include <yal/util/sinkerrorhandler.h>



int main(const int argc,
         const char** argv)
{

    FILE* input = stdin;
    if (argc >1)
    {
        input = fopen(argv[1],"r");
        if (!input)
        {
            fprintf(stderr, "Could not open '%s'\n", argv[1]);
            return EXIT_FAILURE;
        }
    }

    // input sink
    yal::FileInputSink io_input(input);
    // output sink
    yal::FileOutputSink io_output(stdout);


    // open defualt output file
    FILE* output = fopen("a.out.yalb","w");
    if (!output)
    {
        fprintf(stderr, "Could not open 'a.out.yalb' for writing\n");
        return EXIT_FAILURE;
    }
    yal::FileOutputSink code_output(output);

    // error sink
    yal::FileOutputSink err_output(stderr);
    yal::SinkErrorHandler err_handler(err_output);
    yal::Compiler cl(io_input, io_output, code_output, err_handler);

    return cl.compile() ? EXIT_SUCCESS : EXIT_FAILURE;
}
