#ifndef __YAL_FILESINK_H__
#define __YAL_FILESINK_H__

#include "yal/yal.h"
#include "yal/util/inputsink.h"
#include "yal/util/outputsink.h"

#include <cstdio>


namespace yal
{

class FileOutputSink : public OutputSink
{
public:
    FileOutputSink(FILE* ouput);

    FileOutputSink(const FileOutputSink&) = delete;

    FileOutputSink& operator=(const FileOutputSink&) = delete;

    virtual ~FileOutputSink();

    virtual size_t write(const void* data,
                         const size_t data_size) override;

    virtual void close() override;

private:
    FILE* _pFile;
};

class FileInputSink : public InputSink
{
public:
    FileInputSink(FILE* input);

    FileInputSink(const FileInputSink&) = delete;

    FileInputSink& operator=(const FileInputSink&) = delete;

    virtual ~FileInputSink();

    virtual size_t read(void* data,
                        const size_t dataSize) override;

    virtual void close() override;

    virtual bool isEof() const override;

    virtual size_t tell() const override;

    virtual bool seekSet(const size_t offset) override;

private:
    FILE* _pFile;
};

}

#endif
