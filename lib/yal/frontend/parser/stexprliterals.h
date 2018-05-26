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

#pragma once
#include "yal/frontend/parser/ststmtexpression.h"
#include "yal/frontend/lexer/tokens.h"
namespace yal::frontend {

    class STExprBoolLiteral final : public STExpression {
    public:

        STExprBoolLiteral(const StringRef value);

        StringRef getValue() const {
            return m_value;
        }

        void acceptVisitor(SyntaxTreeVisitor& visitor) const override final;
    private:
         const StringRef m_value;
    };


    class STExprIntegerLiteral final : public STExpression {
    public:

        STExprIntegerLiteral(const TokenInfo& value);

        StringRef getValue() const {
            return m_string;
        }

        Token getTokenType() const {
            return m_value.token;
        }

        void acceptVisitor(SyntaxTreeVisitor& visitor) const override final;

    private:
        const TokenInfo m_value;
        StringRef m_string;
    };

    class STExprFloatLiteral final : public STExpression {
    public:

        STExprFloatLiteral(const TokenInfo& value);

        StringRef getValue() const {
            return m_string;
        }

        Token getTokenType() const {
            return m_value.token;
        }

        void acceptVisitor(SyntaxTreeVisitor& visitor) const override final;
    private:
        const TokenInfo m_value;
        StringRef m_string;
    };
}
