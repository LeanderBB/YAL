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

#include "yal/ast/declparamvar.h"
#include "yal/ast/module.h"
#include "yal/ast/astcontext.h"
namespace yal {

    DeclParamVar::DeclParamVar(Module &module,
                               StringRef name,
                               const Qualifier qualifier,
                               const RefType *varType):
        DeclVar(module,AstType::DeclParamVar, qualifier, name, varType){

    }


    DeclParamVarContainer::DeclParamVarContainer(Module& module):
        BaseType(module){

    }

    DeclParamVarContainer::DeclParamVarContainer(Module &module,
                                                 ContainerType &&params):
        BaseType(module, std::move(params)) {

    }


    void
    DeclParamVarContainer::addDeclParam(DeclParamVar* decl) {
        m_nodes.push_back(decl);
    }
}
