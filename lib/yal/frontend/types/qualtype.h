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

#include "yal/frontend/types/type.h"
#include "yal/frontend/types/qualifier.h"
#include "yal/util/formattypes.h"

namespace yal::frontend {

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
    inline size_t FormatType(FormatTypeArgs& loc,
                             const frontend::QualType& value) {
        const size_t bytesWritten = FormatType(loc, value.getQualifier());
        loc.ptr += bytesWritten;
        loc.length -= bytesWritten;
        return bytesWritten + FormatType(loc, *value.getType());
    }
}
