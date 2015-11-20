#ifndef __YALVM_BUILTIN_H__
#define __YALVM_BUILTIN_H__

#include "yalvm/yalvm.h"

YALVM_MODULE_BGN

union yalvm_register;

void
yal_native_array_any_length_uint32(const yalvm_u32 reg_count,
                                   union yalvm_register* registers,
                                   union yalvm_register* return_register);

YALVM_MODULE_END

#endif

