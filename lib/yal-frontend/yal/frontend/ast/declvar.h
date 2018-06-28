/*
 *  Copyright 2017 by Leander Beernaert (lbb-dev@pm.me)
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

#include "yal/frontend/ast/declbase.h"
#include "yal/frontend/types/qualtype.h"

#include <optional>

namespace yal::frontend {

    class StmtExpression;

    class DeclVar : public DeclNamed {
    public:
        using StmtExpressionOpt = std::optional<StmtExpression*>;

        DeclVar(Module& module,
                StringRef name,
                const SourceInfo& sourceInfo,
                DeclScope &scope,
                const QualType& varType,
                StmtExpression* expr);

        const StmtExpressionOpt getExpression() const {
            return m_expression;
        }

        StmtExpressionOpt getExpression() {
            return m_expression;
        }

        const QualType& getQualType() const {
            return m_qualType;
        }

        void setQualType(const QualType& qt);

    protected:

        DeclVar(Module& module,
                const AstType type,
                StringRef name,
                const SourceInfo& sourceInfo,
                DeclScope &scope,
                const QualType& varType,
                StmtExpression* expr);

    protected:
        QualType m_qualType;
        StmtExpressionOpt m_expression;
    };

}

namespace yal {
    template <>
    inline yal::frontend::AstType
    get_typeid_instance<yal::frontend::DeclVar>(const yal::frontend::DeclVar& decl) {
        return decl.getAstType();
    }
}
