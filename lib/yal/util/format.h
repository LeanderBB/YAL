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
#include <cstring>
#include <cstddef>

#include "yal/util/formattypes.h"
namespace yal{


    template <size_t size>
    struct Formater {
        const char* string = nullptr;
        size_t stringLen = 0;
        size_t stringPos = 0;
        size_t bufferPos = 0;
        char buffer[size];

        StringRef toStringRef() const {
            return StringRef(buffer, bufferPos);
        }
    };



    template <size_t size>
    size_t FormatImpl(Formater<size>& formater) {
        const size_t bufferSize =sizeof(formater.buffer) ;
        const size_t leftOverSize = bufferSize - formater.bufferPos;
        const size_t leftOverString = formater.stringLen - formater.stringPos;

        if (leftOverString > 0
                && leftOverSize > formater.bufferPos
                && leftOverSize <= bufferSize) {
            const size_t copySize = std::min(leftOverString, leftOverSize);

            memcpy(formater.buffer + formater.bufferPos,
                   formater.string + formater.stringPos,
                   copySize);
            formater.bufferPos += copySize;
        }
        return formater.bufferPos;
    }

    template <size_t size, typename T, typename ...Args>
    size_t FormatImpl(Formater<size>& formater,
                      const T& arg,
                      const Args&... other) {
        const char* curString = formater.string + formater.stringPos;
        const size_t remainingStrSize = formater.stringLen - formater.stringPos;

        // check if we filled out our buffer
        if (formater.bufferPos == sizeof(formater.buffer)) {
            return formater.bufferPos;
        }

        // find marker
        const char* marker =
                static_cast<const char*>(::memchr(curString, '%',remainingStrSize));

        if (marker == nullptr) {
            return FormatImpl(formater);
        }

        // copy remaining text from marker to buffer
        const size_t markerPosDiff =  (reinterpret_cast<const uint8_t* >(marker) -
                                      reinterpret_cast<const uint8_t*>(curString));

        if (markerPosDiff > 0) {
            ::memcpy(formater.buffer + formater.bufferPos,
                     curString,
                     markerPosDiff);
            formater.bufferPos += markerPosDiff;
        }
        formater.stringPos += markerPosDiff + 1;

        FormatTypeArgs formatLoc {
            sizeof(formater.buffer) -formater.bufferPos,
                    formater.buffer + formater.bufferPos};

        const size_t bytesWritten = FormatType(formatLoc, arg);
        formater.bufferPos += bytesWritten;

        return FormatImpl(formater, other...);
    }

    template <size_t size, typename ...Args>
    size_t Format(Formater<size>& formater,
                  const char* str,
                  const Args&... other) {
        formater.bufferPos = 0;
        formater.string = str;
        formater.stringLen = ::strlen(str);
        formater.stringPos = 0;
        return FormatImpl(formater,other...);
    }




}
