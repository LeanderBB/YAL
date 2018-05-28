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

#include "yal/frontend/ast/declparamvar.h"

#include "yal/frontend/module.h"
#include "yal/frontend/ast/astcontext.h"
#include "yal/frontend/ast/astvisitor.h"

namespace yal::frontend {

    DeclParamVar::DeclParamVar(Module& module,
                               const AstType astType,
                               StringRef name,
                               const SourceInfo &sourceInfo,
                               DeclScope& scope,
                               const QualType& type) :
        DeclVar(module,
                astType,
                name,
                sourceInfo,
                scope,
                type,
                nullptr){
    }

    DeclParamVar::DeclParamVar(Module &module,
                               StringRef name,
                               const SourceInfo &sourceInfo,
                               DeclScope& scope,
                               const QualType& type):
        DeclParamVar(module,
                     AstType::DeclParamVar,
                     name,
                     sourceInfo,
                     scope,
                     type){

    }

    void
    DeclParamVar::acceptVisitor(AstVisitor& visitor) {
        visitor.visit(*this);
    }


    DeclParamVarSelf::DeclParamVarSelf(Module& module,
                                       const SourceInfo &sourceInfo,
                                       DeclScope &scope,
                                       const QualType &refType):
        DeclParamVar(module,
                     AstType::DeclParamVarSelf,
                     "self",
                     sourceInfo,
                     scope,
                     refType) {

    }

    void
    DeclParamVarSelf::acceptVisitor(AstVisitor& visitor) {
        visitor.visit(*this);
    }

}
