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
#pragma once
#include <gtest/gtest.h>
#include <config.h>

#include <yal/yal.h>
#include <yal/backendc/backendc.h>
#include <yal/error/errorprinter.h>
#include <yal/error/errorreporter.h>
#include <yal/frontend/frontend.h>
#include <yal/frontend/modulemanager.h>
#include <yal/io/filestream.h>
#include <yal/io/memorystream.h>
#include <yal/io/sourcemanager.h>
#include <yal/io/sourceitems.h>
#include <yal/util/log.h>

#include <string>
#include <iostream>

using FrontendOptionsType = yal::frontend::FrontendOptions;
using ModuleType = yal::frontend::Module;
using BackendType = yal::backend::c::BackendC;
using BackendOptionsType = yal::backend::c::BackendOptions;

class CompileFixture : public ::testing::Test {
public:

    CompileFixture():
        m_stdstream(),
        m_sourceManager(),
        m_moduleManager(),
        m_errorReporter(),
        m_errorPrinter(m_stdstream, m_sourceManager),
        m_frontEnd(m_errorReporter, m_sourceManager, m_moduleManager){
        const bool result = m_stdstream.open(yal::FileStream::StdStream::Out);
        if (!result) {
            YAL_ASSERT_MESSAGE(false,"Failed to open stdout stream");
        }
    }

    virtual void SetUp() override final{

    }

    virtual void TearDown() override final{
        if (!m_errorReporter.empty()) {
            yal::ErrorPrinter printer(m_stdstream, m_sourceManager);
            printer.enableColorCodes(true);
            printer.print(m_errorReporter);
        }
        m_sourceManager.clear();
        m_moduleManager.clear();
        m_errorReporter.clear();
    }


    void compile(const yal::StringRef str,
                 const yal::StringRef path) {
        auto handle = m_sourceManager.add(std::make_unique<yal::SourceItemStringRef>(str, path));
        FrontendOptionsType options;
        ModuleType* module = m_frontEnd.compile(handle, options);
        if (module == nullptr) {
            return;
        }
        BackendOptionsType optionsBackend(m_errorReporter);
        optionsBackend.compilerBin = BACKEND_C_COMPILER_BIN;
        optionsBackend.buildDir = BACKEND_C_BUILD_DIR;
        optionsBackend.config = "debug";
        optionsBackend.arch = "x84_64";
        optionsBackend.projectRootDir = BACKEND_C_PROJ_ROOT_DIR;
#if defined(BACKEND_C_ISMSVC)
        optionsBackend.isMSVC = true;
#endif
       BackendType backend;
       backend.execute(optionsBackend, *module);
    }

public:
    yal::FileStream m_stdstream;
    yal::SourceManager m_sourceManager;
    yal::frontend::ModuleManager m_moduleManager;
    yal::ErrorReporter m_errorReporter;
    yal::ErrorPrinter m_errorPrinter;
    yal::frontend::Frontend m_frontEnd;
};
