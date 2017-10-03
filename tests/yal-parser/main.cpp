#include <yal/yal.h>
#include <yal/lexer/lexer.h>
#include <yal/lexer/tokens.h>
#include <string>
#include <iostream>
#include <yal/parser/parser.h>
#include <yal/io/filestream.h>
#include <yal/util/log.h>


int main(const int argc,
         const char** argv) {

    /*
    if (argc < 2)  {
        std::cerr << "Usage: " << argv[0] << " <file> " << std::endl;
        return EXIT_FAILURE;
    }*/

    yal::FileStream stream;

    if (argc < 2)  {
        stream.open(yal::FileStream::StdStream::In);
    } else {
        if (!stream.open(argv[1], yal::FileStream::kModeRead)) {
            std::cerr << "Failed to open: " << argv[1] << std::endl;
            return EXIT_FAILURE;
        }
    }

    yal::FileStream stdoutStream;
    stdoutStream.open(yal::FileStream::StdStream::Out);

    yal::Lexer lexer(stream);
    yal::Log log(stdoutStream);
    yal::Parser parser (lexer, log);

    return parser.run()  ? EXIT_SUCCESS : EXIT_FAILURE;
}
