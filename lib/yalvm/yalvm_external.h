#ifndef __YALVM_EXTERNAL_H__
#define __YALVM_EXTERNAL_H__

YALVM_MODULE_BGN

void* yalvm_malloc(yalvm_size size);

void yalvm_free(void* ptr);

void yalvm_memset(void* ptr, int val, yalvm_size size);

void yalvm_memcpy(void* dst, const void* src, yalvm_size size);

YALVM_MODULE_END

#endif

