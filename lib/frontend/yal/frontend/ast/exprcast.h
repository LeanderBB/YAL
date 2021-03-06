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

#include "yal/frontend/ast/stmtexpression.h"
#include "yal/frontend/types/qualtype.h"

namespace yal::frontend {

    class ExprCast : public StmtExpression {
    public:
        ExprCast(Module& module,
                 const SourceInfo& srcInfo,
                 const QualType& qt,
                 StmtExpression* expression);

        StmtExpression& getExpression() {
            return *m_expression;
        }

        const StmtExpression& getExpression() const {
            return *m_expression;
        }

    private:
        StmtExpression* m_expression;
    };
}



