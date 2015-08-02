#include "yalvm/yalvm_stack.h"

typedef struct
{
    void*       frame_base;
    const void* frame_pc;
    void*       return_register;
} yalvm_stack_frame_session_t;

void
yalvm_stack_init(yalvm_stack_t* stack,
                 void* stack_memory,
                 const yalvm_size stack_size)
{
    stack->stack_start = stack_memory;
    stack->stack_end = YALVM_PTR_ADD(stack->stack_start, stack_size);
    stack->stack_frame_base = NULL;
    stack->stack_frame_top = NULL;
}

yalvm_bool
yalvm_stack_frame_begin(yalvm_stack_t* stack,
                        const yalvm_u32 register_count)
{
    const yalvm_size register_memory = register_count * sizeof(yalvm_register_t);
    const size_t offset = register_memory + sizeof(yalvm_stack_frame_session_t);
    void* next_frame = NULL;

    /* no stack frame has been allocated yet */
    if (!stack->stack_frame_base)
    {
       next_frame = YALVM_PTR_ADD(stack->stack_start, offset);
    }
    /* previous frame exists */
    else
    {
        /* calculate in space to save current stack base */
        next_frame = YALVM_PTR_ADD(stack->stack_frame_top, offset);
    }

    /* check whether we exceed our stack size */
    if (next_frame >= stack->stack_end)
    {
        return yalvm_false;
    }
    void* next_stack_base = NULL;
    /* save current stack base */
    if (stack->stack_frame_base)
    {
        yalvm_stack_frame_session_t* stack_top_ptr = (yalvm_stack_frame_session_t*) stack->stack_frame_top;
        stack_top_ptr->frame_base = stack->stack_frame_base;
        stack_top_ptr->frame_pc = NULL;
        next_stack_base = stack_top_ptr + 1;
    }
    else
    {
        yalvm_stack_frame_session_t* stack_top_ptr = (yalvm_stack_frame_session_t*) stack->stack_start;
        stack_top_ptr->frame_base = NULL;
        stack_top_ptr->frame_pc = NULL;
        next_stack_base = stack_top_ptr + 1;
    }

    /* move stack forward */
    stack->stack_frame_base = next_stack_base;
    stack->stack_frame_top = next_stack_base;

    if (register_memory)
    {
        yalvm_memset(next_stack_base, 0, register_memory);
    }

    return yalvm_true;
}

yalvm_bool
yalvm_stack_frame_end(yalvm_stack_t* stack,
                      const void **pc)
{
    /* no frame, can't end */
    if (!stack->stack_frame_base)
    {
        return yalvm_false;
    }


    yalvm_stack_frame_session_t* prev_frame_record = (yalvm_stack_frame_session_t*) stack->stack_frame_base;
    --prev_frame_record;

    /* first frame, empty stack afterwards */
    if ((void*)prev_frame_record < stack->stack_start)
    {
        stack->stack_frame_base = NULL;
        stack->stack_frame_top = NULL;
        *pc = NULL;
    }
    else
    {
        /* check if previous frame is valid */
        if (prev_frame_record->frame_base < stack->stack_start)
        {
            return yalvm_false;
        }

        /* setup previous state */
        stack->stack_frame_base = prev_frame_record->frame_base;
        stack->stack_frame_top = prev_frame_record;
        *pc = prev_frame_record->frame_pc;
    }

    return yalvm_true;
}

yalvm_bool
yalvm_stack_push(yalvm_stack_t* stack,
                 const yalvm_register_t* reg)
{
    yalvm_u64* stack_top_ptr = (yalvm_u64*) stack->stack_frame_top;
    if (stack_top_ptr)
    {
        *stack_top_ptr = reg->reg64.value;
        stack->stack_frame_top = YALVM_PTR_ADD(stack->stack_frame_top,
                                               sizeof(yalvm_register_t));
        return yalvm_true;
    }
    return yalvm_false;
}

yalvm_bool
yalvm_stack_function_begin(yalvm_stack_t* stack,
                           const void* pc,
                           void* return_register,
                           const yalvm_u32 register_count)
{
    if (stack->stack_frame_top)
    {
        /* save pc */
        yalvm_stack_frame_session_t* prev_session = (yalvm_stack_frame_session_t*) stack->stack_frame_base;
        --prev_session;

        if (!((void*)prev_session < stack->stack_start))
        {
            prev_session->frame_pc = pc;
            prev_session->return_register = return_register;
        }

        const size_t offset = register_count * sizeof(yalvm_register_t);

        void* new_stack_top = YALVM_PTR_ADD(stack->stack_frame_base, offset);

        if (new_stack_top < stack->stack_end)
        {
            stack->stack_frame_top = new_stack_top;
            return yalvm_true;
        }
    }
    return yalvm_false;
}

void*
yalvm_stack_local_registers(const yalvm_stack_t* stack)
{
    return (stack->stack_frame_base) ? stack->stack_frame_base : NULL;
}

void*
yalvm_stack_return_register(const yalvm_stack_t* stack)
{
    /* no frame, can't return anything */
    if (!stack->stack_frame_base)
    {
        return NULL;
    }

    void* return_val = NULL;
    yalvm_stack_frame_session_t* prev_frame_record = (yalvm_stack_frame_session_t*) stack->stack_frame_base;
    --prev_frame_record;

    /* first frame, empty stack afterwards */
    if ((void*)prev_frame_record >= stack->stack_start)
    {
        return_val = prev_frame_record->return_register;
    }

    return return_val;
}
