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

#include "yal/parser/stdeclfunction.h"
#include "yal/parser/syntaxtreevisitor.h"

namespace yal {

    STDeclParam::STDeclParam(const STIdentifier* name,
                                   const STQualType *type):
        m_name(name),
        m_type(type) {

    }

    void STDeclParam::setSourceInfo(const SourceInfo &sourceInfo) {
        m_sourceInfo = sourceInfo;
    }

    STDeclFunction::STDeclFunction(const STIdentifier* functionName,
                                   const STIdentifier* functionTarget,
                                   const STQualType* returnType,
                                   Params &&params,
                                   Body &&statements):
        SyntaxTree(SyntaxTreeType::STDeclFunction),
        m_functionName(functionName),
        m_functionTarget(functionTarget),
        m_returnType(returnType),
        m_params(params),
        m_body(std::move(statements)) {

    }


    void
    STDeclFunction::acceptVisitor(SyntaxTreeVisitor& visitor) const {
        visitor.visit(*this);
    }
}
