#include "yal/io/memorystream.h"
#include <algorithm>
namespace yal {

    static inline void ptrDtor(void* ptr) {
        if (ptr != nullptr) {
            free(ptr);
        }
    }

    static inline void ptrDtorVoid(void*) {
    }

    bool
    MemoryStream::open(const size_t initialSizeBytes,
                       const uint32_t mode) {
        void* ptr = malloc(initialSizeBytes);
        if (ptr != nullptr) {
            m_ptr.reset(ptr);
            m_mode = mode;
            m_offset = 0;
            m_sizeBytes = initialSizeBytes;
            m_ptr.reset(ptr);
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
                         const bool takeOwnerShip)
    {
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
            ::memcpy(buffer,m_ptr.get(), nBytesToRead);
            m_offset = newOffset;
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

        if (newOffset < m_sizeBytes && newOffset > m_offset) {
            ::memcpy(m_ptr.get(), buffer, bytes);
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
    MemoryStream::expandBuffer(const size_t newOffsetReference) {
        size_t newSize = m_sizeBytes;

        do {
            newSize += (m_sizeBytes / 4);
        } while (newSize< newOffsetReference);

        void* newPtr = ::realloc(m_ptr.get(), newSize);

        if (newPtr != nullptr) {
            m_ptr.release();
            m_ptr.reset(newPtr);
            m_sizeBytes = newSize;
        }
    }

}
