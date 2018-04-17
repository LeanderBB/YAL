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

#include "yal/util/formattypes.h"

namespace yal::frontend {

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
}


namespace yal {
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
}
