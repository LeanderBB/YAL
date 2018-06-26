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

#include "yal/frontend/parser/stexprfncall.h"

namespace yal::frontend  {

    STExprFnCall::STExprFnCall(const STIdentifier* name,
                               const ParseListExpr::Range params,
                               STParser& parser):
        STExpression(SyntaxTreeType::STExprFnCall),
        m_name(name),
        m_targetType(nullptr),
        m_expr(nullptr),
        m_params(parser.getSTContext().getStdAllocatorWrapper<ParamList::value_type>()),
        m_type(FnType::Regular) {

        parser.getExprList().moveRange(m_params, params);
    }

    STExprFnCall::STExprFnCall(const STIdentifier* name,
                               const STType *type,
                               const ParseListExpr::Range params,
                               STParser& parser):
        STExpression(SyntaxTreeType::STExprFnCall),
        m_name(name),
        m_targetType(type),
        m_expr(nullptr),
        m_params(parser.getSTContext().getStdAllocatorWrapper<ParamList::value_type>()),
        m_type(FnType::Static) {

         parser.getExprList().moveRange(m_params, params);
    }

    STExprFnCall::STExprFnCall(const STIdentifier* name,
                               const STExpression* expr,
                               const ParseListExpr::Range params,
                               STParser& parser):
        STExpression(SyntaxTreeType::STExprFnCall),
        m_name(name),
        m_targetType(nullptr),
        m_expr(expr),
        m_params(parser.getSTContext().getStdAllocatorWrapper<ParamList::value_type>()),
        m_type(FnType::Instance) {

         parser.getExprList().moveRange(m_params, params);
    }
}
