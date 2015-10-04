#ifndef __YALVM_CTX_H__
#define __YALVM_CTX_H__

#include "yalvm/yalvm.h"
#include "yalvm/yalvm_register.h"
#include "yalvm/yalvm_bytecode.h"
#include "yalvm/yalvm_binary.h"
#include "yalvm/yalvm_stack.h"

YALVM_MODULE_BGN

typedef struct yalvm_ctx
{
    const yalvm_binary_t*         binary;
    const yalvm_bytecode_t*     pc;
    yalvm_register_t*           registers;
    yalvm_u32*                  globals32;
    yalvm_u64*                  globals64;
    yalvm_size*                 globalsPtr;
    yalvm_stack_t               stack;
    yalvm_size                  globals_size;
    char                        print_buffer[512];
} yalvm_ctx_t;

typedef struct
{
    const yalvm_func_header_t*  func_header;
    yalvm_ctx_t*                ctx;
    yalvm_u32                   pushed_arg_count;
    yalvm_register_t            return_register;
} yalvm_func_hdl_t;



yalvm_bool
yalvm_ctx_create(yalvm_ctx_t* ctx,
                 const yalvm_binary_t* binary,
                 void* stack,
                 const yalvm_size stack_size);

void
yalvm_ctx_destroy(yalvm_ctx_t* ctx);


yalvm_u32
yalvm_ctx_num_globals32(const yalvm_ctx_t* ctx);

yalvm_u32
yalvm_ctx_num_globals64(const yalvm_ctx_t* ctx);

yalvm_u32
yalvm_ctx_num_globalsPtr(const yalvm_ctx_t* ctx);

yalvm_u32
yalvm_ctx_num_functions(const yalvm_ctx_t* ctx);

const yalvm_u32*
yalvm_ctx_globals32(const yalvm_ctx_t* ctx);

const yalvm_u64*
yalvm_ctx_globals64(const yalvm_ctx_t* ctx);


yalvm_bool
yalvm_ctx_acquire_function(yalvm_ctx_t* ctx,
                           yalvm_func_hdl_t* func_hdl,
                           const char* function_name);

yalvm_bool
yalvm_func_hdl_push_arg(yalvm_func_hdl_t* func_hdl,
                        const yalvm_register_t* arg);

yalvm_u32
yalvm_func_hdl_execute(yalvm_func_hdl_t* func_hdl);

yalvm_bool
yalvm_ctx_release_function(yalvm_func_hdl_t* func_hdl);

YALVM_MODULE_END

#endif
