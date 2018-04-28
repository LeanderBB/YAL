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


#include "yal/frontend/ast/exprrangecast.h"
#include "yal/frontend/ast/astvisitor.h"

namespace yal::frontend {

    ExprRangeCast::ExprRangeCast(frontend::Module& module,
                                 const SourceInfo &srcInfo,
                                 const QualType& destType,
                                 StmtExpression* expression):
        StmtExpression(module, AstType::ExprRangeCast, srcInfo),
        m_expression(expression){

        m_qualType = destType;
    }


    void
    ExprRangeCast::acceptVisitor(AstVisitor& visitor) {
        visitor.visit(*this);
    }
}
