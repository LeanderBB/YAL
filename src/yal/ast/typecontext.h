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

#include <yal/yal.h>
#include <unordered_map>
#include <string>
#include "yal/ast/type.h"
#include "yal/util/stringref.h"
namespace yal {

    class Type;
    class TypeBuiltin;
    class TypeContext {
    public:

        TypeContext();

        bool hasType(const Type* type) const;

        void addType(std::unique_ptr<Type>&& type);

        const Type* getByName(const char* name) const;

        const TypeBuiltin* getTypeBuiltinBool() const {
            return m_typeBool;
        }

        const TypeBuiltin* getTypeBuiltinI8() const {
            return m_typeI8;
        }

        const TypeBuiltin* getTypeBuiltinU8() const {
            return m_typeU8;
        }

        const TypeBuiltin* getTypeBuiltinI16() const {
            return m_typeI16;
        }

        const TypeBuiltin* getTypeBuiltinU16() const {
            return m_typeU16;
        }

        const TypeBuiltin* getTypeBuiltinI32() const {
            return m_typeI32;
        }

        const TypeBuiltin* getTypeBuiltinU32() const {
            return m_typeU32;
        }

        const TypeBuiltin* getTypeBuiltinI64() const {
            return m_typeI64;
        }

        const TypeBuiltin* getTypeBuiltinU64() const {
            return m_typeU64;
        }

        const TypeBuiltin* getTypeBuiltinFloat() const {
            return m_typeFloat;
        }

        const TypeBuiltin* getTypeBuiltinDouble() const {
            return m_typeDouble;
        }

    private:
        const TypeBuiltin* m_typeBool;
        const TypeBuiltin* m_typeI8;
        const TypeBuiltin* m_typeU8;
        const TypeBuiltin* m_typeI16;
        const TypeBuiltin* m_typeU16;
        const TypeBuiltin* m_typeI32;
        const TypeBuiltin* m_typeU32;
        const TypeBuiltin* m_typeI64;
        const TypeBuiltin* m_typeU64;
        const TypeBuiltin* m_typeFloat;
        const TypeBuiltin* m_typeDouble;
        std::unordered_map<StringRef, std::unique_ptr<Type>> m_types;
    };

}
