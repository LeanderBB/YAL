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

#include "yal/io/sourcemanager.h"
#include "yal/io/memorystream.h"
namespace yal {

    class SourceItemFile final : public SourceItem {
    public:

        SourceItemFile(const StringRef path);

        StreamPtr getByteStream() const override final;

        const StringRef getPath() const override final;

    private:
        const std::string m_filePath;
    };


    class SourceItemStdInput final : public SourceItem {
    public:

        StreamPtr getByteStream() const override final;

        const StringRef getPath() const override final;

    };

    class SourceItemStringRef final : public SourceItem {
    public:
        SourceItemStringRef(const StringRef str);

        SourceItemStringRef(const StringRef str,
                            const StringRef path);

        StreamPtr getByteStream() const override final;

        const StringRef getPath() const override final;
    private:
        const StringRef m_string;
        const StringRef m_path;
        MemoryStream m_stream;

    };
}
