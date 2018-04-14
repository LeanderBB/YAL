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

#include "yal/frontend/parser/ststmtexpression.h"

namespace yal::frontend {

    class STStmtExpression;

    class STStructMemberInit {
    public:

        STStructMemberInit(const STIdentifier* name,
                           const STStmtExpression* expr);

        const STIdentifier* getName() const {
            return m_name;
        }

        const STStmtExpression* getExpr() const {
            return m_expr;
        }

        const SourceInfo getSourceInfo() const {
            return m_sourceInfo;
        }

        void setSourceInfo(const SourceInfo& sourceInfo);

    protected:
        const STIdentifier* m_name;
        const STStmtExpression* m_expr;
        SourceInfo m_sourceInfo;
    };

    class STExprStructInit final : public STStmtExpression {
    public:

        using MemberInitList = std::vector<const STStructMemberInit*>;

        STExprStructInit(const STType* structType,
                         const MemberInitList* members);

        const STType* getStructType() const {
            return m_structType;
        }

        const MemberInitList& getMemeberInitExprs() const {
            return *m_memberInits;
        }

        void acceptVisitor(SyntaxTreeVisitor& visitor) const override final;

    private:
        const STType* m_structType;
        const MemberInitList* m_memberInits;
    };
}
