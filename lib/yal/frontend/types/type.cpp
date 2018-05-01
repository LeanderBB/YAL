/*
 *  Copyright 2018 by Leander Beernaert (leanderbb@gmail.com)
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

#include "yal/frontend/types/type.h"
#include "yal/frontend/module.h"
#include "yal/util/stringref.h"
#include "yal/frontend/types/typefunction.h"
namespace yal::frontend {

    size_t
    Type::AlignTo(const size_t size,
                  const size_t alignment) {
        const size_t unaligned =  size % alignment;
        const size_t newSize =  unaligned != 0
                ? size + (alignment - unaligned)
                : size;
        return newSize;
    }

    Type::Type(const Kind kind):
        Type(nullptr, kind, Identifier("unknown")) {

    }

    Type::Type(const Module* module,
               const Kind kind,
               const Identifier &identifier) :
        m_module(module),
        m_kind(kind),
        m_sizeBytes(0),
        m_identifier(identifier),
        m_moduleDependent(0),
        m_moduleExternal(0),
        m_moduleType(0),
        m_defined(0),
        m_trivialCopy(0),
        m_functionTargetable(0),
        m_function(0),
        m_typefunction(0),
        m_typefunctionStatic(0),
        m_struct(0){

        if (module != nullptr) {
            m_moduleDependent = 1;
            m_moduleType = 1;
        }
    }

    Type::~Type() {

    }


    bool
    Type::isFunction() const {
        return m_kind == Kind::TypeFunction && m_function == 1;
    }

    bool
    Type::isTypeFunction() const {
        return m_kind == Kind::TypeFunction && m_typefunction == 1;
    }

    bool Type::isTypeFunctionStatic() const {
        return m_kind == Kind::TypeFunction
                && m_typefunction == 1
                && m_typefunctionStatic ==1;
    }

    bool
    Type::isBuilitin() const {
        return m_kind == Kind::TypeBuiltin;
    }

    bool
    Type::isStruct() const {
        return m_kind == Kind::TypeStruct && m_struct == 1;
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
        return m_moduleDependent  == 1;
    }

    bool
    Type::isExternalType() const {
        return m_moduleExternal  == 1;
    }

    bool
    Type::isModuleType() const {
        return m_moduleType  == 1;
    }

    bool
    Type::isTriviallyCopiable() const {
        return m_trivialCopy == 1;
    }

    bool
    Type::isFunctionTargetable() const {
        return m_functionTargetable == 1;
    }

    const TypeFunction*
    Type::getFunctionWithName(const StringRef name) const {
        Identifier fnId(name,
                        m_identifier.getName(),
                        m_identifier.getModule());
        auto it = m_typeFunctions.find(&fnId);
        return it != m_typeFunctions.end() ? it->second : nullptr;
    }

    const TypeFunction*
    Type::getFunctionWithIdentifier(const Identifier& id) const {
        auto it = m_typeFunctions.find(&id);
        return it != m_typeFunctions.end() ? it->second : nullptr;
    }

    void
    Type::addFunction(TypeFunction* function) {
        YAL_ASSERT(function->isTypeFunction());
        YAL_ASSERT_MESSAGE(m_typeFunctions.find(&function->getIdentifier()) == m_typeFunctions.end(),
                           "Adding duplicate function to type");
        m_typeFunctions.insert(std::make_pair(&function->getIdentifier(),
                                              function));
    }

    SourceInfoOpt
    Type::getSourceInfo() const {
        return SourceInfoOpt();
    }

    bool
    Type::isCastableTo(const Type* other) const {
        if (m_typeId == other->getTypeId()) {
            return true;
        } else {
            return isCastableToDetail(other);
        }
    }
}
