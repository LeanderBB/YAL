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
#include "yal/transpiler/c/transpilerc.h"
#include "yal/transpiler/c/codegenc.h"
#include "yal/io/filestream.h"
#include "yal/ast/module.h"
#include "yal/util/path.h"
#include "yal/util/log.h"
#include <string>
namespace yal {

    TranspilerC::TranspilerC() {

    }

    bool
    TranspilerC::transpile(const TranspilerOptions& options,
                           Log& log,
                           Module& module,
                           SourceManager& srcManager) {

        (void) log;
        (void) srcManager;

        const std::string intermediateDir = Path::Join(options.intermediatOuputDir,"c");

        if (!Path::MakeDirectory(intermediateDir)) {
            log.error("TranspilerC: Failed to create intermediate directory '%'\n",
                      intermediateDir);
            return false;
        }

        const std::string moduleNameNoExt = Path::Join(intermediateDir,
                                                       module.getName());

        std::string moduleSource(moduleNameNoExt);
        moduleSource+= ".c";
        std::string moduleHeader(moduleNameNoExt);
        moduleHeader += ".h";

        FileStream streamSource, streamHeader;

        if (!streamSource.open(moduleSource.c_str(), FileStream::kModeWrite)) {
            log.error("TranspilerC: Failed to create module source file '%'\n",
                      moduleSource);
            return false;
        }

        if (!streamHeader.open(moduleHeader.c_str(), FileStream::kModeWrite)) {
            log.error("TranspilerC: Failed to create module header file '%'\n",
                      moduleHeader);
            return false;
        }

        CodeGenC codegen(module, streamSource, streamHeader);
        codegen.writeFwdDecls();

        return true;
    }

}
