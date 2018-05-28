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

    STExprIntegerLiteral::STExprIntegerLiteral(const TokenInfo &value):
        STExpression(SyntaxTreeType::STExprIntegerLiteral),
        m_value(value),
        m_string(value.tokenStr){
        // remove suffix
        switch (m_value.token) {
            case Token::IntegerLiteral:
            break;
        case Token::IntegerLiteralI8:
        case Token::IntegerLiteralU8:
            m_string = m_string.subStr(m_string.length() - 2);
            break;
        case Token::IntegerLiteralI16:
        case Token::IntegerLiteralU16:
        case Token::IntegerLiteralI32:
        case Token::IntegerLiteralU32:
        case Token::IntegerLiteralI64:
        case Token::IntegerLiteralU64:
            m_string = m_string.subStr(m_string.length() - 3);
            break;
        default:
            YAL_ASSERT_MESSAGE(false, "Shouldn't be reached");
        }
    }

    void
    STExprIntegerLiteral::acceptVisitor(SyntaxTreeVisitor& visitor) const {
        visitor.visit(*this);
    }

    // -----------------------------------------------------------------------

    STExprFloatLiteral::STExprFloatLiteral(const TokenInfo &value):
        STExpression(SyntaxTreeType::STExprIntegerLiteral),
        m_value(value),
        m_string(value.tokenStr){
        // remove suffix
        switch (m_value.token) {
            case Token::DecimalLiteral:
            break;
        case Token::DecimalLiteral32:
        case Token::DecimalLiteral64:
            m_string = m_string.subStr(m_string.length() - 3);
            break;
        default:
            YAL_ASSERT_MESSAGE(false, "Shouldn't be reached");
        }
    }

    void
    STExprFloatLiteral::acceptVisitor(SyntaxTreeVisitor& visitor) const {
        visitor.visit(*this);
    }
}
