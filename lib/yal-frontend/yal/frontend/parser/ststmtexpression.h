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

#include "yal/frontend/parser/ststatement.h"

namespace yal::frontend {

    class STExpression : public SyntaxTree  {

    protected:
        STExpression(const SyntaxTreeType type):
            SyntaxTree(type) {
        }
    };

    class STStmtExpression final : public STStatement {
    public:
        STStmtExpression(const STExpression* expr);

        const STExpression& getExpr() const {
            return *m_expr;
        }

        void acceptVisitor(SyntaxTreeVisitor& visitor) const override final;

    private:
        const STExpression* m_expr;
    };

}

namespace yal {
    template<>
    struct cast_typeid<frontend::STExpression> {
        typedef frontend::SyntaxTreeType type;
    };

    inline frontend::SyntaxTreeType get_typeid(const frontend::STExpression& type) {
        return type.getSyntaxTreeType();
    }
}
