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
#include "yal/util/stringref.h"
namespace yal {
    class Module;
    class RefType;
    class DeclParamVarContainer;
    class DeclFunctionBase : public DeclBase
    {
    protected:
        DeclFunctionBase(Module& module,
                         const AstType type,
                         const StringRef functionName,
                         DeclParamVarContainer* params,
                         RefType* returnType);

    public:
        virtual ~DeclFunctionBase();

        bool hasReturnValue() const {
            return m_returnType != nullptr;
        }

        bool hasFunctionParameters() const;

        const RefType* getReturnType() const {
            return m_returnType;
        }

        const DeclParamVarContainer* getParams() const {
            return m_params;
        }

    protected:
        DeclParamVarContainer* m_params;
        RefType* m_returnType;

    };
}
