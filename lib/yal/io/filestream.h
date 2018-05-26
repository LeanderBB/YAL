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
#include <yal/io/bytestream.h>
namespace yal {
    class FileStream : public ByteStream {
    public:

        enum class StdStream
        {
            In,
            Out,
            Error
        };

        FileStream();

        bool open(const StdStream stream);

        bool open(const char* path,
                  const uint32_t mode);


        YAL_NO_COPY_CLASS(FileStream);

        virtual size_t read(
                void* buffer,
                const size_t bytes
                ) override;

        virtual size_t write(
                const void* buffer,
                const size_t bytes
                ) override;

        virtual bool seek(const size_t position) override;

        virtual size_t getPosition() const override;

        virtual size_t getSizeBytes() const override;

        virtual void skipLine() override;

        virtual std::string readLine() override;

        virtual bool isSeekable() const override;

        virtual void close() override;

        bool isStdStream() const;

    private:
        using FileType = std::unique_ptr<FILE, void(*)(FILE*)>;
        FileType m_file;
        size_t m_fileSizeBytes;
        size_t m_offset;
        uint32_t m_flags;
    };
}
