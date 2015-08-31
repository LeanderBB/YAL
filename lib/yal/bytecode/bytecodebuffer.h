#ifndef __YAL_BYTECODEBUFFER_H__
#define __YAL_BYTECODEBUFFER_H__

#include <yalvm/yalvm_bytecode.h>
#include <vector>

namespace yal
{

class ByteCodeBuffer
{

public:
    ByteCodeBuffer(const size_t intialCapacity = 256);

    ~ByteCodeBuffer();

    size_t size() const
    {
        return _btBuffer.size();
    }

    size_t capacity() const
    {
        return _btBuffer.capacity();
    }

    void expand(size_t additionalCapacity);

    void replace(const size_t idx,
                 const yalvm_bytecode_t code);

    const yalvm_bytecode_t* buffer() const
    {
        return _btBuffer.data();
    }

    size_t append(const yalvm_bytecode_t code);

    void reset();

protected:
    typedef std::vector<yalvm_bytecode_t> ByteCodeVec_t;
    ByteCodeVec_t _btBuffer;
};

}

#endif
