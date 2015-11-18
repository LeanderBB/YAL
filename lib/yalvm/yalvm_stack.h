#ifndef __YALVM_STACK_H__
#define __YALVM_STACK_H__

#include "yalvm/yalvm.h"
#include "yalvm/yalvm_register.h"
#include "yalvm/yalvm_bytecode.h"
#include "yalvm/yalvm_binary.h"

YALVM_MODULE_BGN

typedef struct
{
    void* stack_frame_base;
    void* stack_frame_top;
    void* stack_start;
    void* stack_end;
} yalvm_stack_t;


void
yalvm_stack_init(yalvm_stack_t* stack,
                 void* stack_memory,
                 const yalvm_size stack_size);

yalvm_bool
yalvm_stack_frame_begin(yalvm_stack_t* stack,
                        const yalvm_u32 register_count);

yalvm_bool
yalvm_stack_frame_end(yalvm_stack_t* stack,
                      const void** pc);

yalvm_bool
yalvm_stack_push(yalvm_stack_t* stack,
                 const yalvm_register_t* reg);

yalvm_bool
yalvm_stack_push_ref(yalvm_stack_t* stack,
                     yalvm_register_t* reg);


yalvm_bool
yalvm_stack_function_begin(yalvm_stack_t* stack,
                           const void *pc,
                           void *return_register,
                           const yalvm_u32 register_count);

void*
yalvm_stack_local_registers(const yalvm_stack_t* stack);

void*
yalvm_stack_return_register(const yalvm_stack_t* stack);


YALVM_MODULE_END

#endif
