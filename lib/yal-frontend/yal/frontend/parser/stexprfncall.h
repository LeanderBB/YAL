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

#include "yal/frontend/parser/ststmtexpression.h"
#include "yal/frontend/parser/stparser.h"
namespace yal::frontend {

    class STExpression;
    class STExprFnCall final : public STExpression {
    public:
        using ParamList = STVector<const STExpression*>;

        enum class FnType {
            Regular,
            Static,
            Instance
        };

        STExprFnCall(const STIdentifier* name,
                     const ParseListExpr::Range params,
                     STParser& parser);

        STExprFnCall(const STIdentifier* name,
                     const STType* type,
                     const ParseListExpr::Range params,
                     STParser& parser);

        STExprFnCall(const STIdentifier* name,
                     const STExpression* expr,
                     const ParseListExpr::Range params,
                     STParser& parser);

        const STIdentifier& getName() const {
            return *m_name;
        }

        const STType* getTargetType() const {
            return m_targetType;
        }

        const STExpression* getExpr() const {
            return m_expr;
        }

        const ParamList& getParams() const {
            return m_params;
        }

        FnType getFunctionType() const {
            return m_type;
        }

    private:
        const STIdentifier* m_name;
        const STType* m_targetType;
        const STExpression* m_expr;
        ParamList m_params;
        const FnType m_type;
    };

}
