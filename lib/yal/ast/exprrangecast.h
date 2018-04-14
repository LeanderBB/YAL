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

#pragma once

#include "yal/ast/stmtexpression.h"

namespace yal {
    class RefType;
    class ExprRangeCast : public StmtExpression{
    public:
        ExprRangeCast(frontend::Module& module,
                      RefType* destType,
                      StmtExpression* expression);


        RefType* getDestType() const {
            return m_dstType;
        }

        StmtExpression* getExpression() const {
            return m_expression;
        }

        virtual void acceptVisitor(AstVisitor& visitor) override;

    private:
        RefType* m_dstType;
        StmtExpression* m_expression;
    };
}



