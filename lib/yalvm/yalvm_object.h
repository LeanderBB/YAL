#ifndef __YALVM_OBJECT_H__
#define __YALVM_OBJECT_H__

#include "yalvm/yalvm.h"

YALVM_MODULE_BGN

struct yalvm_ctx;

typedef struct yalvm_object
{
  yalvm_i32 nref;
  void*     ptr;
} yalvm_object_t;


yalvm_object_t*
yalvm_object_alloc(const yalvm_size size);

void
yalvm_object_acquire(yalvm_object_t* obj);

yalvm_bool
yalvm_object_release(yalvm_object_t* obj);

void
yalvm_object_dealloc(yalvm_object_t* obj);

YALVM_MODULE_END

#endif
