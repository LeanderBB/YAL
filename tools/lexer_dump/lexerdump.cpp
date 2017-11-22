/*
 *  Copyright 2017 by Leander Beernaert (leanderbb@gmail.com)
 *
 *  This file is part of YAL.
 *
 *  YAL is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, either version 3
 *  of the License, or (at your option) any later version.
 *
 *  YAL is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with YAL. If not, see <http://www.gnu.org/licenses/>.
 */
#include <yal/yal.h>
#include <yal/lexer/lexer.h>
#include <yal/lexer/tokens.h>
#include <yal/io/filestream.h>
#include <yal/io/memorystream.h>
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

    yal::MemoryStream memStream;
    memStream.create(stream);

    yal::Lexer lexer(memStream);

    while(true) {
        const yal::Lexer::Status status = lexer.scan();
        const yal::TokenInfo info = lexer.getLastToken();
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
