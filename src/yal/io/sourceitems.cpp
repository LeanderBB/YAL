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
#include "yal/io/sourceitems.h"

namespace yal {

    bool
    SourceItemFile::open(const char* path) {
        m_stream.close();
        if (m_stream.open(path, FileStream::kModeRead)) {
            m_filePath = path;
            return true;
        }
        return false;
    }

    const ByteStream &
    SourceItemFile::getByteStream() const {
        return m_stream;
    }

    const char *
    SourceItemFile::getPath() const {
        return m_filePath.c_str();
    }
}
