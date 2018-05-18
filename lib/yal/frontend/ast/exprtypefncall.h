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

#include "yal/frontend/ast/exprfncall.h"

#include <optional>

namespace yal::frontend {

    class STIdentifier;
    class TypeFunction;

    class ExprTypeFnCall : public ExprFnCall {
    public:
        using StmtExpressionOpt = std::optional<StmtExpression*>;

        ExprTypeFnCall(Module& module,
                       const SourceInfo &srcInfo,
                       StmtExpression* expression,
                       const STIdentifier &functionName,
                       ExprList&& functionArgs);

        ExprTypeFnCall(Module& module,
                       const SourceInfo& srcInfo,
                       const TypeFunction* functionType,
                       ExprList&& functionArgs);

        StmtExpressionOpt getExpression() const {
            return m_expression;
        }

        const STIdentifier* getFunctionName() const {
            return m_functionName;
        }

        bool isStaticCall() const;

        void setFunctionType(const TypeFunction *functionType);

        virtual void acceptVisitor(AstVisitor& visitor) override;

    private:
        StmtExpressionOpt m_expression;
        const STIdentifier* m_functionName;

    };

}
