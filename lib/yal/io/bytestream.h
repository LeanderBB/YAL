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
#include <yal/yal.h>
#include <string>
namespace yal {
    class ByteStream {
    public:
        enum Mode {
            kModeRead = 1 << 0,
            kModeWrite= 1 << 1,
            kModeReadWrite = kModeRead | kModeWrite
        };

        enum Flags {
            kFlagSeekable = 1 << 8
        };

        virtual ~ByteStream();

        virtual size_t read(void* buffer,
                            const size_t bytes) = 0;

        virtual size_t write(const void* buffer,
                             const size_t bytes) = 0;

        virtual bool seek(const size_t position) = 0;

        virtual size_t getPosition() const = 0;

        virtual size_t getSizeBytes() const = 0;

        virtual void skipLine() = 0;

        virtual std::string readLine() = 0;

        virtual bool isSeekable() const = 0;

        virtual void close() = 0;

    };
}
