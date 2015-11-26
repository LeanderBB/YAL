#include "yal/bytecode/registerallocator.h"
#include <string.h>
#include <algorithm>
namespace yal
{

const yal_i32 RegisterAllocator::UnusedRegisterValue = -1;

RegisterAllocator::RegisterAllocator()
{
    reset();
}

RegisterAllocator::~RegisterAllocator()
{

}

yal_i32
RegisterAllocator::allocate(const char* varName,
                            const yal_u32 scopeLevel)
{
    if (_nextFreeRegister == UnusedRegisterValue)
    {
        return UnusedRegisterValue;
    }

    Key key;
    key.level = scopeLevel;
    key.name = varName;

    auto it = _registerMap.find(key);
    if (it == _registerMap.end())
    {
        const yal_i32 register_idx = _nextFreeRegister;
        _maxRegisterCount = std::max(register_idx, _maxRegisterCount);
        _nextFreeRegister =  _registerPool[register_idx];
        _registerPool[register_idx] = UnusedRegisterValue;
        _registerMap.insert(std::make_pair(key, register_idx));
        return register_idx;
    }
    return it->second;
}

void
RegisterAllocator::deallocate(const char* varName,
                              const yal_u32 scopeLevel)
{
    Key key;
    key.level = scopeLevel;
    key.name = varName;
    auto it = _registerMap.find(key);
    if (it != _registerMap.end())
    {
        // store next free register
        const yal_i32 register_idx = it->second;
        _registerPool[register_idx] = _nextFreeRegister;
        _nextFreeRegister = register_idx;
        _registerMap.erase(it);
    }
}

bool
RegisterAllocator::isAllocated(const char* varName,
                               const yal_u32 scopeLevel) const
{
    Key key;
    key.level = scopeLevel;
    key.name = varName;
    return _registerMap.find(key) != _registerMap.end();
}

yal_i32
RegisterAllocator::registerForVariable(const char* varName,
                                       const yal_u32 scopeLevel) const
{
    Key key;
    key.level = scopeLevel;
    key.name = varName;
    auto it = _registerMap.find(key);
    if (it != _registerMap.end())
    {
        return it->second;
    }
    return UnusedRegisterValue;
}

yal_i32
RegisterAllocator::allocateTemporary()
{
    if (_nextFreeRegister == UnusedRegisterValue)
    {
        return UnusedRegisterValue;
    }

    const yal_i32 register_idx = _nextFreeRegister;
    _maxRegisterCount = std::max(register_idx, _maxRegisterCount);
    _nextFreeRegister =  _registerPool[register_idx];

    return register_idx;
}

void
RegisterAllocator::deallocateTemporary(const yal_i32 regIdx)
{
    if (regIdx != UnusedRegisterValue)
    {
        _registerPool[regIdx] = _nextFreeRegister;
        _nextFreeRegister = regIdx;
    }
}

void
RegisterAllocator::reset()
{
    unsigned i = 0;
    for ( ;i < kMaxRegisters - 1; ++i)
    {
        _registerPool[i] = i + 1;
    }
    _registerPool[i] = UnusedRegisterValue;
    _maxRegisterCount = 0;
    _nextFreeRegister = 0;
    _registerMap.clear();
}

yal_i32
RegisterAllocator::maxRegisterCount() const
{
    return _maxRegisterCount + 1;
}

yal_u32
RegisterAllocator::HashFunc::operator()(const RegisterAllocator::Key& key) const
{

    yal_u32 hash = 0;
    const char* str = key.name;
    for(;*str; ++str)
    {
        hash += *str;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    const char* ptr = reinterpret_cast<const char*>(&key.level);
    for (unsigned i = 0; i < sizeof(key.level); ++i, ++ptr)
    {
        hash += *ptr;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

bool
RegisterAllocator::Key::operator == (const RegisterAllocator::Key& other) const
{
    if (this->level == other.level)
    {
        return strcmp(this->name, other.name) == 0;
    }
    return false;
}


}
