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

#include "yal/parser/stexprvarref.h"
#include "yal/parser/syntaxtreevisitor.h"

namespace yal {

    STExprVarRef::STExprVarRef(const STIdentifier* identifier):
        STStmtExpression(SyntaxTreeType::STExprVarRef),
        m_varRef(identifier) {

    }

    void
    STExprVarRef::acceptVisitor(SyntaxTreeVisitor& visitor) const {
        visitor.visit(*this);
    }

    // -----------------------------------------------------------------------

    STExprStructVarRef::STExprStructVarRef(const STStmtExpression* expression,
                                           const STIdentifier* identitifer):
        STStmtExpression(SyntaxTreeType::STExprStructVarRef),
        m_expr(expression),
        m_varRef(identitifer) {

    }

    void
    STExprStructVarRef::acceptVisitor(SyntaxTreeVisitor& visitor) const {
        visitor.visit(*this);
    }

}
