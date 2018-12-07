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
#include "yal/io/memorystream.h"
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <cinttypes>
#include "yal/io/filestream.h"

namespace yal {
    typedef unsigned char u8;

    static inline void ptrDtor(void* ptr) {
        if (ptr != nullptr) {
            free(ptr);
        }
    }

    static inline void ptrDtorVoid(void*) {
    }

    bool
    MemoryStream::create(const size_t initialSizeBytes,
                         const uint32_t mode) {
        void* ptr = malloc(initialSizeBytes);
        if (ptr != nullptr) {
            m_ptr.reset(ptr);
            m_mode = mode;
            m_offset = 0;
            m_sizeBytes = initialSizeBytes;
            return true;
        }
        return false;
    }

    bool
    MemoryStream::attach(void* ptr,
                         const size_t sizeBytes,
                         const uint32_t mode,
                         const bool takeOwnerShip) {
        if (ptr != nullptr && sizeBytes != 0) {
            if (takeOwnerShip) {
                m_ptr.reset(ptr);
            } else {
                m_ptr = std::unique_ptr<void, void(*)(void*)>(ptr, ptrDtorVoid);
            }
            m_mode = mode;
            m_offset = 0;
            m_sizeBytes = sizeBytes;
            return true;
        }
        return false;
    }

    bool
    MemoryStream::attach(const void* ptr,
                         const size_t sizeBytes,
                         const bool takeOwnerShip) {
        if (ptr != nullptr && sizeBytes != 0) {
            if (takeOwnerShip) {
                m_ptr.reset(const_cast<void*>(ptr));
            } else {
                m_ptr = std::unique_ptr<void, void(*)(void*)>(
                            const_cast<void*>(ptr),
                            ptrDtorVoid);
            }
            m_mode = kModeRead;
            m_offset = 0;
            m_sizeBytes = sizeBytes;
            return true;
        }
        return false;
    }

    bool
    MemoryStream::create(FileStream& stream){

        if (stream.isStdStream()) {
            close();
            m_mode = kModeReadWrite;
            char buffer[512];
            size_t bytesRead= 0;
            size_t bytesWritten = 0;
            while(true) {
                bytesRead = stream.read(buffer, sizeof(buffer));
                if (bytesRead == 0) {
                    break;
                }
                bytesWritten = write(buffer, bytesRead);
                if (bytesWritten != bytesRead) {
                    close();
                    return false;
                }
            }
            seek(0);
            return true;
        } else {
            const size_t fileSizeBytes = stream.getSizeBytes();
            if (fileSizeBytes != 0) {
                stream.seek(0);
                if (create(fileSizeBytes, kModeRead)) {
                    const size_t bytesRead = stream.read(m_ptr.get(), fileSizeBytes);
                    if (bytesRead != fileSizeBytes) {
                        close();
                    } else {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    MemoryStream::MemoryStream():
        m_ptr(nullptr, ptrDtor),
        m_sizeBytes(0),
        m_offset(0),
        m_mode(0) {
    }

    size_t
    MemoryStream::read(void* buffer,
                       const size_t bytes) {
        if (!(m_mode & kModeRead)) {
            return 0;
        }

        const size_t newOffset = m_offset + bytes;
        size_t nBytesToRead = bytes;
        size_t bytesRead = 0;

        if (newOffset >= m_sizeBytes) {
            nBytesToRead = m_sizeBytes - m_offset;
        }

        if (nBytesToRead != 0) {
            const u8* srcBuffer = reinterpret_cast<const u8*>(m_ptr.get()) + m_offset;
            ::memcpy(buffer,srcBuffer, nBytesToRead);
            m_offset += nBytesToRead;
            bytesRead = nBytesToRead;
        }
        return bytesRead;
    }

    size_t
    MemoryStream::write(const void* buffer,
                        const size_t bytes) {
        if (!(m_mode & kModeWrite)) {
            return 0;
        }

        size_t newOffset = m_offset + bytes;
        size_t bytesWritten = 0;

        if (newOffset > m_sizeBytes) {
            expandBuffer(newOffset);
        }

        if (newOffset <= m_sizeBytes && newOffset > m_offset) {
            u8* dstBuffer = reinterpret_cast<u8*>(m_ptr.get()) + m_offset;
            ::memcpy(dstBuffer, buffer, bytes);
            m_offset = newOffset;
            bytesWritten = bytes;
        }
        return bytesWritten;
    }

    bool
    MemoryStream::seek(const size_t position) {
        m_offset = std::min(position, m_sizeBytes);
        return true;
    }

    size_t
    MemoryStream::getPosition() const {
        return m_offset;
    }

    void
    MemoryStream::skipLine() {
        const u8* srcBuffer = reinterpret_cast<const u8*>(m_ptr.get());
        while (m_offset < m_sizeBytes) {
            if (srcBuffer[m_offset]=='\n') {
                m_offset++;
                break;
            }
            m_offset++;
        }
    }

    std::string
    MemoryStream::readLine() {
        std::string result;
        size_t lineOffset = m_sizeBytes;
        const u8* srcBuffer = reinterpret_cast<const u8*>(m_ptr.get());
        const u8* memFindResult = reinterpret_cast<const uint8_t*>(
                    ::memchr(&srcBuffer[m_offset],
                             '\n',
                             m_sizeBytes - m_offset));

        if (memFindResult != nullptr) {
            lineOffset = static_cast<size_t>(memFindResult - srcBuffer) + 1;
        }

        if (lineOffset > m_offset) {
            const size_t lineSize = lineOffset - m_offset;
            result.resize(lineSize,'\0');
            ::memcpy(&result[0], srcBuffer + m_offset, lineSize -1);
            m_offset = lineOffset + 1;
        }
        return result;
    }

    const void*
    MemoryStream::getPtrAt(const size_t offset) const {
        if (offset < m_sizeBytes) {
            return reinterpret_cast<const u8*>(m_ptr.get()) + offset;
        }
        return nullptr;
    }

    void
    MemoryStream::expandBuffer(const size_t newOffsetReference) {
        size_t newSize = m_sizeBytes == 0 ? newOffsetReference : m_sizeBytes;

        do {
            newSize += (m_sizeBytes / 4);
        } while (newSize< newOffsetReference);

        void* newPtr = ::realloc(m_ptr.get(), newSize);

        if (newPtr != nullptr) {
            if (newPtr != m_ptr.get()) {
                m_ptr.release();
                m_ptr.reset(newPtr);
            }
            m_sizeBytes = newSize;
        }
    }


    void
    MemoryStream::close() {
        m_ptr.reset(nullptr);
        m_offset = 0;
        m_mode = 0;
        m_sizeBytes =0;
    }

}
