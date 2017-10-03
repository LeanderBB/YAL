#pragma once
#include <yal/yal.h>
#include <stdarg.h>
namespace yal{

    class ByteStream;
    class Log {
    public:

        class MultiLineScope {
        public:
            MultiLineScope(Log& log);
            ~MultiLineScope();
            YAL_NO_COPY_CLASS(MultiLineScope);
        private:
            Log& m_log;
        };

        Log(ByteStream& stream);

        Log(ByteStream& out,
            ByteStream& error);

        void message(const char* format, ...);

        void warning(const char* format, ...);

        void error(const char* format, ...);

        void debug(const char* format, ...);

    private:

        size_t formatWithTag(const char* tag,
                             const size_t tagLen,
                             const char* format,
                             ::va_list args);


    private:
        enum {kInternalBufferSizBytes = 4096};
        char m_buffer[kInternalBufferSizBytes];
        ByteStream& m_out;
        ByteStream& m_error;
        bool m_multiLine;
    };

}
