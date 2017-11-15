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
#include "yal/ast/exprunaryoperator.h"

namespace yal{

    ExprUnaryOperator::ExprUnaryOperator(Module& module,
                                         const UnaryOperatorType opType,
                                         StmtExpression* expr):
        StmtExpression(module, AstType::ExprUnaryOperator),
        m_opType(opType),
        m_expr(expr) {
        // TODO: set qual type ?
    }

    void ExprUnaryOperator::setExpression(StmtExpression* expr) {
        m_expr = expr;
        // TODO: Verify!
        m_qualType = expr->getQualType();
    }
}
