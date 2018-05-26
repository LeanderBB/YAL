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

namespace yal
{
    class FileStream;
    class MemoryStream : public ByteStream
    {
    public:
        MemoryStream();

        YAL_NO_COPY_CLASS(MemoryStream);

        bool create(const size_t initialSizeBytes,
                    const uint32_t mode);

        bool attach(void* ptr,
                    const size_t sizeBytes,
                    const uint32_t mode,
                    const bool takeOwnerShip);

        bool attach(const void* ptr,
                    const size_t sizeBytes,
                    const bool takeOwnerShip);

        bool create(FileStream& stream);

        virtual size_t read(void* buffer,
                            const size_t bytes) override;

        virtual size_t write(const void* buffer,
                             const size_t bytes) override;

        virtual bool seek(const size_t position) override;

        virtual size_t getPosition() const override;

        void skipLine() override;

        std::string readLine() override;

        void* getPtr() {
            return m_ptr.get();
        }

        const void* getPtr() const {
            return m_ptr.get();
        }

        const void* getPtrAt(const size_t offset) const;

        virtual size_t getSizeBytes() const override{
            return m_sizeBytes;
        }

        virtual bool isSeekable() const override {
            return true;
        }

        void close() override;

    private:

        void expandBuffer(const size_t newOffsetReference);

    private:
        using PtrType = std::unique_ptr<void, void(*)(void*)>;
        PtrType m_ptr;
        size_t m_sizeBytes;
        size_t m_offset;
        uint32_t m_mode;
    };
}
