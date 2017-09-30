#pragma once

#include <yal/io/bytestream.h>

namespace yal
{
    class MemoryStream : public ByteStream
    {
    public:
        MemoryStream();

        YAL_NO_COPY_CLASS(MemoryStream);

        bool open(const size_t initialSizeBytes,
                  const uint32_t mode);

        bool attach(void* ptr,
                    const size_t sizeBytes,
                    const uint32_t mode,
                    const bool takeOwnerShip);

        bool attach(const void* ptr,
                    const size_t sizeBytes,
                    const bool takeOwnerShip);


        virtual size_t read(void* buffer,
                            const size_t bytes) override;

        virtual size_t write(const void* buffer,
                             const size_t bytes) override;

        virtual bool seek(const size_t position) override;

        virtual size_t getPosition() const override;

        void* getPtr() {
            return m_ptr.get();
        }

        const void* getPtr() const {
            return m_ptr.get();
        }

        virtual size_t getSizeBytes() const override{
            return m_sizeBytes;
        }

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
