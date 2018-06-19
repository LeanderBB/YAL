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

#pragma once

#include "yal/frontend/parser/ststmtexpression.h"
#include "yal/frontend/types/operators.h"
namespace yal::frontend {

    class STExprUnaryOperator final : public STExpression {
    public:

        STExprUnaryOperator(const UnaryOperatorType op,
                            const STExpression* expr);

        void acceptVisitor(SyntaxTreeVisitor& visitor) const override final;

        UnaryOperatorType getOperatorType() const {
            return m_op;
        }

        const STExpression& getExpression() const {
            return *m_expr;
        }

    private:
        const UnaryOperatorType m_op;
        const STExpression* m_expr;
    };


    class STExprBinaryOperator final : public STExpression {
    public:

        STExprBinaryOperator(const BinaryOperatorType op,
                             const STExpression* exprLeft,
                             const STExpression* exprRight);

        void acceptVisitor(SyntaxTreeVisitor& visitor) const override final;

        BinaryOperatorType getOperatorType() const {
            return m_op;
        }

        const STExpression& getExpressionLeft() const {
            return *m_exprLeft;
        }

        const STExpression& getExpressionRight() const {
            return *m_exprRight;
        }
    private:
        const BinaryOperatorType m_op;
        const STExpression* m_exprLeft;
        const STExpression* m_exprRight;
    };

}
