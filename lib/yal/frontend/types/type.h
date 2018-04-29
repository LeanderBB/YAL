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

#pragma once

#include "yal/yal.h"
#include "yal/frontend/types/identifier.h"
#include "yal/io/sourcemanager.h"
#include "yal/util/stringref.h"
#include "yal/util/cast.h"
#include "yal/util/formattypes.h"

#include <unordered_map>

namespace yal::frontend {

    class Module;
    class TypeFunction;

    class Type {
        friend class TypeContext;
    public:
        enum class Kind {
#define YAL_TYPE_MACRO(type) type,
#include "yal/frontend/types/typelist.def"
#undef YAL_TYPE_MACRO
        };

        static size_t AlignTo(const size_t size,
                              const size_t alignment);

        Type(const Kind kind);

        Type(const frontend::Module*,
             const Kind kind,
             const Identifier& identifier);

        virtual ~Type();

        YAL_NO_COPY_CLASS(Type);

        bool isFunction() const;
        bool isTypeFunction() const;
        bool isTypeFunctionStatic() const;
        bool isBuilitin() const;
        bool isStruct() const;
        bool isAlias() const;
        bool isWeakAlias() const;
        bool isStrongAlias() const;

        bool isModuleDependent() const;
        bool isExternalType() const;
        bool isModuleType() const;
        bool isTriviallyCopiable() const;
        bool isFunctionTargetable() const;

        uint32_t getSizeBytes() const {
            return m_sizeBytes;
        }

        const Identifier& getIdentifier() const {
            return m_identifier;
        }

        const Module* getModule() const {
            return m_module;
        }

        const TypeFunction* getFunctionWithIdentifier(const Identifier& id) const;

        const TypeFunction* getFunctionWithName(const StringRef name) const;

        Kind getKind() const {
            return m_kind;
        }

        uint64_t getTypeId() const {
            return m_typeId;
        }

        bool isCastableTo(const Type* other) const;

        void addFunction(TypeFunction* function);

        virtual SourceInfoOpt getSourceInfo() const;

    protected:
        virtual bool isCastableToDetail(const Type*) const {
            return false;
        }

    protected:
        using FunctionMap = std::unordered_map<StringRef,TypeFunction*>;
        uint64_t m_typeId;
        const Module* m_module;
        const Kind m_kind;
        uint32_t m_sizeBytes = 0;
        Identifier m_identifier;
        FunctionMap m_typeFunctions;
        unsigned m_moduleDependent:1;
        unsigned m_moduleExternal:1;
        unsigned m_moduleType:1;
        unsigned m_defined:1;
        unsigned m_trivialCopy:1;
        unsigned m_functionTargetable:1;
        unsigned m_function:1;
        unsigned m_typefunction:1;
        unsigned m_typefunctionStatic:1;
        unsigned m_struct:1;
    };


}

namespace yal {


    template <>
    struct cast_typeid<frontend::Type> {
        typedef frontend::Type::Kind type;
    };

    template<>
    inline yal::frontend::Type::Kind
    get_typeid_instance<yal::frontend::Type>(const yal::frontend::Type& type) {
        return type.getKind();
    }

    namespace frontend {
#define YAL_TYPE_MACRO(TYPE) \
    class TYPE;
#include "yal/frontend/types/typelist.def"
#undef YAL_TYPE_MACRO
    }

#define YAL_TYPE_MACRO(TYPE) \
    template<> struct cast_typeid< yal::frontend::TYPE >{typedef yal::frontend::Type::Kind type;}; \
    template<> constexpr cast_typeid< yal::frontend::TYPE >::type get_typeid< yal::frontend::TYPE >() {return yal::frontend::Type::Kind::TYPE ;}
#include "yal/frontend/types/typelist.def"
#undef YAL_TYPE_MACRO

    inline size_t FormatType(FormatTypeArgs& loc,
                             const frontend::Type& value) {
        return FormatType(loc, value.getIdentifier());
    }

}
