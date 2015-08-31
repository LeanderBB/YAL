#ifndef __YAL_STRINGINPUTSINK_H__
#define __YAL_STRINGINPUTSINK_H__

#include "yal/yal.h"
#include "yal/util/inputsink.h"

namespace yal
{

class StringInputSink : public InputSink
{
public:
    StringInputSink(const char* str,
                    const size_t len);

    StringInputSink(const StringInputSink&) = delete;

    StringInputSink& operator=(const StringInputSink&) = delete;

    virtual ~StringInputSink();

    virtual size_t read(void* data,
                        const size_t dataSize) override;

    virtual void close() override;

    virtual bool isEof() const override;

    virtual size_t tell() const override;

    virtual bool seekSet(const size_t offset) override;

private:
    const char* _string;
    const size_t _size;
    size_t _offset;

};

}
#endif
