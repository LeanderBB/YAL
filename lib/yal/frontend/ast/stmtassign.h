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

#include "yal/frontend/ast/statement.h"

namespace yal::frontend {

    class StmtExpression;

    class StmtAssign : public Statement {
    public:
        StmtAssign(frontend::Module& module,
                   const SourceInfo& srcInfo,
                   StmtExpression* destExpr,
                   StmtExpression* valueExpr);


        StmtExpression* getLeftExpr() const {
            return m_destExpr;
        }


        StmtExpression* getRightExpr() const {
            return m_valueExpr;
        }

        virtual void acceptVisitor(AstVisitor& visitor) override;

    private:
       StmtExpression* m_destExpr;
       StmtExpression* m_valueExpr;
    };

}
