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
#include <gtest/gtest.h>
#include <yal/yal.h>
#include <yal/util/allocator/allocatorstack.h>
#include <limits>
#include <yal/frontend/module.h>
#include <yal/frontend/modulemanager.h>
#include <yal/ast/declfunction.h>
#include <yal/ast/declbase.h>
#include <yal/ast/reftype.h>
#include <yal/io/sourcemanager.h>


class CastTest : public ::testing::Test{

public:

    virtual void SetUp() override {
        m_module = m_moduleManager.createNew("TestModule",
                                             yal::SourceManager::Handle());
    }


    yal::frontend::ModuleManager m_moduleManager;
    yal::frontend::Module* m_module;
};





TEST_F(CastTest, TestDynCast) {
   std::string fnName = "someFunction";
   yal::DeclBase* decl = m_module->newASTNode<yal::DeclFunction>(fnName,
                                                                 nullptr,
                                                                 nullptr,
                                                                 nullptr,
                                                                 nullptr);

   yal::DeclFunction* declFunction =
           yal::dyn_cast<yal::DeclFunction>(decl);
   EXPECT_NE(declFunction, nullptr);

   yal::DeclFunction* sameCase =
           yal::dyn_cast<yal::DeclFunction>(declFunction);

   EXPECT_NE(sameCase, nullptr);
}
