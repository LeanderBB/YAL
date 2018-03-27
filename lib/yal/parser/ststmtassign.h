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

#include "yal/parser/syntaxtree.h"

namespace yal {

    class STStmtExpression;
    class STStmtAssign final : public SyntaxTree{
    public:

        STStmtAssign(const STStmtExpression* leftExpr,
                     const STStmtExpression* rightExpr);

        void acceptVisitor(SyntaxTreeVisitor& visitor) const override final;

        const STStmtExpression* getLeftExpr() const {
            return m_leftExpr;
        }

        const STStmtExpression* getRightExpr() const {
            return m_rightExpr;
        }

    private:
        const STStmtExpression* m_leftExpr;
        const STStmtExpression* m_rightExpr;
    };

}
