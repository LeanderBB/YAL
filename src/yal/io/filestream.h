#pragma once
#include <yal/io/bytestream.h>
namespace yal {
    class FileStream : public ByteStream {
    public:

        static std::unique_ptr<FileStream> Open(
                const char* path,
                const uint32_t mode);

        FileStream();

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
    private:
        using FileType = std::unique_ptr<FILE, void(*)(FILE*)>;
        FileType m_file;
        size_t m_fileSizeBytes;
        uint32_t m_mode;
    };
}
