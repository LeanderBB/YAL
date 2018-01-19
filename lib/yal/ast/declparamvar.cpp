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
#include "yal/ast/astvisitor.h"
namespace yal {

    DeclParamVar::DeclParamVar(Module& module,
                               const AstType astType,
                               StringRef name,
                               const DeclScope* scope,
                               const Qualifier qualifier,
                               RefType* varType) :
        DeclVar(module,astType, qualifier, scope, name, varType){
    }

    DeclParamVar::DeclParamVar(Module &module,
                               StringRef name,
                               const DeclScope *scope,
                               const Qualifier qualifier,
                               RefType *varType):
        DeclParamVar(module,AstType::DeclParamVar,name, scope, qualifier, varType){

    }

    void
    DeclParamVar::acceptVisitor(AstVisitor& visitor) {
        visitor.visit(*this);
    }


    DeclParamVarSelf::DeclParamVarSelf(Module& module,
                                       const DeclScope *scope,
                                       RefType* refType):
        DeclParamVar(module,
                     AstType::DeclParamVarSelf,
                     "self",
                     scope,
                     Qualifier(),
                     refType) {

    }

    void
    DeclParamVarSelf::acceptVisitor(AstVisitor& visitor) {
        visitor.visit(*this);
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

    void
    DeclParamVarContainer::acceptVisitor(AstVisitor& visitor) {
        visitor.visit(*this);
    }
}
