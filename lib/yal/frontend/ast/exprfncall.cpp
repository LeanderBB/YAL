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

#include "yal/frontend/ast/exprfncall.h"
#include "yal/frontend/ast/astvisitor.h"
#include "yal/frontend/types/typefunction.h"
namespace yal::frontend{

    ExprFnCall::ExprFnCall(Module& module,
                           const AstType astType,
                           const SourceInfo& srcInfo,
                           const TypeFunction *functionType,
                           ExprList&& functionArgs):
        StmtExpression(module, astType, srcInfo),
        m_functionType(functionType),
        m_functionArgs(std::move(functionArgs)) {

    }

    ExprFnCall::ExprFnCall(Module& module,
                           const SourceInfo &srcInfo,
                           const TypeFunction *functionType,
                           ExprList&& functionArgs):
        ExprFnCall(module,
                   AstType::ExprFnCall,
                   srcInfo,
                   functionType,
                   std::move(functionArgs)) {

    }

    void
    ExprFnCall::acceptVisitor(AstVisitor& visitor) {
        visitor.visit(*this);
    }

}
