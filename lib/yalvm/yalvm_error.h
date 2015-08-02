#ifndef __YALVM_ERROR_H__
#define __YALVM_ERROR_H__

#include "yalvm/yalvm.h"

YALVM_MODULE_BGN

enum yalvm_error
{
    YALVM_ERROR_NONE,
    YALVM_ERROR_INVALID_BINARY,
    YALVM_ERROR_INVALID_CTX,
    YALVM_ERROR_INSTRUCTION_NOT_IMPLEMENTED,
    YALVM_ERROR_STACK_OVERFLOW,
    YALVM_ERROR_STACK_UNDERFLOW,
    YALVM_ERROR_UNKNOW_INSTRUCTION,
    /* placeholder do not use */
    YALVM_ERROR_COUNT
};


const char*
yalvm_error_str(const yalvm_u32 error);

YALVM_MODULE_END

#endif
