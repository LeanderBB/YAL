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

#include "yal/frontend/parser/stexprstructinit.h"
#include "yal/frontend/parser/syntaxtreevisitor.h"

namespace yal::frontend {

    STStructMemberInit::STStructMemberInit(const STIdentifier* name,
                                           const STExpression *expr):
        m_name(name),
        m_expr(expr) {

    }

    void STStructMemberInit::setSourceInfo(const SourceInfo &sourceInfo) {
        m_sourceInfo = sourceInfo;
    }

    STExprStructInit::STExprStructInit(const STType *structName,
                                       const MemberInitList *members):
        STExpression(SyntaxTreeType::STExprStructInit),
        m_structType(structName),
        m_memberInits(members) {

    }

    void
    STExprStructInit::acceptVisitor(SyntaxTreeVisitor& visitor) const {
        visitor.visit(*this);
    }
}
