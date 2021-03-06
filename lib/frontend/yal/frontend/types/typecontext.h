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

#pragma once

#include "yal/yal.h"
#include "yal/frontend/types/type.h"
#include "yal/util/allocator/allocatorstack.h"
#include "yal/util/stringref.h"

namespace yal::frontend {

    class Type;
    class TypeBuiltin;
    class Identifier;
    class Module;
    class STType;
    class STIdentifier;

    class TypeContext {
    public:

        TypeContext(const Module& module);

        bool hasType(const Identifier& identifier) const;

        bool hasType (const Type& type) const;

        Type* getByIdentifier(const Identifier& identifier) const;

        TypeBuiltin* getTypeBuiltinBool() const {
            return m_typeBool;
        }

        TypeBuiltin* getTypeBuiltinI8() const {
            return m_typeI8;
        }

        TypeBuiltin* getTypeBuiltinU8() const {
            return m_typeU8;
        }

        TypeBuiltin* getTypeBuiltinI16() const {
            return m_typeI16;
        }

        TypeBuiltin* getTypeBuiltinU16() const {
            return m_typeU16;
        }

        TypeBuiltin* getTypeBuiltinI32() const {
            return m_typeI32;
        }

        TypeBuiltin* getTypeBuiltinU32() const {
            return m_typeU32;
        }

        TypeBuiltin* getTypeBuiltinI64() const {
            return m_typeI64;
        }

        TypeBuiltin* getTypeBuiltinU64() const {
            return m_typeU64;
        }

        TypeBuiltin* getTypeBuiltinFloat32() const {
            return m_typeFloat;
        }

        TypeBuiltin* getTypeBuiltinFloat64() const {
            return m_typeDouble;
        }

        template<typename T,typename... ARGS>
        T* allocateType(ARGS&& ...args) {
            T* newType = m_allocator.construct<T>(std::forward<ARGS>(args)...);
            return newType;
        }

        void registerType(Type* type);

        size_t getTypeCount() const {
            return m_types.size();
        }

        const Module& getModule() const {
            return m_module;
        }

        Type* resolveType(const STType& type) const;

        Type* resolveType(const STIdentifier& identifier) const;

        template<typename FN>
        void forEachType(FN& fn) const {
            for (auto& type : m_typeList) {
                fn(*type);
            }
        }

    private:
        friend class Module;
        template<typename FN>
        void forEachType(FN& fn) {
            for (auto& type : m_typeList) {
                fn(*type);
            }
        }


    private:
        using TypeMap = IdentifierPtrMap<Type*>;
        using TypeList = std::vector<Type*>;
        const Module& m_module;
        uint64_t m_typeIdCounter;
        AllocatorStack m_allocator;
        TypeMap m_types;
        TypeList m_typeList;
        TypeBuiltin* m_typeBool;
        TypeBuiltin* m_typeI8;
        TypeBuiltin* m_typeU8;
        TypeBuiltin* m_typeI16;
        TypeBuiltin* m_typeU16;
        TypeBuiltin* m_typeI32;
        TypeBuiltin* m_typeU32;
        TypeBuiltin* m_typeI64;
        TypeBuiltin* m_typeU64;
        TypeBuiltin* m_typeFloat;
        TypeBuiltin* m_typeDouble;
    };

}

