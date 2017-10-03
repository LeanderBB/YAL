#include <yal/yal.h>
#include <yal/lexer/lexer.h>
#include <yal/lexer/tokens.h>
#include <yal/io/filestream.h>
#include <string>
#include <iostream>

int main(const int argc,
         const char** argv) {
    yal::FileStream stream;

    if (argc < 2)  {
        stream.open(yal::FileStream::StdStream::In);
    } else {
        if (!stream.open(argv[1], yal::FileStream::kModeRead)) {
            std::cerr << "Failed to open: " << argv[1] << std::endl;
            return EXIT_FAILURE;
        }
    }

    yal::Lexer lexer(stream);

    while(true) {
        const yal::Lexer::Status status = lexer.scan();
        const yal::Lexer::TokenInfo info = lexer.getLastToken();
        if (status == yal::Lexer::Status::Ok) {
            std::cout <<"[Token] " << yal::TokenToString(info.token)
                      << " (l: " << info.lineStart
                      << " c: " << info.columnStart
                      << ")\n";
        } else if (status == yal::Lexer::Status::Error) {
            std::cout <<" [Error] Unknown token at l: " << info.lineStart
                      << " c: " << info.columnStart
                      << "\n";
            break;
        } else {
            std::cout << "End of Stream\n";
            break;
        }
    }
    return EXIT_SUCCESS;
}
