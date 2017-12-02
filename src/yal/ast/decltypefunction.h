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
#include "yal/ast/declfunctionbase.h"

namespace yal {

    class DeclTypeFunction : public DeclFunctionBase {
    public:
        DeclTypeFunction(Module& module,
                         const StringRef functionName,
                         DeclParamVarContainer* params,
                         RefType* returnType,
                         RefType* destType,
                         StatementList *body);

        const RefType* getTargetType() const {
            return m_targetType;
        }

        bool isStatic() const;

        virtual void acceptVisitor(AstVisitor& visitor) override;

    protected:
        RefType* m_targetType;
    };
}
