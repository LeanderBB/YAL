/*
 *  Copyright 2018 by Leander Beernaert (leanderbb@gmail.com)
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

#include "yal/parser/stexproperators.h"
#include "yal/parser/syntaxtreevisitor.h"

namespace yal {
    STExprUnaryOperator::STExprUnaryOperator(const UnaryOperatorType op,
                                             const STStmtExpression* expr):
        STStmtExpression(SyntaxTreeType::STExprUnaryOperator),
        m_op(op),
        m_expr(expr) {

    }

    void
    STExprUnaryOperator::acceptVisitor(SyntaxTreeVisitor& visitor) const {
        visitor.visit(*this);
    }

    // -----------------------------------------------------------------------

    STExprBinaryOperator::STExprBinaryOperator(const BinaryOperatorType op,
                                               const STStmtExpression* exprLeft,
                                               const STStmtExpression* exprRight):
        STStmtExpression(SyntaxTreeType::STExprBinaryOperator),
        m_op(op),
        m_exprLeft(exprLeft),
        m_exprRight(exprRight) {

    }

    void
    STExprBinaryOperator::acceptVisitor(SyntaxTreeVisitor& visitor) const {
        visitor.visit(*this);
    }
}
