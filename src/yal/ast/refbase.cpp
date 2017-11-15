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

#include "yal/ast/refbase.h"
#include "yal/ast/module.h"
namespace yal {

    void* RefBase::operator new(std::size_t bytes,
                                 Module &module) {
        ASTContext& astctx = module.getASTContext();
        void* ptr = astctx.allocate(bytes);
        YAL_ASSERT_MESSAGE(ptr != nullptr, "RefBase: Failed to allocate memory");
        return ptr;
    }

    RefBase::RefBase(Module& module,
                     const ASTType astType,
                     const Type *type):
        m_module(module),
        m_astType(astType),
        m_type(type){

    }


    RefBase::~RefBase() {

    }
}
