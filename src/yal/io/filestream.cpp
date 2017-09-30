#include "yal/io/filestream.h"
#include <cstdio>
namespace yal {
    static inline void fileDtor(FILE* file) {
        if (file != nullptr)
        {
            ::fclose(file);
        }
    }

    static inline void stdStreamDtor(FILE*) {
    }

    FileStream::FileStream():
        m_file(nullptr, fileDtor) {

    }

    bool
    FileStream::open(const StdStream stream) {

        if (stream == StdStream::In) {
            m_file = FileType(::stdin, stdStreamDtor);
            m_mode = kModeRead;
            m_fileSizeBytes = 0;
        } else if (stream == StdStream::Out) {
            m_file = FileType(::stdout, stdStreamDtor);
            m_mode = kModeWrite;
            m_fileSizeBytes = 0;
        } else if (stream == StdStream::Error){
            m_file = FileType(::stderr, stdStreamDtor);
            m_mode = kModeWrite;
            m_fileSizeBytes = 0;
        } else {
            return false;
        }
        return true;
    }

    bool
    FileStream::open(const char* path,
                     const uint32_t mode) {
        const char* fmode = nullptr;
        if ((mode & kModeReadWrite) == kModeReadWrite){
            fmode = "bw+";
        } else if (mode & kModeRead){
            fmode = "rb";
        } else {
            fmode = "rw";
        }

        FileType file(fopen(path, fmode), fileDtor);
        if (file)
        {
            // NOTE: Use OS API Perhaps?
            size_t fileSize;
            ::fseek(file.get(), 0, SEEK_END);
            fileSize = static_cast<size_t>(::ftell(file.get()));
            ::fseek(file.get(), 0, SEEK_SET);
             m_file = std::move(file);
             m_fileSizeBytes = fileSize;
             m_mode = mode;
             return true;
        }
        return false;
    }

    size_t
    FileStream::read(void* buffer,
                     const size_t bytes) {
        if (!(m_mode & kModeRead)) {
            return 0;
        }

        if (feof(m_file.get())) {
            return 0;
        }

        return ::fread(buffer, 1, bytes, m_file.get());
    }

    size_t
    FileStream::write(const void* buffer,
                      const size_t bytes) {
        if (!(m_mode & kModeWrite)) {
            return 0;
        }

        if (feof(m_file.get())) {
            return 0;
        }

       const size_t bytesWritten = ::fwrite(buffer, 1, bytes, m_file.get());
       const size_t currentPosition = getPosition();
       if (currentPosition > m_fileSizeBytes) {
           m_fileSizeBytes = currentPosition;
       }

       return bytesWritten;
    }

    bool
    FileStream::seek(const size_t position) {
        return 0 == ::fseek(m_file.get(), static_cast<long>(position), SEEK_SET);
    }

    size_t
    FileStream::getPosition() const {
        return static_cast<size_t>(::ftell(m_file.get()));
    }

    size_t
    FileStream::getSizeBytes() const
    {
        return m_fileSizeBytes;
    }
}
