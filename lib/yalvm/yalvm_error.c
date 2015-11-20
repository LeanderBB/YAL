#include "yalvm/yalvm_error.h"

static const char* yalvm_error_strs[YALVM_ERROR_COUNT] =
{
    "No error",
    "Invalid or corrupted binary source",
    "Invalid execution context",
    "Instruction has not been implemented for the current configuration",
    "Context execution has exceeded the allocated stack size",
    "Context execution has no more stack frames to pop",
    "Could not allocated memory",
    "Unknown instruction encountered",
    "Invalid function call instruction encountered",
    "Accessing data outside of the current boundaries",
};

const char*
yalvm_error_str(const yalvm_u32 error)
{
    return (error < YALVM_ERROR_COUNT) ? yalvm_error_strs[error]
                                         : "Uknown error";
}


