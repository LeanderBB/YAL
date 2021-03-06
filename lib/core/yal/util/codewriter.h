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
#include "yal/yal.h"
#include "yal/util/format.h"

namespace yal {

    struct Formater;
    class ByteStream;

    class CodeWriter {
    public:

        CodeWriter(ByteStream& output);

        ~CodeWriter();

        YAL_NO_COPY_CLASS(CodeWriter);

        template <typename ...Args>
        void write(const char* format,
                   const Args&... args) {
            Format(m_formater, format, args...);
            write(m_formater.toStringRef());
        }

        void write(const StringRef ref);

        void write();

        void ident();

        void unident();

    private:
        void writeToStream();


    private:
        static constexpr uint32_t kFormaterSize = 1024;
        static constexpr size_t kBufferSize = 2048;
        size_t m_bufferOffset;
        char m_buffer[kBufferSize];
        FormaterStack<kFormaterSize> m_formater;
        ByteStream& m_stream;
        uint32_t m_identLevel;
        bool m_wasNewLine;
    };

}
