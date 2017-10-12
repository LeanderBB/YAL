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
#include "yal/ast/declmodule.h"
#include "yal/ast/module.h"
#include "yal/ast/astcontext.h"

namespace yal {

    void* DeclModule::operator new(std::size_t bytes,
                                   Module& module) {
        ASTContext& astctx = module.getASTContext();
        void* ptr = astctx.allocate(bytes);
        YAL_ASSERT_MESSAGE(ptr != nullptr, "DeclModule: Failed to allocate memory");
        astctx.addNode(reinterpret_cast<DeclModule*>(ptr));
        return ptr;
    }


    DeclModule::DeclModule(Module& module):
        m_module(module){

    }

    DeclModule::DeclModule(Module& module,
                           std::vector<DeclBase*>&& declarations) :
        m_module(module),
        m_declartions(std::move(declarations)) {

    }

    void
    DeclModule::addDecl(DeclBase* node) {
        m_declartions.push_back(node);
    }

}
