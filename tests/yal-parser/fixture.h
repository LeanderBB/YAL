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
#pragma once
#include <gtest/gtest.h>
#include <yal/yal.h>
#include <yal/lexer/lexer.h>
#include <yal/lexer/tokens.h>
#include <string>
#include <iostream>
#include <yal/parser/parser.h>
#include <yal/io/filestream.h>
#include <yal/io/memorystream.h>
#include <yal/util/log.h>
#include <yal/ast/modulemanager.h>
#include <yal/io/sourcemanager.h>
#include <yal/ast/astprinter.h>
#include <yal/ast/astcontext.h>
#include <yal/io/sourcemanager.h>
#include <yal/io/sourceitems.h>
#include <yal/compiler/compiler.h>

class CompileFixture : public ::testing::Test {
public:

    yal::SourceManager::Handle createSourceHanlde(const yal::StringRef str) {
        return m_sourceManager.add(std::make_unique<yal::SourceItemStringRef>(str));
    }

    virtual void SetUp() override final{
        m_stdstream.open(yal::FileStream::StdStream::Out);
        m_log = std::make_unique<yal::Log>(m_stdstream);
    }


    virtual void TearDown() override final{
        m_log.reset();
        m_stdstream.close();
        m_sourceManager.clear();
    }

public:
    yal::FileStream m_stdstream;
    std::unique_ptr<yal::Log> m_log;
    yal::SourceManager m_sourceManager;
    yal::ModuleManager m_moduleManager;
};
