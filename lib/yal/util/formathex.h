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

namespace yal {


    template <typename T>
    struct FormatHex {
        const size_t size;
        const uint8_t* ptr;

        FormatHex(const T& value):
            size(sizeof(T)),
            ptr(reinterpret_cast<const uint8_t*>(&value)) {
        }

        FormatHex(const T* value):
            size(sizeof(T*)),
            ptr(reinterpret_cast<const uint8_t*>(value)) {
        }
    };


    char ByteToHex (const uint8_t byte);

    template<typename T>
    inline size_t FormatType(FormatTypeArgs& args,
                             const FormatHex<T>& value) {
        size_t actualCopySize = value.size * 2;
        actualCopySize = std::min(actualCopySize, args.length);
        actualCopySize -= (actualCopySize % 2);
        if (actualCopySize == 0) {
            return 0;
        }

        for (size_t i = 0, j = 0; j < actualCopySize; ++i, j+=2) {
            args.ptr[j] = ByteToHex((value.ptr[i] & 0xF0) >> 4);
            args.ptr[j+1] = ByteToHex((value.ptr[i] & 0X0F));
        }
        return actualCopySize;
    }



}
