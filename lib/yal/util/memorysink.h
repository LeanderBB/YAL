#ifndef __YAL_MEMORYSINK_H__
#define __YAL_MEMORYSINK_H__

#include "yal/yal.h"
#include "yal/util/outputsink.h"

namespace yal
{

class MemoryOuputSink : public OutputSink
{
public:
    MemoryOuputSink(const size_t bufferSize);

    MemoryOuputSink(const MemoryOuputSink&) = delete;

    MemoryOuputSink& operator=(const MemoryOuputSink&) = delete;

    virtual ~MemoryOuputSink();

    virtual size_t write(const void* data,
                         const size_t data_size) override;

    virtual void close() override;

    const void* buffer() const
    {
        return _buffer;
    }

    size_t bufferSize() const
    {
        return _bufferSize;
    }

    size_t dataSize() const
    {
        return _offset;
    }

private:
    char* _buffer;
    const size_t _bufferSize;
    size_t _offset;
};

}
#endif
