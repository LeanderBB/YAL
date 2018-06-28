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

#include "yal/util/codewriter.h"

#include "yal/io/bytestream.h"

namespace yal {

    static const char kIdentChar = char(-100);
    static const char kUnidentChar = char(-102);
    static const StringRef kIdentLevels[] = {
        "",
        "  ",
        "    ",
        "      ",
        "        ",
        "           ",
        "             ",
        "               ",
        "                 ",
        "                   ",
        "                     ",
        "                       ",
        "                         ",
        "                           ",
        "                             ",
        "                               ",
    };
#if defined(YAL_ASSERT_ENABLED)
    static const uint32_t kIdentLevelCount
    = uint32_t(sizeof(kIdentLevels) / sizeof(StringRef));
#endif

    CodeWriter::CodeWriter(ByteStream& output):
        m_bufferOffset(0),
        m_stream(output),
        m_identLevel(0),
        m_wasNewLine(false) {

    }

    CodeWriter::~CodeWriter() {
        writeToStream();
    }

    void
    CodeWriter::write(const StringRef ref) {
        size_t newOffset = ref.size() + m_bufferOffset;
        if (newOffset > kBufferSize) {
            // flush buffer
            writeToStream();
            newOffset = ref.size();
        }
        memcpy(m_buffer + m_bufferOffset, ref.data(), ref.size());
        m_bufferOffset = newOffset;
    }

    void
    CodeWriter::write() {
        size_t newOffset = 1+ m_bufferOffset;
        if (newOffset > kBufferSize) {
            // flush buffer
            writeToStream();
            newOffset = 1;
        }
        m_buffer[m_bufferOffset] = '\n';
        m_bufferOffset = newOffset;
    }

    void
    CodeWriter::ident() {
        size_t newOffset = 1+ m_bufferOffset;
        if (newOffset > kBufferSize) {
            // flush buffer
            writeToStream();
            newOffset = 1;
        }
        m_buffer[m_bufferOffset] = kIdentChar;
        m_bufferOffset = newOffset;
    }

    void
    CodeWriter::unident() {
        size_t newOffset = 1+ m_bufferOffset;
        if (newOffset > kBufferSize) {
            // flush buffer
            writeToStream();
            newOffset = 1;
        }
        m_buffer[m_bufferOffset] = kUnidentChar;
        m_bufferOffset = newOffset;
    }

    void
    CodeWriter::writeToStream() {
        size_t offset = 0;
        size_t marker = 0;
        while (offset < m_bufferOffset) {
            const char ch = m_buffer[offset];
            if (ch == kIdentChar) {
                ++m_identLevel;
                YAL_ASSERT(m_identLevel < kIdentLevelCount);
                m_stream.write(m_buffer + marker, offset - marker);
                marker = offset + 1;
            } else if(ch == kUnidentChar) {
                YAL_ASSERT(m_identLevel != 0);
                --m_identLevel;
                m_stream.write(m_buffer + marker, offset - marker);
                marker = offset + 1;
            } else if (ch == '\n') {
                m_stream.write(m_buffer + marker, offset - marker + 1);
                marker = offset + 1;
                m_wasNewLine = true;
            } else if (m_wasNewLine) {
                const StringRef& identStr = kIdentLevels[m_identLevel];
                m_stream.write(identStr.data(), identStr.size());
                m_wasNewLine = false;
            }
            ++offset;
        }

        if (offset != marker) {
            if (m_wasNewLine) {
                const StringRef& identStr = kIdentLevels[m_identLevel];
                m_stream.write(identStr.data(), identStr.size());
            }
            m_stream.write(m_buffer + marker, offset - marker );
        }

        m_bufferOffset = 0;
    }
}
