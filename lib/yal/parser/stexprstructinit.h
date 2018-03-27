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

#pragma once

#include "yal/parser/syntaxtree.h"

namespace yal {

    class STStmtExpression;

    struct StrurctMemberInit {
        const STIdentifier* name;
        const STStmtExpression* expr;
    };

    class STExprStructInit final : public SyntaxTree {
    public:

        using MemberInitList = std::vector<const StrurctMemberInit*>;

        STExprStructInit(const STIdentifier* structType,
                         MemberInitList&& members);

        const STIdentifier* getStructType() const {
            return m_structType;
        }

        const MemberInitList& getMemeberInitExprs() const {
            return m_memberInits;
        }

        void acceptVisitor(SyntaxTreeVisitor& visitor) const override final;

    private:
        const STIdentifier* m_structType;
        const MemberInitList m_memberInits;
    };
}
