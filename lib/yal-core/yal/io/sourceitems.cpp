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

    // SourceItemFile --------------------------------------------------------

    SourceItemFile::SourceItemFile(const StringRef path):
        m_filePath(path.toString()) {

    }

    SourceItem::StreamPtr
    SourceItemFile::getByteStream() const {
        auto fstream = std::make_unique<FileStream>();
        // Read file into memory stream
        if (!fstream->open(m_filePath, ByteStream::kModeRead)) {
            fstream.reset();
        }
        return nullptr;
    }

    const StringRef
    SourceItemFile::getPath() const {
        return m_filePath;
    }

    // SourceItemStdInput ----------------------------------------------------
    SourceItem::StreamPtr
    SourceItemStdInput::getByteStream() const {
        auto fstream = std::make_unique<FileStream>();
        fstream->open(FileStream::StdStream::In);
        return fstream;
    }

    const StringRef
    SourceItemStdInput::getPath() const {
        return "stdin";
    }
    // SourceItemStringRef ---------------------------------------------------

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

    SourceItem::StreamPtr
    SourceItemStringRef::getByteStream() const {
        auto stream = std::make_unique<MemoryStream>();
        stream->attach(m_string.data(), m_string.size(), false);
        return stream;
    }

    const StringRef
    SourceItemStringRef::getPath() const {
        return m_path;
    }
}
