#ifndef __YAL_REGISTALLOCATOR_H__
#define __YAL_REGISTALLOCATOR_H__

#include "yal/util/str_utils.h"

namespace yal
{

class RegisterAllocator
{
public:

    static const yal_i32 UnusedRegisterValue;

    RegisterAllocator();

    ~RegisterAllocator();

    /// \return Valid regisiter id if it is the first allocation of the variable.
    /// RegisterUnused if there are no more registers available.
    yal_i32 allocate(const char* varName,
                     const yal_u32 scopeLevel);

    void deallocate(const char* varName,
                    const yal_u32 scopeLevel);

    bool isAllocated(const char* varName,
                     const yal_u32 scopeLevel) const;

    /// \return RegisterUnused if the variable is not registered and we were
    /// not able to allocate a new register for it
    yal_i32 registerForVariable(const char* varName,
                                const yal_u32 scopeLevel) const;

    void reset();

    /// Returns the maximum number of register by the allocator during its
    /// lifetime.
    yal_i32 maxRegisterCount() const;

    /// \return RegisterUnused if there are no more registers available.
    yal_i32 allocateTemporary();

    void deallocateTemporary(const yal_i32 regIdx);

protected:

    struct Key
    {
        const char* name;
        yal_u32 level;

        bool operator == (const Key& other) const;
    };

    struct HashFunc
    {
        yal_u32 operator()(const RegisterAllocator::Key& key) const;

        bool operator()(const RegisterAllocator::Key& key1,
                        const RegisterAllocator::Key& key2) const
        {
            return key1 == key2;
        }
    };


protected:
    yal_i32 _maxRegisterCount;
    yal_i32 _nextFreeRegister;
    yal_i32 _registerPool[kMaxRegisters];
    std::unordered_map<RegisterAllocator::Key, yal_i32, RegisterAllocator::HashFunc> _registerMap;
};

}
#endif
