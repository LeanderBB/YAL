#include "yalvm/yalvm_error.h"

static const char* yalvm_error_strs[YALVM_ERROR_COUNT] =
{
    "No error",
    "Invalid or corrupted binary source",
    "Invalid execution context",
    "Instruction has not been implemented for the current configuration",
    "Context execution has exceeded the allocated stack size",
    "Context execution has no more stack frames to pop",
    "Unknown instruction encountered",
};

const char*
yalvm_error_str(const yalvm_u32 error)
{
    return (error < YALVM_ERROR_COUNT) ? yalvm_error_strs[error]
                                         : "Uknown error";
}


