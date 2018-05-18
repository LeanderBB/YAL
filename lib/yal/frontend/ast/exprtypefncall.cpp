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

#include "yal/frontend/ast/exprtypefncall.h"
#include "yal/frontend/ast/astvisitor.h"
#include "yal/frontend/types/typefunction.h"
#include "yal/frontend/module.h"

namespace yal::frontend {

    using namespace frontend;

    ExprTypeFnCall::ExprTypeFnCall(Module& module,
                                   const SourceInfo& srcInfo,
                                   StmtExpression* expression,
                                   const STIdentifier &functionName,
                                   ExprList&& functionArgs):
        ExprFnCall(module,
                   AstType::ExprTypeFnCall,
                   srcInfo,
                   nullptr,
                   std::move(functionArgs)),
        m_expression(expression == nullptr
                     ? StmtExpressionOpt() :
                       StmtExpressionOpt(expression)),
        m_functionName(&functionName){

    }

    ExprTypeFnCall::ExprTypeFnCall(Module& module,
                                   const SourceInfo& srcInfo,
                                   const TypeFunction *functionType,
                                   ExprList&& functionArgs):
        ExprFnCall(module,
                   AstType::ExprTypeFnCall,
                   srcInfo,
                   functionType,
                   std::move(functionArgs)),
        m_expression(),
        m_functionName(nullptr) {
        YAL_ASSERT(functionType->isTypeFunctionStatic());
    }

    void
    ExprTypeFnCall::setFunctionType(const TypeFunction *functionType) {
        YAL_ASSERT(functionType != nullptr && functionType->isTypeFunction());
        m_functionType = functionType;
    }

    bool
    ExprTypeFnCall::isStaticCall() const {
        return !m_expression.has_value() || m_functionName == nullptr;
    }

    void
    ExprTypeFnCall::acceptVisitor(AstVisitor& visitor) {
        visitor.visit(*this);
    }

}
