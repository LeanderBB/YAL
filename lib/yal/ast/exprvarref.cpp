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

#include "yal/ast/exprvarref.h"
#include "yal/ast/astvisitor.h"
#include "yal/ast/declparamvar.h"
#include "yal/ast/reftype.h"
namespace yal {

    ExprVarRef::ExprVarRef(Module& module,
                           const AstType astType,
                           const DeclVar* decl):
        StmtExpression(module, astType),
        m_declVar(decl){
        RefType* varType = decl->getVarType();
        m_qualType = varType->getQualType();
    }

    ExprVarRef::ExprVarRef(Module &module,
                           const DeclVar *decl) :
        ExprVarRef(module, AstType::ExprVarRef, decl) {
    }


    void
    ExprVarRef::acceptVisitor(AstVisitor& visitor) {
        visitor.visit(*this);
    }


    ExprVarRefSelf::ExprVarRefSelf(Module& module,
                                   const DeclVar* decl):
        ExprVarRef(module, AstType::ExprVarRefSelf, decl) {
        YAL_ASSERT(dyn_cast<DeclParamVarSelf>(decl) != nullptr);
    }

    void
    ExprVarRefSelf::acceptVisitor(AstVisitor& visitor) {
        visitor.visit(*this);
    }
}
