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

#pragma once

#include "yal/frontend/ast/stmtexpression.h"
#include "yal/frontend/types/operators.h"

namespace yal::frontend {

    class ExprUnaryOperator : public StmtExpression {
    public:

        ExprUnaryOperator(Module& module,
                          const SourceInfo &srcInfo,
                          const UnaryOperatorType opType,
                          StmtExpression* expr);

        frontend::UnaryOperatorType getOperatorType() const {
            return m_opType;
        }

        const StmtExpression* getExpression() const {
            return m_expr;
        }

        StmtExpression* getExpression() {
            return m_expr;
        }

        void setExpression(StmtExpression* expr);

    private:
        const frontend::UnaryOperatorType m_opType;
        StmtExpression* m_expr;
    };

}
