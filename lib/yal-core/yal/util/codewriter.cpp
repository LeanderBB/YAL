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

    static const uint32_t kIdentLevelCount
    = uint32_t(sizeof(kIdentLevels) / sizeof(StringRef));

    CodeWriter::CodeWriter(ByteStream& output):
        m_stream(output),
        m_identLevel(0) {

    }

    void
    CodeWriter::write(const StringRef ref) {
        FormatWriteWithLinePrefix(m_stream, ref, kIdentLevels[m_identLevel]);
    }

    void
    CodeWriter::write() {
        const char ch = '\n';
        m_stream.write(&ch, 1);
    }

    void
    CodeWriter::ident() {
        m_identLevel ++;
        YAL_ASSERT(m_identLevel < kIdentLevelCount);
    }

    void
    CodeWriter::unident() {
        YAL_ASSERT(m_identLevel != 0);
        m_identLevel --;
    }

    void
    CodeWriter::write(Formater& formater) {
        FormatWriteWithLinePrefix(m_stream, formater, kIdentLevels[m_identLevel]);
        FormatReset(formater);
    }
}
