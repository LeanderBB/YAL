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
    class DeclFunctionBase : public DeclBase
    {
    protected:
        DeclFunctionBase(Module& module,
                         const Kind kind,
                         const ASTType type,
                         const StringRef functionName,
                         const RefType* returnType);

    public:
        virtual ~DeclFunctionBase();

        bool hasReturnValue() const {
            return m_returnType != nullptr;
        }

        bool hasFunctionParameters() const;

        const StringRef getFunctioName() const {
            return m_name;
        }

        const RefType* getReturnType() const {
            return m_returnType;
        }

        // getFunctionParameters()...

    protected:
        const StringRef m_name;
        const RefType* m_returnType;

    };


    class DeclFunction : public DeclFunctionBase {
    public:

        static DeclFunction* Create(Module& module,
                                    const StringRef functionName,
                                    const RefType *returnType);

        DeclFunction(Module& module,
                     const StringRef functionName,
                     const RefType* returnType);

    };


    class DeclTypeFunction : public DeclFunctionBase {
    public:
        DeclTypeFunction(Module& module,
                         const StringRef functionName,
                         const RefType* returnType,
                         const RefType* destType);

        const RefType* getTypeForFunction() const;


    protected:
        const RefType* m_typeForFunction;
    };
}
