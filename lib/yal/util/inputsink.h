#ifndef __YAL_INPUTSINK_H__
#define __YAL_INPUTSINK_H__

namespace yal
{

class InputSink
{
public:

    InputSink(){}

    virtual ~InputSink(){}

    virtual size_t read(void* data,
                        const size_t data_size) = 0;

    virtual void close() = 0;

    virtual bool isEof() const = 0;

    virtual size_t tell() const = 0;

    virtual bool seekSet(const size_t offset) = 0;

};


}

#endif

