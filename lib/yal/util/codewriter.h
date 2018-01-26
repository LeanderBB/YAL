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

#include "yal/util/format.h"

namespace yal {

    class ByteStream;

    class CodeWriter {
    public:

        CodeWriter(ByteStream& output,
                   const uint32_t identSpacing);

        template <typename ...Args>
        void write(const char* format,
                   const Args&... args) {
            Format(m_formaterText, format, args...);
            writeToStream();
        }

        void write();

        void ident();

        void uindent();

    private:

        void writeToStream();

        void writeIdent();
    private:
        enum {
            kInternalBufferSizBytes = 2048,
            kIdentBufferSize = 256};

        ByteStream& m_stream;
        FormaterStack<kIdentBufferSize> m_formaterIdent;
        FormaterStack<kInternalBufferSizBytes> m_formaterText;
        uint32_t m_identLevel;
        const uint32_t m_identSpacing;
        bool m_isNewLine;
    };

}
