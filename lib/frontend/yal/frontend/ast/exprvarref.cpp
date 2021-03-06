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

#include "yal/frontend/ast/exprvarref.h"
#include "yal/frontend/ast/declparamvar.h"

namespace yal::frontend {

    bool IsExprVarRef(const StmtExpression& expr) {
        return expr.getAstType() == AstType::ExprVarRef
                || expr.getAstType() == AstType::ExprVarRefSelf;
    }

    ExprVarRef::ExprVarRef(Module& module,
                           const AstType astType,
                           const SourceInfo& srcInfo,
                           const DeclVar* decl):
        StmtExpression(module, astType, srcInfo),
        m_declVar(*decl){
        m_qualType = decl->getQualType();
    }

    ExprVarRef::ExprVarRef(frontend::Module &module,
                           const SourceInfo& srcInfo,
                           const DeclVar *decl) :
        ExprVarRef(module, AstType::ExprVarRef, srcInfo, decl) {
    }

    ExprVarRefSelf::ExprVarRefSelf(frontend::Module& module,
                                   const SourceInfo& srcInfo,
                                   const DeclVar* decl):
        ExprVarRef(module, AstType::ExprVarRefSelf, srcInfo, decl) {
        YAL_ASSERT(dyn_cast<DeclParamVarSelf>(decl) != nullptr);
    }

}
