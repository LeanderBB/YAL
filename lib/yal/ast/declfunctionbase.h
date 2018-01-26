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
    class StatementList;
    class DeclFunctionBase : public DeclBase, public DeclTyped
    {
    protected:
        DeclFunctionBase(Module& module,
                         const AstType type,
                         const StringRef functionName,
                         const DeclScope* scope,
                         DeclParamVarContainer* params,
                         RefType* returnType,
                         StatementList* body);

    public:
        virtual ~DeclFunctionBase();

        bool hasReturnValue() const {
            return m_returnType != nullptr;
        }

        bool hasFunctionParameters() const {
            return m_params != nullptr;
        }

        RefType* getReturnType() const {
            return m_returnType;
        }

        QualType getReturnQualType() const;

        void setReturnType(RefType* returnType);

        DeclParamVarContainer* getParams() const {
            return m_params;
        }

        void setParams(DeclParamVarContainer* params);

        StatementList* getFunctionBody() const {
            return m_functionBody;
        }

        void setFunctionBody(StatementList* body);

    protected:
        DeclParamVarContainer* m_params;
        RefType* m_returnType;
        StatementList* m_functionBody;

    };
}
