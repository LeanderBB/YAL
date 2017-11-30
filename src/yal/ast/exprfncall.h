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
#include "yal/ast/nodecontainer.h"

namespace yal {

    class RefType;
    class StatementList;
    class ExprList;
    class ExprFnCall : public StmtExpression {

    protected:
        ExprFnCall(Module& module,
                   const AstType astType,
                   RefType* functionType,
                   ExprList* functionArgs);
    public:

        ExprFnCall(Module& module,
                   RefType* functionType,
                   ExprList* functionArgs);

        RefType* getFunctionType() const {
            return m_functionType;
        }

        ExprList* getFunctionArgs() const {
            return m_functionArgs;
        }

        virtual void acceptVisitor(AstVisitor& visitor) override;

    private:
        RefType* m_functionType;
        ExprList* m_functionArgs;

    };

}
