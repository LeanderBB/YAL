/*
 *  Copyright 2017 by Leander Beernaert (lbb-dev@pm.me)
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

#include <cstring>
#include <cstddef>
#include <string>

namespace yal{

    class ByteStream;

    struct Formater {
        const char* string = nullptr;
        size_t stringLen = 0;
        size_t stringPos = 0;
        size_t bufferPos = 0;
        char* buffer = nullptr;
        const size_t bufferSize = 0;

        StringRef toStringRef() const {
            return StringRef(buffer, bufferPos);
        }

        void reset() {
            stringLen = 0;
            stringPos =0;
            bufferPos = 0;
        }

    protected:
        Formater(char* bufferPtr,
                 const size_t bufferPtrSize) :
            buffer(bufferPtr),
            bufferSize(bufferPtrSize) {

        }
    };

    template <size_t size>
    struct FormaterStack : Formater {
        char stackBuffer[size];
        FormaterStack() :
            Formater(stackBuffer, size){
        }
    };

    struct FormaterString : Formater {
        FormaterString(std::string& str) :
            Formater(const_cast<char*>(str.data()), str.size()){
        }
    };


    struct FormaterFromFormatArgs : Formater {
        FormaterFromFormatArgs(FormatTypeArgs& args):
            Formater(args.ptr, args.length){
        }
    };

    inline size_t FormatImpl(Formater& formater) {
        const size_t leftOverSize = formater.bufferSize - formater.bufferPos;
        const size_t leftOverString = formater.stringLen - formater.stringPos;

        if (leftOverString > 0
                && leftOverSize > formater.bufferPos
                && leftOverSize <= formater.bufferSize) {
            const size_t copySize = std::min(leftOverString, leftOverSize);

            memcpy(formater.buffer + formater.bufferPos,
                   formater.string + formater.stringPos,
                   copySize);
            formater.bufferPos += copySize;
        }
        return formater.bufferPos;
    }

    template <typename T, typename ...Args>
    inline size_t FormatImpl(Formater& formater,
                             const T& arg,
                             const Args&... other) {
        const char* curString = formater.string + formater.stringPos;
        const size_t remainingStrSize = formater.stringLen - formater.stringPos;

        // check if we filled out our buffer
        if (formater.bufferPos == formater.bufferSize) {
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
            formater.bufferSize -formater.bufferPos,
                    formater.buffer + formater.bufferPos};

        const size_t bytesWritten = FormatType(formatLoc, arg);
        formater.bufferPos += bytesWritten;

        return FormatImpl(formater, other...);
    }

    template <typename ...Args>
    size_t Format(Formater& formater,
                  const char* str,
                  const Args&... other) {
        formater.bufferPos = 0;
        formater.string = str;
        formater.stringLen = ::strlen(str);
        formater.stringPos = 0;
        return FormatImpl(formater,other...);
    }

    template <typename ...Args>
    size_t FormatAppend(Formater& formater,
                        const char* str,
                        const Args&... other) {
        formater.string = str;
        formater.stringLen = ::strlen(str);
        formater.stringPos = 0;
        return FormatImpl(formater,other...);
    }

    void FormatWrite(ByteStream& stream,
                     const Formater& formater);

    void FormatWriteWithLinePrefix(ByteStream& stream,
                                   const Formater& formater,
                                   const StringRef linePrefix);

    void FormatWriteWithLinePrefix(ByteStream& stream,
                                   const StringRef string,
                                   const StringRef linePrefix);

    inline void FormatReset(Formater& formater) {
        formater.bufferPos = 0;
    }
}
