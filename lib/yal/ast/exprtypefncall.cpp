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

#include "yal/ast/exprtypefncall.h"
#include "yal/ast/astvisitor.h"
#include "yal/ast/reftype.h"
#include "yal/ast/typedecl.h"
#include "yal/ast/module.h"
namespace yal {

    ExprTypeFnCall::ExprTypeFnCall(Module& module,
                                       StmtExpression* expression,
                                       const TokenInfo& functionName,
                                       ExprList *functionArgs):
        ExprFnCall(module, AstType::ExprTypeFnCall,
                   nullptr, functionArgs),
        m_expression(expression),
        m_functionNameToken(functionName){

    }

    void
    ExprTypeFnCall::setFunctionType(const TypeDecl *functionType){
        YAL_ASSERT(functionType->isTypeFunction());
        m_functionType = m_module.newASTNode<RefType>(functionType);
        SourceInfo srcinfo;
        srcinfo.handle = m_module.getSourceHandle();
        srcinfo.begin.line = m_functionNameToken.lineStart;
        srcinfo.begin.column = m_functionNameToken.columnStart;
        srcinfo.begin.streamOffset = m_functionNameToken.tokenOffsetInStream;
        srcinfo.end.line = m_functionNameToken.lineEnd;
        srcinfo.end.column = m_functionNameToken.columnEnd;
        srcinfo.end.streamOffset = m_functionNameToken.tokenOffsetInStream
                + m_functionNameToken.tokenStr.size;
        m_functionType->setSourceInfo(srcinfo);
    }

    void
    ExprTypeFnCall::acceptVisitor(AstVisitor& visitor) {
        visitor.visit(*this);
    }

}
