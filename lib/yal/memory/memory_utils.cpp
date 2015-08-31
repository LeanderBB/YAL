#include "yal/memory/memory_utils.h"

void*
yal_malloc(size_t size)
{
    return malloc(size);
}

void
yal_free(void* ptr)
{
    free(ptr);
}

void*
yal_calloc(size_t s,
           size_t size)
{
    return calloc(s, size);
}

void*
yal_realloc(void* ptr,
            size_t size)
{
    return realloc(ptr, size);
}
