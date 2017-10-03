#include "yal/io/filestream.h"
#include <cstdio>
#include <string>
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
        m_file(nullptr, fileDtor),
        m_fileSizeBytes(0),
        m_offset(0),
        m_flags(0){

    }

    bool
    FileStream::open(const StdStream stream) {

        if (stream == StdStream::In) {
            m_file = FileType(::stdin, stdStreamDtor);
            m_flags = kModeRead;
            m_fileSizeBytes = 0;
            m_offset = 0;
        } else if (stream == StdStream::Out) {
            m_file = FileType(::stdout, stdStreamDtor);
            m_flags = kModeWrite;
            m_fileSizeBytes = 0;
            m_offset = 0;
        } else if (stream == StdStream::Error){
            m_file = FileType(::stderr, stdStreamDtor);
            m_flags = kModeWrite;
            m_fileSizeBytes = 0;
            m_offset = 0;
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
             m_flags = mode;
             m_flags |= kFlagSeekable;
             m_offset = 0;
             return true;
        }
        return false;
    }

    size_t
    FileStream::read(void* buffer,
                     const size_t bytes) {
        if (!(m_flags & kModeRead)) {
            return 0;
        }

        if (feof(m_file.get())) {
            return 0;
        }

        const size_t bytesRead = ::fread(buffer, 1, bytes, m_file.get());
        m_offset += bytesRead;
        return bytesRead;
    }

    size_t
    FileStream::write(const void* buffer,
                      const size_t bytes) {
        if (!(m_flags & kModeWrite)) {
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

       m_offset += bytesWritten;

       return bytesWritten;
    }

    bool
    FileStream::seek(const size_t position) {
        return 0 == ::fseek(m_file.get(), static_cast<long>(position), SEEK_SET);
    }

    size_t
    FileStream::getPosition() const {
        return m_offset;
    }


    enum {
        kSkipLineBuffer = 1024
    };

    size_t
    FileStream::getSizeBytes() const {
        return m_fileSizeBytes;
    }

    void
    FileStream::skipLine() {
        char buffer[kSkipLineBuffer];
        while(true){
            const char* result = fgets(buffer, kSkipLineBuffer, m_file.get());
            if (result == nullptr) {
                return;
            }else {
                const size_t lineSize = strlen(buffer);
                if (lineSize < kSkipLineBuffer - 1) {
                    // found new line
                    return;
                } else if (lineSize == kSkipLineBuffer -1
                           && buffer[kSkipLineBuffer-2] == '\n') {
                    // found new line with size the same as the buffer
                    return;
                }
            }
        }
    }

    std::string
    FileStream::readLine() {
        std::string strResult;
        char buffer[kSkipLineBuffer];
        while(true){
            const char* result = fgets(buffer, kSkipLineBuffer, m_file.get());
            if (result == nullptr) {
                break;
            }else {
                const size_t lineSize = strlen(buffer);
                if (lineSize < kSkipLineBuffer ) {
                    // found new line
                    strResult.append(buffer, lineSize - 1);
                    break;
                } else if (lineSize == kSkipLineBuffer -1
                           && buffer[kSkipLineBuffer-2] == '\n') {
                    // found new line with size the same as the buffer
                    strResult.append(buffer, lineSize - 1);
                    break;
                } else {
                    strResult.append(buffer, lineSize);
                }
            }
        }
        return strResult;
    }

    bool
    FileStream::isSeekable() const {
        return m_flags & kFlagSeekable;
    }
}
