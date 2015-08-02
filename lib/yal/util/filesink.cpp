#include "yal/util/filesink.h"

namespace yal
{

FileOutputSink::FileOutputSink(FILE* ouput):
    _pFile(ouput)
{

}

FileOutputSink::~FileOutputSink()
{
    close();
}

size_t
FileOutputSink::write(const void* data,
                     const size_t data_size)
{
    return fwrite(data, 1, data_size, _pFile);
}

void
FileOutputSink::close()
{
    if(_pFile)
    {
        if (_pFile != stdout)
        {
        fclose(_pFile);
        }
        _pFile = nullptr;
    }
}

FileInputSink::FileInputSink(FILE* input):
    _pFile(input)
{

}

FileInputSink::~FileInputSink()
{
    close();
}

size_t
FileInputSink::read(void* data,
                    const size_t dataSize)
{
    YAL_ASSERT(_pFile);
    return fread(data, 1, dataSize, _pFile);
}

void
FileInputSink::close()
{
    if(_pFile)
    {
        if (_pFile != stdin)
        {
        fclose(_pFile);
        }
        _pFile = nullptr;
    }
}

bool
FileInputSink::isEof() const
{
    return (_pFile) ? feof(_pFile) : true;
}

size_t
FileInputSink::tell() const
{
    return ftell(_pFile);
}

bool
FileInputSink::seekSet(const size_t offset)
{
    return fseek(_pFile, offset, SEEK_SET) == 0;
}

}

