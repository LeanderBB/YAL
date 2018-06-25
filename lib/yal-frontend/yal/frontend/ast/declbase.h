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

#include "yal/frontend/ast/asttypes.h"
#include "yal/frontend/types/type.h"
#include "yal/frontend/types/identifier.h"
#include "yal/io/sourcemanager.h"
#include "yal/util/allocator/allocatorstack.h"
#include "yal/util/cast.h"
#include "yal/util/stringref.h"

#include <optional>

namespace yal::frontend {

    class Module;
    class AstVisitor;
    class Type;
    class TypeContext;
    class DeclScope;

    class DeclBase {
    protected:
        using DeclScopeOpt = std::optional<DeclScope*>;

        DeclBase(Module& module,
                 const AstType type,
                 const SourceInfo& sourceInfo,
                 DeclScope& scope);

        DeclBase(Module& module,
                 const AstType type);
    public:

        YAL_NO_COPY_CLASS(DeclBase);

        virtual ~DeclBase();

        const SourceInfo& getSourceInfo() const {
            return m_sourceInfo;
        }

        AstType getAstType() const {
            return m_astType;
        }

        Module& getModule() {
            return m_module;
        }

        const Module& getModule() const {
            return m_module;
        }

        const DeclScopeOpt getScopeWhereDeclared() const {
            return m_scopeWhereDecl;
        }

        void setScopeWhereDeclared(DeclScopeOpt scope);

        bool isVariableDecl() const;

        virtual void acceptVisitor(AstVisitor& visitor) = 0;

    protected:
        Module& m_module;
        DeclScopeOpt m_scopeWhereDecl;
        const AstType m_astType;
        const SourceInfo m_sourceInfo;
        Identifier m_identifier;
    };


    class DeclNamed : public DeclBase {
    public:

        StringRef getName() const {
            return m_identifier.getName();
        }

        const Identifier& getIdentifier() const {
            return m_identifier;
        }
    protected:
        DeclNamed(Module& module,
                 const AstType type,
                 const Identifier &identifier,
                 const SourceInfo& sourceInfo,
                 DeclScope& scope);

        DeclNamed(Module& module,
                 const AstType type,
                 const Identifier &identifier);

    protected:
        Identifier m_identifier;
    };
}

namespace yal {
    template<>
    struct cast_typeid<yal::frontend::DeclBase> {
        typedef yal::frontend::AstType type;
    };

    template <>
    inline yal::frontend::AstType
    get_typeid_instance<yal::frontend::DeclBase>(const yal::frontend::DeclBase& decl) {
        return decl.getAstType();
    }

    template<>
    struct cast_typeid<yal::frontend::DeclNamed> {
        typedef yal::frontend::AstType type;
    };

    template <>
    inline yal::frontend::AstType
    get_typeid_instance<yal::frontend::DeclNamed>(const yal::frontend::DeclNamed& decl) {
        return decl.getAstType();
    }
}
