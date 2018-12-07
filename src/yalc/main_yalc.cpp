/*
 *  Copyright 2017 by Leander Beernaert (lbb-dev@pm.me)
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

#include <yal/backendc/backendc.h>
#include <yal/error/errorprinter.h>
#include <yal/error/errorreporter.h>
#include <yal/frontend/ast/astprinter.h>
#include <yal/frontend/frontend.h>
#include <yal/frontend/modulemanager.h>
#include <yal/io/filestream.h>
#include <yal/io/memorystream.h>
#include <yal/io/sourcemanager.h>
#include <yal/io/sourcemanager.h>
#include <yal/io/sourceitems.h>
#include <yal/os/path.h>
#include <yal/util/log.h>

#include <string>
#include <iostream>

int main(const int argc,
         const char** argv) {

    std::unique_ptr<yal::SourceItem> sourceItem;
    if (argc < 2)  {
        sourceItem = std::make_unique<yal::SourceItemStdInput>();
    } else {
        sourceItem = std::make_unique<yal::SourceItemFile>(argv[1]);
    }

    yal::FileStream stdoutStream;
    stdoutStream.open(yal::FileStream::StdStream::Out);

    auto realPath = yal::Path::GetRealPath("./");
    const std::string realPathStr = realPath.value_or("./");

    yal::SourceManager sourceManager;
    yal::frontend::ModuleManager moduleManager(realPathStr);
    yal::ErrorReporter errorReporter;

    auto handle = sourceManager.add(std::move(sourceItem));

    yal::frontend::Frontend frontend(errorReporter,
                                     sourceManager,
                                     moduleManager);

    yal::frontend::FrontendOptions options;
    yal::frontend::Module* module = frontend.compile(handle,options);

    if (module != nullptr) {
        yal::frontend::AstPrinter astPrinter(stdoutStream);
        astPrinter.visit(*module->getDeclNode());

        if (argc > 2) {
            yal::backend::c::BackendC backend;
            yal::backend::c::BackendOptions optionsBackend(errorReporter);
            optionsBackend.arch = "x86_64";
            optionsBackend.buildDir = argv[2];
            optionsBackend.config = "yalc";
            optionsBackend.compilerBin = "/usr/bin/clang";
            optionsBackend.projectRootDir = realPathStr;
            backend.execute(optionsBackend, *module);
        }

    }

    if (!errorReporter.empty()) {
        yal::ErrorPrinter errPrinter(stdoutStream, sourceManager);
        errPrinter.enableColorCodes(true);
        errPrinter.print(errorReporter);
    }
    return module != nullptr && !errorReporter.hasErrors()
            ? EXIT_SUCCESS : EXIT_FAILURE;
}
