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

#include "yal/io/sourcemanager.h"
#include "yal/io/memorystream.h"

namespace yal {

    class SourceItemFile : public SourceItem {
    public:

        SourceItemFile() = default;
        ~SourceItemFile() = default;

        bool open(const char* path);

        bool open(FileStream& stream,
                  const char* path);

        const MemoryStream &getByteStream() const override;

        const char *getPath() const override;

    private:
        MemoryStream m_stream;
        std::string m_filePath;
    };
}
