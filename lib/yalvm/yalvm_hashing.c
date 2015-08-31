#include "yalvm/yalvm_hashing.h"

yalvm_u32
yalvm_one_at_a_time_hash(const char* str)
{
    yalvm_u32 hash = 0;

    for(;*str; ++str)
    {
        hash += *str;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

