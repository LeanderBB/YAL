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

#include "yal/ast/exprfloatliteral.h"
#include "yal/ast/module.h"
#include "yal/ast/typebuiltin.h"
#include "yal/ast/astvisitor.h"
#include <limits>

namespace yal {

    ExprFloatLiteral::ExprFloatLiteral(Module &module,
                                           const double literalValue):
        StmtExpression(module, AstType::ExprFloatLiteral),
        m_literalValue(literalValue) {

        const Type* decimalType = nullptr;
        if (literalValue >= static_cast<double>(std::numeric_limits<float>::lowest())
                && literalValue<= static_cast<double>(std::numeric_limits<float>::max())) {
            decimalType = module.getTypeContext().getTypeBuiltinFloat32();
        } else {
            decimalType = module.getTypeContext().getTypeBuiltinFloat64();
        }

        Qualifier qual = Qualifier();
        qual.setMutable();
        m_qualType = QualType::Create(qual,decimalType);
    }

    void
    ExprFloatLiteral::acceptVisitor(AstVisitor& visitor) {
        visitor.visit(*this);
    }
}
