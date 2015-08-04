#ifndef __YALVM_CTX_H__
#define __YALVM_CTX_H__

#include "yalvm/yalvm.h"
#include "yalvm/yalvm_register.h"
#include "yalvm/yalvm_bytecode.h"
#include "yalvm/yalvm_binary.h"
#include "yalvm/yalvm_stack.h"

YALVM_MODULE_BGN

typedef struct
{
    const yalvm_bin_header_t*   header;
    yalvm_u8*                   binary;
    const yalvm_u8*             binary_end;
    yalvm_stack_t               stack;
    const yalvm_bytecode_t*     pc;
    yalvm_register_t*           registers;
    yalvm_bin_global32_t*       globals32;
    yalvm_bin_global64_t*       globals64;
    const yalvm_u32*            constants32;
    const yalvm_u64*            constants64;
    const char**                strings;
    const yalvm_func_header_t** functions;
} yalvm_ctx_t;


void
yalvm_ctx_create(yalvm_ctx_t* ctx,
               void* stack,
               const yalvm_size stack_size);

void
yalvm_ctx_destroy(yalvm_ctx_t* ctx);

yalvm_bool
yalvm_ctx_set_binary(yalvm_ctx_t* ctx,
                     void* binary,
                     const yalvm_size binary_size);

yalvm_u32
yalvm_ctx_execute(yalvm_ctx_t* ctx);

yalvm_u32
yalvm_ctx_num_globals32(const yalvm_ctx_t* ctx);

yalvm_u32
yalvm_ctx_num_globals64(const yalvm_ctx_t* ctx);

yalvm_u32
yalvm_ctx_num_functions(const yalvm_ctx_t* ctx);

const yalvm_bin_global32_t*
yalvm_ctx_globals32(const yalvm_ctx_t* ctx);

const yalvm_bin_global64_t*
yalvm_ctx_globals64(const yalvm_ctx_t* ctx);



YALVM_MODULE_END

#endif
