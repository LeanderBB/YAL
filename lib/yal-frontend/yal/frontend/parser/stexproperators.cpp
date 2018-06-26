/*
 *  Copyright 2018 by Leander Beernaert (lbb-dev@pm.me)
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

#include "yal/frontend/parser/stexproperators.h"

namespace yal::frontend {
    STExprUnaryOperator::STExprUnaryOperator(const UnaryOperatorType op,
                                             const STExpression* expr):
        STExpression(SyntaxTreeType::STExprUnaryOperator),
        m_op(op),
        m_expr(expr) {

    }

    // -----------------------------------------------------------------------

    STExprBinaryOperator::STExprBinaryOperator(const BinaryOperatorType op,
                                               const STExpression* exprLeft,
                                               const STExpression* exprRight):
        STExpression(SyntaxTreeType::STExprBinaryOperator),
        m_op(op),
        m_exprLeft(exprLeft),
        m_exprRight(exprRight) {

    }
}
