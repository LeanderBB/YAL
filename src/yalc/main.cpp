#include <iostream>
#include <yal/yal.h>
#include <yal/compiler/compiler.h>
#include <yal/util/filesink.h>
#include <yal/util/argparser.h>
#include <yal/parser/parseexception.h>
#include <yal/symbols/semanticexception.h>
#include <yal/bytecode/bytecodegenexception.h>

static const char* sDescription =
        "yalc - Yet Anoter Language Compiler (" YAL_VERSION_STR ")\n"
                                                                "usage: yalc [options] <input file>\n";

enum Option
{
    kOptionOutputFile = yal::KArgsOptionHelp + 1,
    kOptionDumpAst
};


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

static void
printLocatioInfo(const yal::SourceLocationInfo& info,
                 std::ostream& stream)
{
    stream << "[" << info.firstLine << ":" << info.firstColumn << "] ";
}

int main(const int argc,
         const char** argv)
{
    // setup arg parser
    yal::ArgParser arg_parser;
    arg_parser.add(kOptionOutputFile, 'o', "output", "Ouput file name", yal::kArgFlagSingleValue);
    arg_parser.add(kOptionDumpAst, "dump-ast", "Dump abstract syntax tree", 0);

    int parse_result = -1;

    try
    {
        parse_result  = arg_parser.parse(argc, argv);
    } catch(yal::ArgParserException& argException)
    {
        std::cerr << argException.what() << std::endl;
        arg_parser.printHelp(std::cerr, sDescription);
        return EXIT_FAILURE;
    }

    if (parse_result < 0)
    {
        arg_parser.printHelp(std::cerr, sDescription);
        return EXIT_FAILURE;
    }

    yal_u32 compiler_flags = 0;

    if (arg_parser.isSet(0))
    {
        arg_parser.printHelp(std::cout, sDescription);
        return EXIT_SUCCESS;
    }

    const char* output_name = "a.out.yalb";

    if (arg_parser.isSet(kOptionOutputFile))
    {
        const char* arg = arg_parser.getSingleArg(kOptionOutputFile);
        output_name = arg;
    }

    if (arg_parser.isSet(kOptionDumpAst))
    {
        compiler_flags |= yal::Compiler::kDumpAst;
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

    try
    {
        yal::Compiler cl(io_input, code_output);
        return cl.compile(compiler_flags) ? EXIT_SUCCESS : EXIT_FAILURE;
    }
    catch(const yal::ParseException& parseException)
    {
        std::cerr << "Parse Error: ";
        printLocatioInfo(parseException.location(), std::cerr);
        std::cerr << parseException.what() << std::endl;
    }
    catch(const yal::SemanticException& semanticException)
    {
        std::cerr<< "Semantic Error: ";
        printLocatioInfo(semanticException.astNode().locationInfo(), std::cerr);
        std::cerr << semanticException.what() << std::endl;
    }
    catch(const yal::ByteCodeGenException& byteCodeGenException)
    {
        std::cerr<< "ByteCodeGen Error: ";
        std::cerr << byteCodeGenException.what() << std::endl;
    }
}
