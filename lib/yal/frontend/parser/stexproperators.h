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

#pragma once

#include "yal/frontend/parser/ststmtexpression.h"
#include "yal/frontend/types/operators.h"
namespace yal {

    class STExprUnaryOperator final : public STStmtExpression {
    public:

        STExprUnaryOperator(const UnaryOperatorType op,
                            const STStmtExpression* expr);

        void acceptVisitor(SyntaxTreeVisitor& visitor) const override final;

        UnaryOperatorType getOperatorType() const {
            return m_op;
        }

        const STStmtExpression* getExpression() const {
            return m_expr;
        }

    private:
        const UnaryOperatorType m_op;
        const STStmtExpression* m_expr;
    };


    class STExprBinaryOperator final : public STStmtExpression {
    public:

        STExprBinaryOperator(const BinaryOperatorType op,
                             const STStmtExpression* exprLeft,
                             const STStmtExpression* exprRight);

        void acceptVisitor(SyntaxTreeVisitor& visitor) const override final;

        BinaryOperatorType getOperatorType() const {
            return m_op;
        }

        const STStmtExpression* getExpressionLeft() const {
            return m_exprLeft;
        }

        const STStmtExpression* getExpressionRight() const {
            return m_exprRight;
        }
    private:
        const BinaryOperatorType m_op;
        const STStmtExpression* m_exprLeft;
        const STStmtExpression* m_exprRight;
    };

}
