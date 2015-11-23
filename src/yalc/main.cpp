#include <iostream>
#include <limits>
#include <yal/yal.h>
#include <yal/compiler/compiler.h>
#include <yal/util/filesink.h>
#include <yal/util/argparser.h>
#include <yal/parser/parseexception.h>
#include <yal/symbols/semanticexception.h>
#include <yal/bytecode/bytecodegenexception.h>
#include <yalvm/yalvm.h>

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
}

void*
yalvm_realloc(void*, yalvm_size)
{
    YAL_ASSERT(false && "Should not be called, resolves linker dependency");
}

void*
yalvm_memmove(void *,
              const void *,
              yalvm_size)
{
    YAL_ASSERT(false && "Should not be called, resolves linker dependency");
}


}

static void
printLocatioInfo(const yal::SourceLocationInfo& info,
                 std::ostream& stream)
{
    stream <<":" << info.firstLine << ":" << info.firstColumn << ": ";
}

static void
printLines(const yal::Compiler& compiler,
           const yal::SourceLocationInfo& sourceLoc,
           yal::InputSink& input,
           std::ostream& output)
{
    auto& line_offsets = compiler.lineOffsets();
    const size_t first_line = static_cast<size_t>(sourceLoc.firstLine) -1 ;
    if (first_line > line_offsets.size())
    {
        return;
    }
    const auto& start_offset = line_offsets[first_line];
    if (!input.seekSet(start_offset))
    {
        return;
    }

    size_t last_line = static_cast<size_t>(sourceLoc.lastLine == sourceLoc.firstLine ? sourceLoc.lastLine : sourceLoc.lastLine - 1);
    const size_t end_offset = last_line >= line_offsets.size()
            ? 256
            : line_offsets[last_line];

    const size_t diff = (end_offset - 1) - start_offset;
    char buffer[diff];
    size_t bytes_read = input.read(buffer, diff);
    if (!bytes_read)
    {
        return;
    }

    output <<" `-> ";
    output.write(buffer, bytes_read);
    output << std::endl;
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

    const char* output_name = "yalc.out";

    if (arg_parser.isSet(kOptionOutputFile))
    {
        const char* arg = arg_parser.getSingleArg(kOptionOutputFile);
        output_name = arg;
    }

    if (arg_parser.isSet(kOptionDumpAst))
    {
        compiler_flags |= yal::Compiler::kDumpAst;
    }


    FILE* input = nullptr;
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
    else
    {
        arg_parser.printHelp(std::cerr, sDescription);
        return EXIT_FAILURE;
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

    yal::Compiler cl(io_input, code_output);
    try
    {
        return cl.compile(compiler_flags) ? EXIT_SUCCESS : EXIT_FAILURE;
    }
    catch(const yal::ParseException& parseException)
    {
        std::cerr << argv[parse_result];
        printLocatioInfo(parseException.location(), std::cerr);
        std::cerr << parseException.what() << std::endl;
        printLines(cl, parseException.location(), io_input, std::cerr);
    }
    catch(const yal::SemanticException& semanticException)
    {
        std::cerr << argv[parse_result];
        printLocatioInfo(semanticException.astNode().locationInfo(), std::cerr);
        std::cerr << semanticException.what();
        printLines(cl, semanticException.astNode().locationInfo(), io_input, std::cerr);
    }
    catch(const yal::ByteCodeGenException& byteCodeGenException)
    {
        std::cerr << argv[parse_result] <<": ";
        std::cerr << byteCodeGenException.what() << std::endl;
    }
}
