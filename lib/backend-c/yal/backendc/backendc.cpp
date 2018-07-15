/*
 *  Copyright 2018 by Leander Beernaert (lbb-dev@pm.me)
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

#include "yal/backendc/backendc.h"

#include "yal/backendc/ctype.h"
#include "yal/backendc/errorsbackendc.h"
#include "yal/backendc/astvisitorcheader.h"
#include "yal/backendc/astvisitorcsource.h"
#include "yal/error/errorreporter.h"
#include "yal/frontend/module.h"
#include "yal/io/filestream.h"
#include "yal/io/memorystream.h"
#include "yal/os/path.h"
#include "yal/os/process.h"

namespace yal::backend::c {

    static const StringRef kBackendDirName("c");
    static const StringRef kBuildDirSrcName("src");
    static const StringRef kBuildDirObjName("obj");
    static const char* kExtHeader= ".h";
    static const char* kExtSource= ".c";
    static const char* kExtObject= ".o";

    BackendOptions::BackendOptions(ErrorReporter& errRep):
        errReporter(errRep),
        rvo(1),
        pragmaOnce(1),
        optimize(0),
        debugSymbols(0),
        isMSVC(0) {

    }

    bool
    BackendC::execute(const BackendOptions& options,
                      frontend::Module &module) {
        (void) options;
        (void) module;

        // Module Path
        StringRef moduleName = module.getName();
        if (const size_t pos = moduleName.findLastOf(':'); pos != StringRef::npos) {
            moduleName = moduleName.subStr(pos + 1);
        }

        const StringRef moduleFullPath = module.getPath();
        const StringRef pathToModule = Path::GetPath(moduleFullPath);

        StringRef moduleRelativePath;
        if (Path::IsAbsolute(pathToModule)) {
            moduleRelativePath = pathToModule.subStr(options.projectRootDir.size(),
                                                   pathToModule.size() - options.projectRootDir.size());
        } else {
            moduleRelativePath = pathToModule;
        }

        // Directories
        const std::string buildRoot = Path::JoinV(options.buildDir,
                                                  kBackendDirName,
                                                  options.arch,
                                                  options.config);
        const std::string buildRootSrc = Path::JoinV(StringRef(buildRoot),
                                                     kBuildDirSrcName);
        const std::string buildRootObj = Path::JoinV(StringRef(buildRoot),
                                                     kBuildDirObjName);

        const std::string moduleDirSrc = Path::Join(buildRootSrc, moduleRelativePath);
        const std::string moduleDirObj = Path::Join(buildRootObj, moduleRelativePath);

        std::string modulePathHeaderRelative = Path::Join(moduleRelativePath, moduleName);
        std::string modulePathHeader = Path::Join(moduleDirSrc, moduleName);
        std::string modulePathSource = modulePathHeader;
        modulePathHeader += kExtHeader;
        modulePathHeaderRelative += kExtHeader;
        modulePathSource += kExtSource;

        std::string modulePathobject = Path::Join(moduleDirObj, moduleName);
        modulePathobject += kExtObject;

        // create directory output dir for source
        if (!Path::MakeDirectory(moduleDirSrc)) {
            auto error = std::make_unique<ErrorMkdir>(moduleDirSrc);
            options.errReporter.report(std::move(error));
            return false;
        }

        // create directy output for object file
        if (!Path::MakeDirectory(moduleDirObj)) {
            auto error = std::make_unique<ErrorMkdir>(moduleDirObj);
            options.errReporter.report(std::move(error));
            return false;
        }

        // initialize support structures
        CTypeCache typeCache;
        typeCache.init(module.getTypeContext());

        // open header file
        {
            FileStream streamHeader;
            if (!streamHeader.open(modulePathHeader, ByteStream::kModeWrite)) {
                auto error = std::make_unique<ErrorOpenFile>(modulePathHeader, ByteStream::kModeWrite);
                options.errReporter.report(std::move(error));
                return false;
            }

            // generate header code
            AstVisitorCHeader visitorHeader(streamHeader,
                                            module,
                                            typeCache);

            if (!visitorHeader.execute(options)) {
                return false;
            }
        }

        // open source
        {
            FileStream streamSource;
            if (!streamSource.open(modulePathSource, ByteStream::kModeWrite)) {
                auto error = std::make_unique<ErrorOpenFile>(modulePathSource, ByteStream::kModeWrite);
                options.errReporter.report(std::move(error));
                return false;
            }

            // generate source code
            AstVisitorCSource visitorSource(streamSource,
                                            module,
                                            typeCache);
            if (!visitorSource.execute(options, modulePathHeaderRelative)) {
                return false;
            }
        }

        // compile source code

        // TODO: conver to OSPath
        Process process;
        ProcessArgs pargs;
        pargs.binPath = options.compilerBin;
        pargs.captureOutput = true;
        pargs.args = {
            "-std=c99",
            "-I",buildRootSrc,
            "-o",
            modulePathobject,
            "-c",
            modulePathSource
        };

        std::string msg;
        msg.reserve(512);
        ProcessResult presult = process.run(pargs);

        if (presult.status != ProcessStatus::Ok) {
            auto error = std::make_unique<ErrorExecProcess>(pargs);
            options.errReporter.report(std::move(error));
            return false;
        }

        if (presult.output.get() != nullptr) {
            char buffer[512];
            size_t bytesRead = 1;
            while (bytesRead != 0) {
                bytesRead = presult.output->read(buffer, 512);
                if (bytesRead != 0) {
                    msg.append(buffer, bytesRead);
                }
            }
        }
        if (process.join() != 0 || !msg.empty()) {
            auto error = std::make_unique<ErrorCCompile>(std::move(msg));
            options.errReporter.report(std::move(error));
            return false;
        }
        return true;
    }

}
