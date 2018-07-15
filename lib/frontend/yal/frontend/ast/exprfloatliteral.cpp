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

#include "yal/frontend/ast/exprfloatliteral.h"
#include "yal/frontend/module.h"
#include "yal/frontend/types/typebuiltin.h"
#include <limits>

namespace yal::frontend {

    ExprFloatLiteral::ExprFloatLiteral(Module &module,
                                       const SourceInfo& srcInfo,
                                       const double literalValue,
                                       const DecimalType type):
        StmtExpression(module, AstType::ExprFloatLiteral, srcInfo),
        m_literalValue(literalValue),
        m_decimalType(type) {

        const Type* decimalType = nullptr;
        if (type == DecimalType::Decimal32) {
            decimalType = module.getTypeContext().getTypeBuiltinFloat32();
        } else {
            decimalType = module.getTypeContext().getTypeBuiltinFloat64();
        }

        Qualifier qual = Qualifier();
        qual.setMutable();
        m_qualType = QualType::Create(qual,decimalType);
    }

}
