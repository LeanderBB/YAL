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
#include "yal/util/stringref.h"
#include "yal/util/cast.h"
#include "yal/util/formattypes.h"

#include <unordered_map>

namespace yal::frontend {

    class Module;
    class TypeDecl;

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

        const TypeDecl* getFunctionWithIdentifier(const Identifier& id) const;

        const TypeDecl* getFunctionWithName(const StringRef name) const;

        Kind getKind() const {
            return m_kind;
        }

        uint64_t getTypeId() const {
            return m_typeId;
        }

        bool isCastableTo(const Type* other) const;

    protected:
        virtual bool isCastableToDetail(const Type*) const {
            return false;
        }

    protected:

        void addFunction(TypeDecl* function);

    protected:
        using FunctionMap = std::unordered_map<StringRef,TypeDecl*>;
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
        unsigned m_struct:1;
    };

    class Qualifier {
    public:
        Qualifier();

        void setMutable();
        void setImmutable();
        void setReference();
        void setPointer();
        void setRequiresReplace(const bool v);
        void setLValue(const bool v);

        bool isMutable() const;
        bool isImmutable() const;
        bool isReference() const;
        bool isPointer() const;
        bool requiresReplace() const;
        bool isLValue() const;
        bool isRValue() const;

    private:
        unsigned m_mutable:1;
        unsigned m_reference:1;
        unsigned m_pointer:1;
        unsigned m_reqReplace:1;
        unsigned m_lvalue:1;
    };

    class QualType {
    public:

        static QualType Create(const Qualifier& qualifier,
                               const Type* type);

        bool isValid() const {
            return  m_type != nullptr;
        }

        const Qualifier& getQualifier() const {
            return m_qualifier;
        }

        void setQualifier(const Qualifier qual) {
            m_qualifier = qual;
        }

        const Type* getType() const {
            return m_type;
        }


        bool isMutable() const {
            return m_qualifier.isMutable();
        }

        bool isImmutable() const {
            return m_qualifier.isImmutable();
        }

        bool isReference() const {
            return m_qualifier.isReference();
        }

        bool isPointer() const {
            return m_qualifier.isPointer();
        }

        bool requiresReplace() const {
            return m_qualifier.requiresReplace();
        }

        bool isLValue() const {
            return m_qualifier.isLValue();
        }

        bool isRValue() const {
            return m_qualifier.isRValue();
        }

        bool isTriviallyCopiable() const;

        bool isMovedType() const;

    private:
        Qualifier m_qualifier;
        const Type* m_type = nullptr;
    };
}

namespace yal {


    template <>
    struct cast_typeid<frontend::Type> {
        typedef frontend::Type::Kind type;
    };

    inline frontend::Type::Kind get_typeid(const frontend::Type& type) {
        return type.getKind();
    }

    namespace frontend {
#define YAL_TYPE_MACRO(TYPE) \
    class TYPE;
#include "yal/frontend/types/typelist.def"
#undef YAL_TYPE_MACRO
    }

#define YAL_TYPE_MACRO(TYPE) \
    class TYPE;\
    template<> struct cast_typeid< frontend::TYPE >{typedef frontend::Type::Kind type;}; \
    template<> constexpr cast_typeid< frontend::TYPE >::type get_typeid< frontend::TYPE >() {return frontend::Type::Kind::TYPE ;}
#include "yal/frontend/types/typelist.def"
#undef YAL_TYPE_MACRO

    inline size_t FormatType(FormatTypeArgs& loc,
                             const frontend::Type& value) {
        return FormatType(loc, value.getIdentifier());
    }

    inline size_t FormatType(FormatTypeArgs& loc,
                             const frontend::Qualifier& value) {
        size_t bytesWritten = 0;
        FormatTypeArgs localArg = loc;
        if (value.isMutable()) {
            bytesWritten += FormatType(localArg, "mut");
            localArg = FormatTypeArgsAdvance(loc, bytesWritten);
        }

        if (value.isReference()) {
            bytesWritten += FormatType(localArg, "&");
            localArg = FormatTypeArgsAdvance(loc, bytesWritten);
        }
        bytesWritten += FormatType(localArg, " ");
        return bytesWritten;
    }


    inline size_t FormatType(FormatTypeArgs& loc,
                             const frontend::QualType& value) {
        const size_t bytesWritten = FormatType(loc, value.getQualifier());
        loc.ptr += bytesWritten;
        loc.length -= bytesWritten;
        return bytesWritten + FormatType(loc, *value.getType());
    }

    \
}
