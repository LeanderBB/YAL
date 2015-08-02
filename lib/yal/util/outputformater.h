#ifndef __YAL_OUTPUTFORMATER_H__
#define __YAL_OUTPUTFORMATER_H__

#include "yal/yal.h"

namespace yal
{
class OutputSink;
class OutputFormater
{
    enum
    {
        kStaticBufferSize = 1024
    };
public:
    OutputFormater();

    ~OutputFormater();

    bool format(const char* format, ...);

    size_t write(OutputSink& sink) const;

    size_t formatAndWrite(OutputSink& sink, const char* format, ...);

    const char* str() const
    {
        return _buffer;
    }

    int strLen() const
    {
        return _curLen;
    }
private:
    char _buffer[kStaticBufferSize];
    int _curLen;
};

}
#endif
