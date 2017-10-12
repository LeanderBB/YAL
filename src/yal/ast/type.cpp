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

#include "yal/ast/type.h"

namespace yal {

    Type::Type(const Kind kind) :
        m_kind(kind){

    }

    Type::~Type() {

    }


    bool
    Type::isFunction() const {
        return m_kind == Kind::TypeFunction;
    }

    bool
    Type::isTypeFunction() const {
        return m_kind == Kind::TypeFunctionType;
    }

    bool
    Type::isBuilitin() const {
        return m_kind == Kind::TypeBuiltin;
    }

    bool
    Type::isStruct() const {
        return m_kind == Kind::TypeStruct;
    }

    bool
    Type::isAlias() const {
        return m_kind == Kind::TypeAliasStrong
                || m_kind == Kind::TypeAliasWeak;
    }

    bool
    Type::isWeakAlias() const {
        return m_kind == Kind::TypeAliasWeak;
    }

    bool
    Type::isStrongAlias() const {
        return m_kind == Kind::TypeAliasStrong;
    }

    bool
    Type::isModuleDependent() const {
        return m_flags & kFlagModuleDependent;
    }

    bool
    Type::isExternalType() const {
        return m_flags & kFlagExternalType;
    }

    bool
    Type::isModuleType() const {
        return m_flags & kFlagModuleType;
    }

    void
    Qualifier::setMutable() {
        m_flags &= ~(static_cast<uint32_t>(kFlagImmutable));
        m_flags |= kFlagMutable;
    }

    void
    Qualifier::setImmutable() {
        m_flags &= ~(static_cast<uint32_t>(kFlagMutable));
        m_flags |= kFlagImmutable;
    }

    void
    Qualifier::setReference() {
        m_flags &= ~(static_cast<uint32_t>(kFlagPointer));
        m_flags |= kFlagReference;
    }

    void
    Qualifier::setPointer() {
        m_flags &= ~(static_cast<uint32_t>(kFlagReference));
        m_flags |= kFlagPointer;
    }


    bool
    Qualifier::isMutable() const {
        return m_flags & kFlagMutable;
    }

    bool
    Qualifier::isImmutable() const {
        return m_flags & kFlagImmutable;
    }

    bool
    Qualifier::isReference() const {
        return m_flags & kFlagReference;
    }

    bool
    Qualifier::isPointer() const {
        return m_flags & kFlagPointer;
    }

    bool
    Qualifier::isValid() const {
        return m_flags != 0;
    }
}
