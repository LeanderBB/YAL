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

#include "yal/frontend/ast/stmtexpression.h"
#include "yal/frontend/types/operators.h"

namespace yal::frontend {

    class ExprBinaryOperator : public StmtExpression {
    public:

        ExprBinaryOperator(Module& module,
                           const SourceInfo &srcInfo,
                           const BinaryOperatorType opType,
                           StmtExpression* left,
                           StmtExpression* right);

        frontend::BinaryOperatorType getOperatorType() const {
            return m_opType;
        }

        const StmtExpression* getExpressionLeft() const {
            return m_exprLeft;
        }

        StmtExpression* getExpressionLeft() {
            return m_exprLeft;
        }

        void setExpressionLeft(StmtExpression* expr);

        const StmtExpression* getExpressionRight() const {
            return m_exprRight;
        }

        StmtExpression* getExpressionRight() {
            return m_exprRight;
        }

        void setExpressionRight(StmtExpression* expr);

    private:

        void updateQualType();

    private:
        const frontend::BinaryOperatorType m_opType;
        StmtExpression* m_exprLeft;
        StmtExpression* m_exprRight;
    };

}
