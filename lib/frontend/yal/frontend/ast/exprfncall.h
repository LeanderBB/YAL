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

namespace yal::frontend {

    class TypeFunction;

    class ExprFnCall : public StmtExpression {

    protected:
        ExprFnCall(Module& module,
                   const AstType astType,
                   const SourceInfo &srcInfo,
                   const TypeFunction* functionType,
                   ExprList&& functionArgs);
    public:

        ExprFnCall(Module& module,
                   const SourceInfo& srcInfo,
                   const TypeFunction* functionType,
                   ExprList&& functionArgs);

        const TypeFunction* getFunctionType() const {
            return m_functionType;
        }

        const ExprList& getFunctionArgs() const {
            return m_functionArgs;
        }

        ExprList& getFunctionArgs() {
            return m_functionArgs;
        }

    protected:
        const TypeFunction* m_functionType;
        ExprList m_functionArgs;

    };

}
