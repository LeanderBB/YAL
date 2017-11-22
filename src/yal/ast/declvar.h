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
                const Qualifier qualifier,
                const RefType* varType,
                StmtExpression* expr);

        const RefType* getVarType() const {
            return m_varType;
        }

        const StmtExpression* getExpression() const {
            return m_expression;
        }

        StmtExpression* getExpression() {
            return m_expression;
        }

        Qualifier getQualifier() const  {
            return m_qualifier;
        }

    protected:

        DeclVar(Module& module,
                const AstType type,
                const Qualifier qualifier,
                StringRef name,
                const RefType* varType);

    protected:
        const RefType* m_varType;
        Qualifier m_qualifier;
        StmtExpression* m_expression;
    };

}
