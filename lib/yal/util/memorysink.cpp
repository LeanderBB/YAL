#include "yal/util/memorysink.h"
#include <cstring>
namespace yal
{

MemoryOutputSink::MemoryOutputSink(const size_t bufferSize):
    _buffer(new char[bufferSize]),
    _bufferSize(bufferSize),
    _offset(0)
{

}

MemoryOutputSink::~MemoryOutputSink()
{
    delete[] _buffer;
}


size_t
MemoryOutputSink::write(const void *data,
                       const size_t data_size)
{
    size_t new_offset = _offset + data_size;
    size_t bytes_written = 0;
    if (new_offset < _bufferSize && new_offset > _offset)
    {
        memcpy(_buffer + _offset, data, data_size);
        _offset = new_offset;
        bytes_written = data_size;
    }
    return bytes_written;
}


void
MemoryOutputSink::close()
{
    _offset = _bufferSize;
}


}
