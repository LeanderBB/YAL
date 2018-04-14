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
#include "yal/ast/statement.h"
namespace yal{

    class StmtExpression;
    class StmtReturn : public Statement{
    public:

        StmtReturn(frontend::Module& module);

        StmtReturn(frontend::Module& module,
                   StmtExpression* expression);

        const StmtExpression* getExpression() const {
            return m_expr;
        }

        StmtExpression* getExpression() {
            return m_expr;
        }

        void setExpression(StmtExpression* expr);

        bool hasReturnExpression() const;

        virtual void acceptVisitor(AstVisitor &visitor) override;

    private:
        StmtExpression* m_expr;
    };

}
