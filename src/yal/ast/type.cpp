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
#include "yal/ast/module.h"
#include "yal/util/stringref.h"

namespace yal {


    Type::Type(const Kind kind):
        Type(nullptr, kind, nullptr) {

    }

    Type::Type(const Module* module,
               const Kind kind,
               const StringRef name) :
        m_module(module),
        m_declNode(nullptr),
        m_kind(kind),
        m_sizeBytes(0),
        m_name(name),
        m_identifier(name, module),
        m_moduleDependent(0),
        m_moduleExternal(0),
        m_moduleType(0),
        m_defined(0),
        m_trivialCopy(0) {

        if (module != nullptr) {
            m_moduleDependent = 1;
            m_moduleType = 1;
        }
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
        return m_moduleDependent;
    }

    bool
    Type::isExternalType() const {
        return m_moduleExternal;
    }

    bool
    Type::isModuleType() const {
        return m_moduleType;
    }

    bool
    Type::isTriviallyCopiable() const {
        return m_trivialCopy;
    }

    const StringRef
    Type::getName() const {
        return StringRef(m_name);
    }

    Qualifier:: Qualifier():
        m_mutable(0),
        m_reference(0),
        m_pointer(0){

    }

    void
    Qualifier::setMutable() {
        m_mutable = 1;
    }

    void
    Qualifier::setImmutable() {
        m_mutable = 0;
    }

    void
    Qualifier::setReference() {
        m_reference = 1;
        m_pointer = 0;
    }

    void
    Qualifier::setPointer() {
        m_pointer = 1;
        m_reference = 0;
    }


    bool
    Qualifier::isMutable() const {
        return m_mutable;
    }

    bool
    Qualifier::isImmutable() const {
        return !m_mutable;
    }

    bool
    Qualifier::isReference() const {
        return m_reference;
    }

    bool
    Qualifier::isPointer() const {
        return m_pointer;
    }


    QualType
    QualType::Create(const Qualifier& qualifier,
                     const Type* type) {
        QualType qt;
        qt.m_qualifier = qualifier;
        qt.m_type = type;
        return qt;
    }

}
