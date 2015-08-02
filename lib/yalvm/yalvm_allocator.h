#ifndef __YALVM_ALLOCATOR_H__
#define __YALVM_ALLOCATOR_H__

YALVM_MODULE_BGN

#if defined(YALVM_USE_CSTDLIB)

#define yalvm_malloc(size) malloc(size)

#define yalvm_free(ptr) free(ptr)

#define yalvm_memset(ptr, val, size) memset(ptr, val, size)

#define yalvm_memcpy(dst, src, size) memcpy(dst, src, size)

#else

void* yalvm_malloc(yalvm_size size);

void yalvm_free(void* ptr);

void yalvm_memset(void* ptr, int val, yalvm_size size);

void yalvm_memcpy(void* dst, const void* src, yalvm_size size);

#endif

YALVM_MODULE_END

#endif

