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
#include "yal/io/sourceitems.h"
#include "yal/io/filestream.h"
namespace yal {

    bool
    SourceItemFile::open(const char* path) {
        m_stream.close();
        m_filePath.clear();
        FileStream fstream;
        // Read file into memory stream
        if (fstream.open(path, FileStream::kModeRead)) {
            return open(fstream, path);
        }
        return false;
    }

    bool
    SourceItemFile::open(FileStream& stream,
                         const char *path) {
        if (m_stream.create(stream)) {
            m_filePath = path;
            return true;
        }
        return false;
    }

    const MemoryStream &
    SourceItemFile::getByteStream() const {
        return m_stream;
    }

    const StringRef
    SourceItemFile::getPath() const {
        return m_filePath;
    }


    SourceItemStringRef::SourceItemStringRef(const StringRef str):
        SourceItemStringRef(str, "unknown") {
    }

    SourceItemStringRef::SourceItemStringRef(const StringRef str,
                                             const StringRef path):
        m_string(str),
        m_path(path),
        m_stream() {
        YAL_ASSERT(str.size() != 0);
        m_stream.attach(str.data(), str.size(), false);
    }

    const MemoryStream&
    SourceItemStringRef::getByteStream() const {
        return m_stream;
    }

    const StringRef
    SourceItemStringRef::getPath() const {
        return m_path;
    }
}
