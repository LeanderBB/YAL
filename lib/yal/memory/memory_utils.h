#ifndef __YAL_MEMORY_UTILS_H__
#define __YAL_MEMORY_UTILS_H__

#include <yal/yal.h>

#if defined(__cplusplus)
extern "C"
{
#endif
void*
yal_malloc(size_t size);

void
yal_free(void* ptr);

void
*yal_calloc(size_t s,
            size_t size);

void*
yal_realloc(void* ptr,
            size_t size);

#if defined(__cplusplus)
}
#endif
#endif
