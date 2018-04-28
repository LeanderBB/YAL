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

#include "yal/frontend/parser/stexprliterals.h"
#include "yal/frontend/parser/syntaxtreevisitor.h"

namespace yal::frontend {

    STExprBoolLiteral::STExprBoolLiteral(const StringRef value):
        STExpression(SyntaxTreeType::STExprBoolLiteral),
        m_value(value) {

    }

    void
    STExprBoolLiteral::acceptVisitor(SyntaxTreeVisitor& visitor) const {
        visitor.visit(*this);
    }

    // -----------------------------------------------------------------------

    STExprIntegerLiteral::STExprIntegerLiteral(const StringRef value):
        STExpression(SyntaxTreeType::STExprIntegerLiteral),
        m_value(value){

    }

    void
    STExprIntegerLiteral::acceptVisitor(SyntaxTreeVisitor& visitor) const {
        visitor.visit(*this);
    }

    // -----------------------------------------------------------------------

    STExprFloatLiteral::STExprFloatLiteral(const StringRef value):
        STExpression(SyntaxTreeType::STExprIntegerLiteral),
        m_value(value){

    }

    void
    STExprFloatLiteral::acceptVisitor(SyntaxTreeVisitor& visitor) const {
        visitor.visit(*this);
    }
}
