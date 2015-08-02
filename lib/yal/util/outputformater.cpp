#include "yal/util/outputformater.h"

#include "yal/util/outputsink.h"
#include <cstdio>
#include <cstdarg>

namespace yal
{

OutputFormater::OutputFormater():
    _buffer(),
    _curLen(0)
{

}

OutputFormater::~OutputFormater()
{

}

bool
OutputFormater::format(const char* format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    _curLen = 0;
    int output_len = vsnprintf(_buffer, kStaticBufferSize, format, argptr);
    if (output_len > 0)
    {
        _curLen = output_len;
    }
    va_end(argptr);
    return output_len > 0;
}

size_t
OutputFormater::write(OutputSink& sink) const
{
    return (_curLen) ? sink.write(_buffer, _curLen) : 0;
}

size_t
OutputFormater::formatAndWrite(OutputSink& sink, const char* format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    _curLen = 0;
    int output_len = vsnprintf(_buffer, kStaticBufferSize, format, argptr);
    if (output_len > 0)
    {
        _curLen = output_len;
        return sink.write(_buffer, _curLen);
    }
    va_end(argptr);
    return 0;
}

}
