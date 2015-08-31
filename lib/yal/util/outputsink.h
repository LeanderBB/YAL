#ifndef __YAL_OUTPUTSINK__
#define __YAL_OUTPUTSINK__

namespace yal
{

class OutputSink
{
public:

    OutputSink(){}

    virtual ~OutputSink(){}

    virtual size_t write(const void* data,
                         const size_t data_size) =0;

    virtual void close() = 0;

};


class NullOutputSink : public OutputSink
{
public:
    NullOutputSink():
    OutputSink()
    {}

    virtual ~NullOutputSink(){}

    virtual size_t write(const void*,
                         const size_t data_size) override
    {
        return data_size;
    }

    virtual void close() override {}
};

}
#endif
