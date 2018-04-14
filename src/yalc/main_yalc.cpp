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

#include <yal/ast/modulemanager.h>
#include <yal/ast/astprinter.h>
#include <yal/ast/astcontext.h>
#include <yal/compiler/compiler.h>
#include <yal/error/errorprinter.h>
#include <yal/error/errorreporter.h>
#include <yal/lexer/lexer.h>
#include <yal/lexer/tokens.h>
#include <yal/io/filestream.h>
#include <yal/io/memorystream.h>
#include <yal/io/sourcemanager.h>
#include <yal/io/sourcemanager.h>
#include <yal/io/sourceitems.h>
#include <yal/parser/parser.h>
#include <yal/transpiler/c/transpilerc.h>
#include <yal/util/log.h>

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

    yal::FileStream stdoutStream;
    stdoutStream.open(yal::FileStream::StdStream::Out);
    yal::Log log(stdoutStream);

   auto sourceStream = std::make_unique<yal::SourceItemFile>();

   if (!sourceStream->open(stream, (argc < 2) ? "stdin" : argv[1])) {
       std::cerr << "Failed to create source stream" << std::endl;
       return EXIT_FAILURE;
   }

    yal::SourceManager sourceManager;
    yal::ModuleManager moduleManager;
    yal::ErrorReporter errorReporter;

    auto handle = sourceManager.add(std::move(sourceStream));

    yal::Compiler compiler(log, errorReporter, sourceManager, moduleManager);

    yal::Module* module = compiler.compile(handle);

    if (!errorReporter.empty()) {
        yal::ErrorPrinter errPrinter(stdoutStream, sourceManager);
        errPrinter.enableColorCodes(true);
        errPrinter.print(errorReporter);
    }

    if (module != nullptr) {
        yal::AstPrinter astPrinter(stdoutStream);
        astPrinter.visit(*module->getDeclNode());

        if (argc > 2) {
            yal::TranspilerC transpiler;
            yal::TranspilerOptions options;
            options.intermediatOuputDir = argv[2];
            transpiler.transpile(options, log, *module, sourceManager);
        }

    }

    return module != nullptr ? EXIT_SUCCESS : EXIT_FAILURE;
}
