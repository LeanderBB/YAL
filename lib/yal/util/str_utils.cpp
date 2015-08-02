#include "yal/util/str_utils.h"
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <ctype.h>
#include "yal/memory/memory_utils.h"
#include "yalvm/yalvm_hashing.h"

namespace yal
{

bool
StrToBool(const char* str,
             bool* out)
{
    bool result = false;
    if (strcmp("true", str) == 0)
    {
        result = true;
        *out = true;
    }
    else if (strcmp("false", str) == 0)
    {
        result = true;
        *out = false;
    }
    return result;
}

bool
StrToU32(const char* cstr,
            yal_u32* out)
{
    char* cout= NULL;
    yal_u32 result = (yal_u32)(strtoumax(cstr, &cout, 10));
    if (*cout == '\0')
    {
        *out = result;
        return true;
    }
    return false;
}

bool
StrToI32(const char* str,
            yal_i32* out)
{
    char* cout = NULL;
    yal_i32 result = (yal_i32)(strtoimax(str, &cout, 10));
    if (*cout == '\0')
    {
        *out = result;
        return true;
    }
    return false;
}

bool
StrToU64(const char* str,
            yal_u64* out)
{
    char* cout=NULL;
    yal_u64 result = strtoull(str, &cout, 10);
    if (*cout == '\0')
    {
        *out = result;
        return true;
    }
    return false;
}

bool
StrToI64(const char* str,
            yal_i64* out)
{
    char* cout=NULL;
    yal_i64 result = strtoll(str, &cout, 10);
    if (*cout == '\0')
    {
        *out = result;
        return true;
    }
    return false;
}

bool
StrToFloat(const char* str,
              float* out)
{
    char* cout=NULL;
    float result = strtof(str, &cout);
    if (*cout == '\0')
    {
        *out = result;
        return true;
    }
    return false;
}

bool
StrToDouble(const char* str,
               double* out)
{
    char* cout=NULL;
    double result = strtod(str, &cout);
    if (*cout == '\0')
    {
        *out = result;
        return true;
    }
    return false;
}

char*
StrCopy(const char *str)
{
    if (!str)
    {
        return NULL;
    }

    size_t len = strlen(str);

    if (!len)
    {
        return NULL;
    }

    char* str_copy = (char*) yal_malloc(len + 1);
    memcpy(str_copy, str, len + 1);
    return str_copy;
}

yal_u32
StrHashFunc::operator()(const char* str) const
{
    return yalvm_one_at_a_time_hash(str);
}


bool StrHashFunc::operator()(const char* str1,
                            const char* str2) const
{
   return strcmp(str1, str2) == 0;
}

}
