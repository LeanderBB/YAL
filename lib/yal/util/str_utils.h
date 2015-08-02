#ifndef __YAL_STR_UTILS_H__
#define __YAL_STR_UTILS_H__

#include "yal/yal.h"
#include <unordered_map>

namespace yal
{

bool
StrToBool(const char* str,
          bool* out);

bool
StrToU32(const char* cstr,
         yal_u32* out);

bool
StrToI32(const char* str,
         yal_i32* out);

bool
StrToU64(const char* str,
         yal_u64* out);

bool
StrToI64(const char* str,
         yal_i64* out);

bool
StrToFloat(const char* str,
           float* out);

bool
StrToDouble(const char* str,
            double* out);

char*
StrCopy(const char* str);


struct StrHashFunc
{
    yal_u32 operator()(const char* str) const;

    bool operator()(const char* str1,
                    const char* str2) const;
};

template <class Value>
using StrHashMap = std::unordered_map<const char*, Value, StrHashFunc, StrHashFunc>;


}

#endif
