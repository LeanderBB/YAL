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
#include <string>

namespace yal {

    class TypeRegistry;

    class Type {
        friend class TypeRegistry;
    public:
        enum class Kind {
#define YAL_AST_TYPE(type) type,
#include "yal/ast/typelist.def"
#undef YAL_AST_TYPE
        };

        Type(const Kind kind);

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


        uint32_t getSizeBytes() const {
            return m_sizeBytes;
        }

        const char* getName() const {
            return m_name.c_str();
        }
    protected:

        enum Flags {
            kFlagModuleDependent = 1 << 0,
            kFlagExternalType = 1 << 1,
            kFlagModuleType = 1 << 2,
            kFlagDefined = 1 << 3
        };

    protected:
        const Kind m_kind;
        std::string m_name;
        uint32_t m_sizeBytes = 0;
        uint32_t m_flags = 0;
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
        bool isValid() const;

    private:
        enum Flags {
            kFlagMutable = 1<< 0,
            kFlagImmutable = 1 << 1,
            kFlagReference = 1 << 2,
            kFlagPointer = 1 << 3
        };

    private:
        uint32_t m_flags = 0;
    };


    class QualType {
    public:

        bool isValid() const {
            return m_qualifier.isValid() && m_type != nullptr;
        }

        const Qualifier& getQualifier() const {
            return m_qualifier;
        }

        void setQualifier(const Qualifier& qualifier) {
            m_qualifier = qualifier;
        }

        const Type* getType() const {
            return m_type;
        }

        void setType(const Type* type) {
            m_type = type;
        }
    private:
        Qualifier m_qualifier;
        const Type* m_type = nullptr;
    };

    inline QualType MakeQualType(const Qualifier& qualifier,
                                  const Type* type) {
        QualType qt;
        qt.setQualifier(qualifier);
        qt.setType(type);
        return qt;
    }
}
