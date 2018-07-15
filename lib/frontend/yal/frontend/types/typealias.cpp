/*
 *  Copyright 2018 by Leander Beernaert (lbb-dev@pm.me)
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

#include "yal/frontend/types/typealias.h"
#include "yal/frontend/module.h"
#include "yal/frontend/parser/stdeclalias.h"

namespace yal::frontend {

    TypeAliasWeak::TypeAliasWeak(const Module& module,
                                 const STDeclAlias& decl,
                                 Type& aliasedType):
        Type(&module, Kind::TypeAliasWeak,
             Identifier( decl.getName().getString(), module)),
        m_aliasedType(aliasedType),
        m_stDecl(decl),
        m_decl(nullptr) {
        m_functionTargetable = m_aliasedType.isFunctionTargetable();
    }


    SourceInfoOpt
    TypeAliasWeak::getSourceInfo() const {
        return m_stDecl.getSourceInfo();
    }

    bool
    TypeAliasWeak::isCastableToAutoImpl(const Type& other) const {
        return m_aliasedType.isCastableToAuto(other);
    }

    bool
    TypeAliasWeak::isCastableToRequestImpl(const Type& other) const {
        return m_aliasedType.isCastableToRequest(other);
    }

    const TypeFunction*
    TypeAliasWeak::getFunctionWithIdImpl(const Identifier& id) const {
        return m_aliasedType.getFunctionWithIdentifier(id);
    }

    const TypeFunction*
    TypeAliasWeak::getFunctionWithNameImpl(const StringRef name) const {
        return m_aliasedType.getFunctionWithName(name);
    }

    const Type&
    TypeAliasWeak::resolve() const {
        return m_aliasedType;
    }

    // Type Alias Strong ---------------------------------------------------

    TypeAliasStrong::TypeAliasStrong(const Module& module,
                                     const STDeclAlias& decl,
                                     Type& aliasedType):
        TypeTargetable(&module, Kind::TypeAliasStrong,
             Identifier( decl.getName().getString(), module)),
        m_aliasedType(aliasedType),
        m_stDecl(decl),
        m_decl(nullptr) {
        m_functionTargetable = m_aliasedType.isFunctionTargetable();
    }

    SourceInfoOpt
    TypeAliasStrong::getSourceInfo() const {
        return m_stDecl.getSourceInfo();
    }

    bool
    TypeAliasStrong::isCastableToAutoImpl(const Type&) const {
        // Only allow autocast to same type
        return false;
    }

    bool
    TypeAliasStrong::isCastableToRequestImpl(const Type& other) const {
        // allow strong aliases to be cast to their original type
        return other.getTypeId() == m_aliasedType.getTypeId();
    }

    const TypeFunction*
    TypeAliasStrong::getFunctionWithIdImpl(const Identifier& id) const {
        // Search local this type first, before checking aliased type
        const TypeFunction* result = TypeTargetable::getFunctionWithIdImpl(id);
        return (result == nullptr)
                ? m_aliasedType.getFunctionWithIdentifier(id)
                : result;
    }

    const TypeFunction*
    TypeAliasStrong::getFunctionWithNameImpl(const StringRef name) const {
        // Search local this type first, before checking aliased type
        const TypeFunction* result = TypeTargetable::getFunctionWithNameImpl(name);
        return (result == nullptr)
                ? m_aliasedType.getFunctionWithName(name)
                : result;
    }

    const Type&
    TypeAliasStrong::resolve() const {
        return m_aliasedType;
    }
}

