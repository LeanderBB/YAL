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

#include "yal/ast/declbase.h"

namespace yal {

    class RefType;
    class StmtExpression;
    class DeclVar : public DeclBase{
    public:
        DeclVar(Module& module,
                StringRef name,
                const DeclScope* scope,
                const Qualifier qualifier,
                RefType* varType,
                StmtExpression* expr);

        const StmtExpression* getExpression() const {
            return m_expression;
        }

        StmtExpression* getExpression() {
            return m_expression;
        }

        QualType getQualType() const {
            return m_qualType;
        }

        virtual void acceptVisitor(AstVisitor& visitor) override;

    protected:

        DeclVar(Module& module,
                const AstType type,
                const Qualifier qualifier,
                const DeclScope *scope,
                StringRef name,
                RefType* varType);

    protected:
        RefType* m_varType;
        QualType m_qualType;
        StmtExpression* m_expression;
    };

}
