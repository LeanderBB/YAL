#ifndef __YALVM_EXTERNAL_H__
#define __YALVM_EXTERNAL_H__

YALVM_MODULE_BGN

struct yalvm_ctx;

void*
yalvm_malloc(yalvm_size size);

void
yalvm_free(void* ptr);

void
yalvm_memset(void* ptr,
             int val,
             yalvm_size size);

void
yalvm_memcpy(void* dst,
             const void* src,
             yalvm_size size);

void*
yalvm_realloc(void* ptr,
              yalvm_size size);

void*
yalvm_memmove(void *dest,
              const void *src,
              yalvm_size n);

void
yalvm_print(struct yalvm_ctx* ctx,
            const char* string);

int
yalvm_snprintf(char *str,
               yalvm_size size,
               const char *format,
               ...);

YALVM_MODULE_END

#endif

