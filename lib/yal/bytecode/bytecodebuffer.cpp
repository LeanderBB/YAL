#include "yal/bytecode/bytecodebuffer.h"

namespace yal
{

ByteCodeBuffer::ByteCodeBuffer(const size_t intialCapacity)
{
    _btBuffer.reserve(intialCapacity);
}

ByteCodeBuffer::~ByteCodeBuffer()
{

}

void
ByteCodeBuffer::expand(size_t additionalCapacity)
{
    _btBuffer.reserve(capacity() + additionalCapacity);
}

size_t
ByteCodeBuffer::append(const yalvm_bytecode_t code)
{
    _btBuffer.push_back(code);
    return size() - 1;
}

void
ByteCodeBuffer::replace(const size_t idx,
                        const yalvm_bytecode_t code)
{
    if (idx < size())
    {
        _btBuffer[idx] = code;
    }
}

void
ByteCodeBuffer::reset()
{
    _btBuffer.clear();
}

}
