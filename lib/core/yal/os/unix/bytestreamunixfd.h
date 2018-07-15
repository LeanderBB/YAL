/*
 *  Copyright 2018 by Leander Beernaert (lbb-dev@pm.me)
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

#include "yal/io/bytestream.h"

namespace yal {

    class ByteStreamUnixFd final : public ByteStream
    {
    public:

        ByteStreamUnixFd();

        ~ByteStreamUnixFd();

        YAL_NO_COPY_CLASS(ByteStreamUnixFd);

        void setFd(const int fd,
                   const uint32_t flags);

        size_t read(void* buffer,
                    const size_t bytes) override final;

        size_t write(const void* buffer,
                     const size_t bytes) override final;

        bool seek(const size_t position) override final;

        size_t getPosition() const override final;

        size_t getSizeBytes() const override final;

        void skipLine() override final;

        std::string readLine() override final;

        bool isSeekable() const override final;

        void close() override final;

    private:
        int m_fd;
        uint32_t m_flags;
    };

}
