#include "yal/util/stringinputsink.h"
#include <cstring>

namespace yal
{

StringInputSink::StringInputSink(
        const char* str,
        const size_t len):
    _string(str),
    _size(len),
    _offset(0)
{

}

StringInputSink::~StringInputSink()
{

}

size_t
StringInputSink::read(void* data,
                       const size_t dataSize)
{
    const size_t read_size =  (dataSize + _offset > _size) ? (_size - _offset) : dataSize;
    memcpy(data, _string + _offset, read_size);
    _offset += read_size;
    return read_size;
}

void
StringInputSink::close()
{
    _offset = _size;
}

bool
StringInputSink::isEof() const
{
    return _offset >= _size;
}

size_t
StringInputSink::tell() const
{
    return _offset;
}

bool
StringInputSink::seekSet(const size_t offset)
{
    if (offset < _size)
    {
        _offset = offset;
        return true;
    }
    return false;
}

}
