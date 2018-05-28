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
#include "yal/frontend/ast/declscope.h"
#include "yal/frontend/types/qualtype.h"
#include "yal/util/stringref.h"

namespace yal::frontend {

    class TypeFunction;
    class Module;
    class Statement;
    class DeclParamVar;

    class DeclFunction : public DeclBase
    {
    public:

        using Params = std::vector<DeclParamVar*, StdAllocatorWrapperStack<DeclParamVar*>>;
        using Body = std::vector<Statement*, StdAllocatorWrapperStack<Statement*>>;

    protected:
        DeclFunction(Module& module,
                     const AstType type,
                     TypeFunction* fnType,
                     DeclScope& scope,
                     const QualType& returnType,
                     Params&& params,
                     Body&& body);

        DeclFunction(Module& module,
                     const AstType type,
                     TypeFunction* fnType,
                     DeclScope& scope,
                     const QualType& returnType);

    public:

        DeclFunction(Module& module,
                     TypeFunction* fnType,
                     DeclScope& scope,
                     const QualType& returnType,
                     Params&& params,
                     Body&& body);

        DeclFunction(Module& module,
                     TypeFunction* fnType,
                     DeclScope& scope,
                     const QualType& returnType);

        virtual ~DeclFunction() = default;

        bool hasReturnValue() const {
            return m_returnType.isValid();
        }

        bool hasFunctionParameters() const {
            return !m_params.empty();
        }

        const QualType& getReturnType() const {
            return m_returnType;
        }

        void setReturnType(const QualType& returnType);

        const Params& getParams() const {
            return m_params;
        }

        Params& getParams() {
            return m_params;
        }

        void setParams(Params&& params);

        const Body& getFunctionBody() const {
            return m_functionBody;
        }

        Body& getFunctionBody() {
            return m_functionBody;
        }

        void setBody(Body&& body);

        TypeFunction& getFunctionType() {
            return *m_fnType;
        }

        const TypeFunction& getFunctionType() const {
            return *m_fnType;
        }

        DeclScope& getFunctionScope() {
            return m_fnDeclScope;
        }

        void acceptVisitor(AstVisitor& visitor) override;

    protected:
        TypeFunction* m_fnType;
        QualType m_returnType;
        DeclScope m_fnDeclScope;
        Params m_params;
        Body m_functionBody;
    };
}

namespace yal {
    template <>
    inline yal::frontend::AstType
    get_typeid_instance<yal::frontend::DeclFunction>(const yal::frontend::DeclFunction& decl) {
        return decl.getAstType();
    }
}

