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

#pragma once

#include "yal/yal.h"
#include "yal/ast/identifier.h"
#include "yal/util/stringref.h"

namespace yal {

    class TypeRegistry;
    class Module;
    class DeclBase;

    class Type {
        friend class TypeRegistry;
    public:
        enum class Kind {
#define YAL_AST_TYPE(type) type,
#include "yal/ast/typelist.def"
#undef YAL_AST_TYPE
        };

        Type(const Kind kind);

        Type(const Module*,
             const Kind kind,
             const StringRef name);

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

        uint32_t getSizeBytes() const {
            return m_sizeBytes;
        }

        const StringRef getName() const;

        const Identifier& getIdentifier() const {
            return m_identifier;
        }

        const Module* getModule() const {
            return m_module;
        }

        const DeclBase* getDeclNode() const {
            return m_declNode;
        }


    protected:
        const Module* m_module;
        const DeclBase* m_declNode;
        const Kind m_kind;
        uint32_t m_sizeBytes = 0;
        StringRef m_name;
        Identifier m_identifier;
        unsigned m_moduleDependent:1;
        unsigned m_moduleExternal:1;
        unsigned m_moduleType:1;
        unsigned m_defined:1;
        unsigned m_trivialCopy:1;
    };


    class Qualifier {
    public:

        void setMutable();
        void setImmutable();
        void setReference();
        void setPointer();

        bool isMutable() const;
        bool isImmutable() const;
        bool isReference() const;
        bool isPointer() const;

    private:
        unsigned m_mutable:1;
        unsigned m_reference:1;
        unsigned m_pointer:1;
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

        const Type* getType() const {
            return m_type;
        }
    private:
        Qualifier m_qualifier;
        const Type* m_type = nullptr;
    };

}