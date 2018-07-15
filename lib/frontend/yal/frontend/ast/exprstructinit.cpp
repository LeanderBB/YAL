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

#include "yal/frontend/ast/exprstructinit.h"
#include "yal/frontend/types/typestruct.h"
#include "yal/frontend/types/qualtype.h"

namespace yal::frontend {

    ExprStructInit::ExprStructInit(Module& module,
                                   const SourceInfo& srcInfo, const Type &exprType,
                                   const TypeStruct &structType,
                                   MemberInitExprList&& memberInitList):
        StmtExpression(module, AstType::ExprStructInit, srcInfo),
        m_memberInitList(std::move(memberInitList)),
        m_typeStruct(structType) {

        Qualifier qualifier;
        qualifier.setMutable();
        qualifier.setLValue(false);
        m_qualType = QualType::Create(qualifier, &exprType);
    }

}
