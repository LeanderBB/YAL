#include <yal/yal.h>
#include <yal/compiler/compiler.h>
#include <cstdio>
#include <yal/util/filesink.h>
#include <yal/util/sinkerrorhandler.h>
#include <yal/util/argparser.h>


static const char* sDescription =
        "yalc - Yet Anoter Language Compiler (" YAL_VERSION_STR ")\n"
        "usage: yalc [options] <input file>\n";

enum Option
{
    kOptionOutputFile = yal::KArgsOptionHelp + 1
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
    arg_parser.add(kOptionOutputFile, 'o', "output", "Ouput file name", yal::kArgFlagSingleValue);

    const int parse_result = arg_parser.parse(argc, argv, err_output);

    if (parse_result < 0)
    {
        arg_parser.printHelp(io_output, sDescription);
        return EXIT_FAILURE;
    }

    if (arg_parser.isSet(0))
    {
        arg_parser.printHelp(io_output, sDescription);
        return EXIT_SUCCESS;
    }

    const char* output_name = "a.out.yalb";

    if (arg_parser.isSet(kOptionOutputFile))
    {
        const char* arg = arg_parser.getSingleArg(kOptionOutputFile);
        output_name = arg;
    }


    FILE* input = stdin;
    const int args_left = argc - parse_result;
    if (args_left > 0)
    {
        input = fopen(argv[parse_result],"r");
        if (!input)
        {
            fprintf(stderr, "Could not open '%s'\n", argv[parse_result]);
            return EXIT_FAILURE;
        }
    }

    // input sink
    yal::FileInputSink io_input(input);



    // open defualt output file
    FILE* output = fopen(output_name,"w");
    if (!output)
    {
        fprintf(stderr, "Could not open '%s' for writing\n", output_name);
        return EXIT_FAILURE;
    }

    yal::FileOutputSink code_output(output);
    // error sink

    yal::SinkErrorHandler err_handler(err_output);
    yal::Compiler cl(io_input, io_output, code_output, err_handler);

    return cl.compile() ? EXIT_SUCCESS : EXIT_FAILURE;
}
