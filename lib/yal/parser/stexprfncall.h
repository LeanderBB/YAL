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
    class STExprFnCall final : public SyntaxTree {
    public:
        using ParamList = std::vector<const STStmtExpression*>;

        enum class Type {
            Regular,
            Static,
            Instance
        };

        STExprFnCall(const STIdentifier* name,
                     ParamList&& params);

        STExprFnCall(const STIdentifier* name,
                     const STIdentifier* type,
                     ParamList&& params);

        STExprFnCall(const STIdentifier* name,
                     const STStmtExpression* expr,
                     ParamList&& params);

        const STIdentifier* getName() const {
            return m_name;
        }

        const STIdentifier* getTypeName() const {
            return m_typeName;
        }

        const STStmtExpression* getExpr() const {
            return m_expr;
        }

        const ParamList& getParams() const {
            return m_params;
        }

        Type getType() const {
            return m_type;
        }

        void acceptVisitor(SyntaxTreeVisitor& visitor) const override final;

    private:
        const STIdentifier* m_name;
        const STIdentifier* m_typeName;
        const STStmtExpression* m_expr;
        const ParamList m_params;
        const Type m_type;
    };

}
