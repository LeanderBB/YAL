#ifndef __YALVM_STRING_H__
#define __YALVM_STRING_H__

#include "yalvm/yalvm_array.h"

YALVM_MODULE_BGN

typedef struct yalvm_string
{
    yalvm_array_base_t array;
} yalvm_string_t;


yalvm_object_t*
yalvm_string_create_empty();

yalvm_object_t*
yalvm_string_create_with_constant(const char* text);

void
yalvm_string_destroy(yalvm_object_t *obj);

const char*
yalvm_string_str(const yalvm_object_t* obj);


YALVM_MODULE_END

#endif
