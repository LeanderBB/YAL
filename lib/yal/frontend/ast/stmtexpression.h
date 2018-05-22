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

#pragma once

#include "yal/frontend/ast/statement.h"
#include "yal/frontend/types/qualtype.h"

namespace yal::frontend {

    class StmtExpression : public Statement {
    public:

        StmtExpression(Module& module,
                       const AstType astType,
                       const SourceInfo& srcInfo);

        const QualType& getQualType() const {
            return m_qualType;
        }

        void setQualType(QualType qualType);

    protected:
        QualType m_qualType;
    };


    using ExprList = std::vector<StmtExpression*, StdAllocatorWrapperStack<StmtExpression*>>;
}


namespace yal {
    template<>
    struct cast_typeid<yal::frontend::StmtExpression> {
        typedef yal::frontend::AstType type;
    };

    template <>
    inline yal::frontend::AstType
    get_typeid_instance<yal::frontend::StmtExpression>(const yal::frontend::StmtExpression& stmt) {
        return stmt.getAstType();
    }

}
