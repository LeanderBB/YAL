#include "yalvm/yalvm_ctx.h"
#include "yalvm/yalvm_error.h"

void
yalvm_ctx_create(yalvm_ctx_t* ctx,
                 void* stack,
                 const yalvm_size stack_size)
{
    yalvm_stack_init(&ctx->stack, stack, stack_size);
    ctx->registers = NULL;
    ctx->header = NULL;
    ctx->constants32 = NULL;
    ctx->constants64= NULL;
    ctx->globals32 = NULL;
    ctx->globals64 = NULL;
    ctx->functions = NULL;
    ctx->strings = NULL;
    ctx->pc = NULL;
}

void
yalvm_ctx_destroy(yalvm_ctx_t *ctx)
{
    if (ctx->functions)
    {
        yalvm_free(ctx->functions);
        ctx->functions = NULL;
    }

    if (ctx->strings)
    {
        yalvm_free(ctx->strings);
        ctx->strings = NULL;
    }
}

yalvm_bool
yalvm_ctx_set_binary(yalvm_ctx_t* ctx,
                     void* binary,
                     const yalvm_size binary_size)
{
    yalvm_u8* input = (yalvm_u8*) binary;
    const yalvm_u8* input_end = input + binary_size;

    /* validate input */
    const yalvm_bin_header_t* bin_header = (yalvm_bin_header_t*) input;

    if (!yalvm_bin_header_valid_magic(bin_header))
    {
        return yalvm_false;
    }

    /* setup input ptrs */
    ctx->header = bin_header;
    ctx->binary = binary;
    ctx->binary_end = input_end;

    ctx->globals32 = (yalvm_bin_global32_t*) (input
            + yalvm_bin_header_offset_global32(bin_header, 0));

    ctx->globals64 = (yalvm_bin_global64_t*) (input
            + yalvm_bin_header_offset_global64(bin_header, 0));

    ctx->constants32 = (yalvm_u32*) (input
            + yalvm_bin_header_offset_constant32(bin_header, 0));

    ctx->constants64 = (yalvm_u64*) (input
            + yalvm_bin_header_offset_constant32(bin_header, 0));


    const void* string_offset = (input + yalvm_bin_header_offset_strings(bin_header));
    /* cache string locations */
    if (bin_header->n_strings)
    {
        ctx->strings = (const char**)yalvm_malloc(sizeof(char*) * bin_header->n_strings);
        size_t offset = 0;
        for (yalvm_u16 i = 0; i < bin_header->n_strings; ++i)
        {
            const void* cur_ptr = YALVM_PTR_ADD(string_offset, offset);
            const yalvm_u32* string_size = (const yalvm_u32*) cur_ptr;
            offset += sizeof(yalvm_u32);

            ctx->strings[i] = (const char*) YALVM_PTR_ADD(string_offset, offset);
            offset += (*string_size) + 1;
        }
    }

    /* process functions */
    ctx->functions = (const yalvm_func_header_t**)
            yalvm_malloc(sizeof(yalvm_func_header_t) * bin_header->n_functions + 1);

    input += yalvm_bin_header_offset_functions(bin_header);

    for(yalvm_u32 i = 0; i < bin_header->n_functions; ++i)
    {
        yalvm_func_header_t* function_header = (yalvm_func_header_t*)input;

        if (input >= input_end || !yalvm_func_header_valid_magic(function_header))
        {
            yalvm_ctx_destroy(ctx);
            return yalvm_false;
        }

        ctx->functions[i] = function_header;

        input += sizeof(yalvm_func_header_t)
                + (sizeof(yalvm_bytecode_t) * function_header->code_size);
    }

    /* set pc to global function if there is one */
    if (input < input_end)
    {
        yalvm_func_header_t* global_func_header = (yalvm_func_header_t*)input;

        if (input >= input_end
                || !yalvm_func_header_valid_magic(global_func_header)
                || yalvm_func_global_hash() != global_func_header->hash)
        {
            yalvm_ctx_destroy(ctx);
            return yalvm_false;
        }

        ctx->pc = (yalvm_bytecode_t*) (global_func_header + 1);
        if (!yalvm_stack_frame_begin(&ctx->stack,
                                     global_func_header->n_registers)
                || !yalvm_stack_function_begin(&ctx->stack,
                                               NULL,
                                               NULL,
                                               global_func_header->n_registers))
        {
            yalvm_ctx_destroy(ctx);
            return yalvm_false;
        }
        ctx->registers = yalvm_stack_local_registers(&ctx->stack);
    }

    return yalvm_true;
}



#define YALVM_UNACK_REGISTERS(type) \
    yalvm_u8 dst, src1, src2; \
    yalvm_bytecode_unpack_registers(code, &dst, &src1, &src2); \
    yalvm_ ## type  * reg_dst = yalvm_register_to_ ## type (&ctx->registers[dst]); \
    yalvm_ ## type  * reg_src1 = yalvm_register_to_ ## type (&ctx->registers[src1]); \
    yalvm_ ## type  * reg_src2 = yalvm_register_to_ ## type (&ctx->registers[src2])


#define YALVM_UNACK_REGISTER(type) \
    yalvm_u8 dst; \
    yalvm_bytecode_unpack_register(code, &dst); \
    yalvm_ ## type  * reg_dst = yalvm_register_to_ ## type (&ctx->registers[dst]); \


yalvm_u32
yalvm_ctx_execute(yalvm_ctx_t* ctx)
{
    /* check valid counter */
    if (!ctx->pc || !ctx->registers || !ctx->binary)
    {
        return YALVM_ERROR_INVALID_CTX;
    }

    while((yalvm_u8*)ctx->pc < ctx->binary_end)
    {
        const yalvm_bytecode_t code = *(ctx->pc++);
        const yalvm_bytecode_inst_t inst = yalvm_bytecode_unpack_instruction(code);
        switch(inst)
        {
        /* End of execution */
        case YALVM_BYTECODE_HALT:
            break;
            /* Load Global */
        case YALVM_BYTECODE_LOAD_GLOBAL_32:
        {
            yalvm_u8 dst_reg;
            yalvm_u16 val;
            yalvm_bytecode_unpack_dst_value(code, &dst_reg, &val);
            ctx->registers[dst_reg].reg32.value = ctx->globals32[val].val;
            break;
        }
        case YALVM_BYTECODE_LOAD_GLOBAL_64:
        {
            yalvm_u8 dst_reg;
            yalvm_u16 val;
            yalvm_bytecode_unpack_dst_value(code, &dst_reg, &val);
            ctx->registers[dst_reg].reg64.value = ctx->globals64[val].val;
            break;
        }
        case YALVM_BYTECODE_STORE_GLOBAL_32:
        {
            yalvm_u8 dst_reg;
            yalvm_u16 val;
            yalvm_bytecode_unpack_dst_value(code, &dst_reg, &val);
            ctx->globals32[val].val = ctx->registers[dst_reg].reg32.value;
            break;
        }
        case YALVM_BYTECODE_STORE_GLOBAL_64:
        {
            yalvm_u8 dst_reg;
            yalvm_u16 val;
            yalvm_bytecode_unpack_dst_value(code, &dst_reg, &val);
            ctx->globals64[val].val = ctx->registers[dst_reg].reg64.value;
            break;
        }
            /* Load value from memory */
        case YALVM_BYTECODE_LOAD_MEMORY:
            return YALVM_ERROR_INSTRUCTION_NOT_IMPLEMENTED;
            /* Store value in memory */
        case YALVM_BYTECODE_STORE_MEMORY:
            return YALVM_ERROR_INSTRUCTION_NOT_IMPLEMENTED;
            /* Load value from bytecode */
        case YALVM_BYTECODE_LOAD_VALUE:
        {
            yalvm_u8 dst_reg;
            yalvm_u16 val;
            yalvm_bytecode_unpack_dst_value(code, &dst_reg, &val);
            ctx->registers[dst_reg].reg64.value = val;
            break;
        }
            /* Load constant  */
        case YALVM_BYTECODE_LOAD_CONST_32:
        {
            yalvm_u8 dst_reg;
            yalvm_u16 val;
            yalvm_bytecode_unpack_dst_value(code, &dst_reg, &val);
            ctx->registers[dst_reg].reg32.value = ctx->constants32[val];
            break;
        }
        case YALVM_BYTECODE_LOAD_CONST_64:
        {
            yalvm_u8 dst_reg;
            yalvm_u16 val;
            yalvm_bytecode_unpack_dst_value(code, &dst_reg, &val);
            ctx->registers[dst_reg].reg64.value = ctx->constants64[val];
            break;
        }
            /* Summation instruction */
        case YALVM_BYTECODE_ADD_I:
        {
            YALVM_UNACK_REGISTERS(i32);
            *reg_dst = *reg_src1 + *reg_src2;
            break;
        }
        case YALVM_BYTECODE_ADD_IL:
        {
            YALVM_UNACK_REGISTERS(i64);
            *reg_dst = *reg_src1 + *reg_src2;
            break;
        }
        case YALVM_BYTECODE_ADD_U:
        {
            YALVM_UNACK_REGISTERS(u32);
            *reg_dst = *reg_src1 + *reg_src2;
            break;
        }
        case YALVM_BYTECODE_ADD_UL:
        {
            YALVM_UNACK_REGISTERS(u64);
            *reg_dst = *reg_src1 + *reg_src2;
            break;
        }
        case YALVM_BYTECODE_ADD_F:
        {
            YALVM_UNACK_REGISTERS(f32);
            *reg_dst = *reg_src1 + *reg_src2;
            break;
        }
        case YALVM_BYTECODE_ADD_FL:
        {
            YALVM_UNACK_REGISTERS(f64);
            *reg_dst = *reg_src1 + *reg_src2;
            break;
        }
            /* Substraction instruction */
        case YALVM_BYTECODE_SUB_I:
        {
            YALVM_UNACK_REGISTERS(i32);
            *reg_dst = *reg_src1 - *reg_src2;
            break;
        }
        case YALVM_BYTECODE_SUB_IL:
        {
            YALVM_UNACK_REGISTERS(i64);
            *reg_dst = *reg_src1 - *reg_src2;
            break;
        }
        case YALVM_BYTECODE_SUB_U:
        {
            YALVM_UNACK_REGISTERS(u32);
            *reg_dst = *reg_src1 - *reg_src2;
            break;
        }
        case YALVM_BYTECODE_SUB_UL:
        {
            YALVM_UNACK_REGISTERS(u64);
            *reg_dst = *reg_src1 - *reg_src2;
            break;
        }
        case YALVM_BYTECODE_SUB_F:
        {
            YALVM_UNACK_REGISTERS(f32);
            *reg_dst = *reg_src1 - *reg_src2;
            break;
        }
        case YALVM_BYTECODE_SUB_FL:
        {
            YALVM_UNACK_REGISTERS(f64);
            *reg_dst = *reg_src1 - *reg_src2;
            break;
        }
            /* Multiplication instruction */
        case YALVM_BYTECODE_MUL_I:
        {
            YALVM_UNACK_REGISTERS(i32);
            *reg_dst = (*reg_src1) * (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_MUL_IL:
        {
            YALVM_UNACK_REGISTERS(i64);
            *reg_dst = (*reg_src1) * (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_MUL_U:
        {
            YALVM_UNACK_REGISTERS(u32);
            *reg_dst = (*reg_src1) * (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_MUL_UL:
        {
            YALVM_UNACK_REGISTERS(u64);
            *reg_dst = (*reg_src1) * (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_MUL_F:
        {
            YALVM_UNACK_REGISTERS(f32);
            *reg_dst = (*reg_src1) * (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_MUL_FL:
        {
            YALVM_UNACK_REGISTERS(f64);
            *reg_dst = (*reg_src1) * (*reg_src2);
            break;
        }
            /* Division instruction */
        case YALVM_BYTECODE_DIV_I:
        {
            YALVM_UNACK_REGISTERS(i32);
            *reg_dst = (*reg_src1) / (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_DIV_IL:
        {
            YALVM_UNACK_REGISTERS(i64);
            *reg_dst = (*reg_src1) / (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_DIV_U:
        {
            YALVM_UNACK_REGISTERS(u32);
            *reg_dst = (*reg_src1) / (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_DIV_UL:
        {
            YALVM_UNACK_REGISTERS(u64);
            *reg_dst = (*reg_src1) / (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_DIV_F:
        {
            YALVM_UNACK_REGISTERS(f32);
            *reg_dst = (*reg_src1) / (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_DIV_FL:
        {
            YALVM_UNACK_REGISTERS(f64);
            *reg_dst = (*reg_src1) / (*reg_src2);
            break;
        }
            /* Negative (-var) */
        case YALVM_BYTECODE_NEG_I:
        {
            YALVM_UNACK_REGISTER(i32);
            *reg_dst = -(*reg_dst);
            break;
        }
        case YALVM_BYTECODE_NEG_IL:
        {
            YALVM_UNACK_REGISTER(i64);
            *reg_dst = -(*reg_dst);
            break;
        }
        case YALVM_BYTECODE_NEG_F:
        {
            YALVM_UNACK_REGISTER(f32);
            *reg_dst = -(*reg_dst);
            break;
        }
        case YALVM_BYTECODE_NEG_FL:
        {
            YALVM_UNACK_REGISTER(f64);
            *reg_dst = -(*reg_dst);
            break;
        }
            /* Bit and */
        case YALVM_BYTECODE_BIT_AND_32:
        {
            YALVM_UNACK_REGISTERS(u32);
            *reg_dst = (*reg_src1) & (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_BIT_AND_64:
        {
            YALVM_UNACK_REGISTERS(u64);
            *reg_dst = (*reg_src1) & (*reg_src2);
            break;
        }
            /* Bit or */
        case YALVM_BYTECODE_BIT_OR_32:
        {
            YALVM_UNACK_REGISTERS(u32);
            *reg_dst = (*reg_src1) | (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_BIT_OR_64:
        {
            YALVM_UNACK_REGISTERS(u64);
            *reg_dst = (*reg_src1) | (*reg_src2);
            break;
        }
            /* Bit xor */
        case YALVM_BYTECODE_BIT_XOR_32:
        {
            YALVM_UNACK_REGISTERS(u32);
            *reg_dst = (*reg_src1) ^ (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_BIT_XOR_64:
        {
            YALVM_UNACK_REGISTERS(u64);
            *reg_dst = (*reg_src1) ^ (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_BIT_NOT_32:
        {
            YALVM_UNACK_REGISTER(u32);
            *reg_dst = ~(*reg_dst);
            break;
        }
        case YALVM_BYTECODE_BIT_NOT_64:
        {
            YALVM_UNACK_REGISTER(u64);
            *reg_dst = ~(*reg_dst);
            break;
        }
            /* Shift Left */
        case YALVM_BYTECODE_SHIFTL_32:
        {
            YALVM_UNACK_REGISTERS(u32);
            *reg_dst = (*reg_src1) << (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_SHIFTL_64:
        {
            YALVM_UNACK_REGISTERS(u64);
            *reg_dst = (*reg_src1) << (*reg_src2);
            break;
        }
            /* Shift Right */
        case YALVM_BYTECODE_SHIFTR_32:
        {
            YALVM_UNACK_REGISTERS(u32);
            *reg_dst = (*reg_src1) >> (*reg_src2);
            break;
        }
        case  YALVM_BYTECODE_SHIFTR_64:
        {
            YALVM_UNACK_REGISTERS(u64);
            *reg_dst = (*reg_src1) >> (*reg_src2);
            break;
        }
            /* Logical And */
        case  YALVM_BYTECODE_AND:
        {
            YALVM_UNACK_REGISTERS(u64);
            *reg_dst = (*reg_src1) && (*reg_src2);
            break;
        }
            /* Logical Or */
        case YALVM_BYTECODE_OR:
        {
            YALVM_UNACK_REGISTERS(u64);
            *reg_dst = (*reg_src1) || (*reg_src2);
            break;
        }
            /* Logical not */
        case YALVM_BYTECODE_NOT:
        {
            YALVM_UNACK_REGISTER(u64);
            *reg_dst = !(*reg_dst);
            break;
        }
            /* Compare Greater */
        case YALVM_BYTECODE_COMPARE_GT_I:
        {
            YALVM_UNACK_REGISTERS(i32);
            *reg_dst = (*reg_src1) > (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_COMPARE_GT_IL:
        {
            YALVM_UNACK_REGISTERS(i64);
            *reg_dst = (*reg_src1) > (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_COMPARE_GT_U:
        {
            YALVM_UNACK_REGISTERS(u32);
            *reg_dst = (*reg_src1) > (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_COMPARE_GT_UL:
        {
            YALVM_UNACK_REGISTERS(u64);
            *reg_dst = (*reg_src1) > (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_COMPARE_GT_F:
        {
            YALVM_UNACK_REGISTERS(f32);
            *reg_dst = (*reg_src1) > (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_COMPARE_GT_FL:
        {
            YALVM_UNACK_REGISTERS(f64);
            *reg_dst = (*reg_src1) > (*reg_src2);
            break;
        }
            /* Compare Greater or Equal */
        case YALVM_BYTECODE_COMPARE_GE_I:
        {
            YALVM_UNACK_REGISTERS(i32);
            *reg_dst = (*reg_src1) >= (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_COMPARE_GE_IL:
        {
            YALVM_UNACK_REGISTERS(i64);
            *reg_dst = (*reg_src1) >= (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_COMPARE_GE_U:
        {
            YALVM_UNACK_REGISTERS(u32);
            *reg_dst = (*reg_src1) >= (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_COMPARE_GE_UL:
        {
            YALVM_UNACK_REGISTERS(u64);
            *reg_dst = (*reg_src1) >= (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_COMPARE_GE_F:
        {
            YALVM_UNACK_REGISTERS(f32);
            *reg_dst = (*reg_src1) >= (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_COMPARE_GE_FL:
        {
            YALVM_UNACK_REGISTERS(f64);
            *reg_dst = (*reg_src1) >= (*reg_src2);
            break;
        }
            /* Compare Lesser */
        case YALVM_BYTECODE_COMPARE_LT_I:
        {
            YALVM_UNACK_REGISTERS(i32);
            *reg_dst = (*reg_src1) < (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_COMPARE_LT_IL:
        {
            YALVM_UNACK_REGISTERS(i64);
            *reg_dst = (*reg_src1) < (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_COMPARE_LT_U:
        {
            YALVM_UNACK_REGISTERS(u32);
            *reg_dst = (*reg_src1) < (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_COMPARE_LT_UL:
        {
            YALVM_UNACK_REGISTERS(u64);
            *reg_dst = (*reg_src1) < (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_COMPARE_LT_F:
        {
            YALVM_UNACK_REGISTERS(f32);
            *reg_dst = (*reg_src1) < (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_COMPARE_LT_FL:
        {
            YALVM_UNACK_REGISTERS(f64);
            *reg_dst = (*reg_src1) < (*reg_src2);
            break;
        }
            /* Compare Lesser or Equal */
        case YALVM_BYTECODE_COMPARE_LE_I:
        {
            YALVM_UNACK_REGISTERS(i32);
            *reg_dst = (*reg_src1) <= (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_COMPARE_LE_IL:
        {
            YALVM_UNACK_REGISTERS(i64);
            *reg_dst = (*reg_src1) <= (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_COMPARE_LE_U:
        {
            YALVM_UNACK_REGISTERS(u32);
            *reg_dst = (*reg_src1) <= (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_COMPARE_LE_UL:
        {
            YALVM_UNACK_REGISTERS(u64);
            *reg_dst = (*reg_src1) <= (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_COMPARE_LE_F:
        {
            YALVM_UNACK_REGISTERS(f32);
            *reg_dst = (*reg_src1) <= (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_COMPARE_LE_FL:
        {
            YALVM_UNACK_REGISTERS(f64);
            *reg_dst = (*reg_src1) <= (*reg_src2);
            break;
        }
            /* Compare Equal */
        case YALVM_BYTECODE_COMPARE_EQ_I:
        {
            YALVM_UNACK_REGISTERS(i32);
            *reg_dst = (*reg_src1) == (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_COMPARE_EQ_IL:
        {
            YALVM_UNACK_REGISTERS(i64);
            *reg_dst = (*reg_src1) == (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_COMPARE_EQ_U:
        {
            YALVM_UNACK_REGISTERS(u32);
            *reg_dst = (*reg_src1) == (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_COMPARE_EQ_UL:
        {
            YALVM_UNACK_REGISTERS(u64);
            *reg_dst = (*reg_src1) == (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_COMPARE_EQ_F:
        {
            YALVM_UNACK_REGISTERS(f32);
            *reg_dst = (*reg_src1) == (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_COMPARE_EQ_FL:
        {
            YALVM_UNACK_REGISTERS(f64);
            *reg_dst = (*reg_src1) == (*reg_src2);
            break;
        }
            /* Compare not Equal*/
        case YALVM_BYTECODE_COMPARE_NE_I:
        {
            YALVM_UNACK_REGISTERS(i32);
            *reg_dst = (*reg_src1) != (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_COMPARE_NE_IL:
        {
            YALVM_UNACK_REGISTERS(i64);
            *reg_dst = (*reg_src1) != (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_COMPARE_NE_U:
        {
            YALVM_UNACK_REGISTERS(u32);
            *reg_dst = (*reg_src1) != (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_COMPARE_NE_UL:
        {
            YALVM_UNACK_REGISTERS(u64);
            *reg_dst = (*reg_src1) != (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_COMPARE_NE_F:
        {
            YALVM_UNACK_REGISTERS(f32);
            *reg_dst = (*reg_src1) != (*reg_src2);
            break;
        }
        case YALVM_BYTECODE_COMPARE_NE_FL:
        {
            YALVM_UNACK_REGISTERS(f64);
            *reg_dst = (*reg_src1) != (*reg_src2);
            break;
        }
            /* Uncoditional Jmp */
        case YALVM_BYTECODE_JUMP:
        {
            /*TODO: destination check */
            yalvm_u32 value;
            yalvm_bytecode_unpack_value(code, &value);
            ctx->pc += value;
            break;
        }
            /* Conditional Jmp */
        case YALVM_BYTECODE_JUMP_TRUE:
        {
            /*TODO: destination check */
            yalvm_u8 dst;
            yalvm_u16 value;
            yalvm_bytecode_unpack_dst_value(code, &dst, &value);
            if (ctx->registers[dst].reg64.value)
            {
                ctx->pc += value;
            }
            break;
        }
        case YALVM_BYTECODE_JUMP_FALSE:
        {
            /*TODO: destination check */
            yalvm_u8 dst;
            yalvm_u16 value;
            yalvm_bytecode_unpack_dst_value(code, &dst, &value);
            if (!ctx->registers[dst].reg64.value)
            {
                ctx->pc += value;
            }
            break;
        }
        case YALVM_BYTECODE_LOAD_FUNCTION:
        {
            /* TODO: Function check */
            yalvm_u8 dst;
            yalvm_u16 value;
            yalvm_bytecode_unpack_dst_value(code, &dst, &value);
            const yalvm_func_header_t* function = ctx->functions[value];

            /* create new stack frame */
            if (!yalvm_stack_frame_begin(&ctx->stack, function->n_registers))
            {
                return YALVM_ERROR_STACK_OVERFLOW;
            }
            /* save function ptr */
            ctx->registers[dst].ptr.value =(void*)function;
            break;
        }
            /* Push Function Argument */
        case YALVM_BYTECODE_PUSH_ARG:
        {
            yalvm_u8 dst;
            yalvm_bytecode_unpack_register(code, &dst);

            if(!yalvm_stack_push(&ctx->stack, &ctx->registers[dst]))
            {
                return YALVM_ERROR_STACK_OVERFLOW;
            }
            break;
        }
            /* Call function */
        case YALVM_BYTECODE_CALL:
        {
            yalvm_u8 dst, return_register, unused;
            yalvm_bytecode_unpack_registers(code, &dst, &return_register, &unused);

            yalvm_func_header_t* function_hdr =
                    (yalvm_func_header_t*) ctx->registers[dst].ptr.value;

            /* prepare stack */
            void* return_register_ptr = NULL;
            if (return_register != 0xFF)
            {
                return_register_ptr = &ctx->registers[return_register];
            }

            if (!yalvm_stack_function_begin(&ctx->stack,
                                            ctx->pc,
                                            return_register_ptr,
                                            function_hdr->n_registers))
            {
                return YALVM_ERROR_STACK_OVERFLOW;
            }

            /* update counter */
            ctx->pc = (yalvm_bytecode_t*) (function_hdr + 1);

            /* update register location */
            ctx->registers = yalvm_stack_local_registers(&ctx->stack);
            YALVM_ASSERT(ctx->registers);

            break;
        }
            /* Return Instruction */
        case YALVM_BYTECODE_RETURN:
        {
            yalvm_u8 dst;
            yalvm_bytecode_unpack_register(code, &dst);

            /* copy return register data */
            if (dst != 0xFF)
            {
                yalvm_register_t* return_register = yalvm_stack_return_register(&ctx->stack);
                YALVM_ASSERT(return_register);
                yalvm_register_copy(return_register, &ctx->registers[dst]);
            }

            /* pop frame */
            if (!yalvm_stack_frame_end(&ctx->stack, (const void **)&ctx->pc))
            {
                return YALVM_ERROR_STACK_UNDERFLOW;
            }

            /* update register location */
            ctx->registers = yalvm_stack_local_registers(&ctx->stack);
            break;
        }
            /* Copy Register */
        case YALVM_BYTECODE_COPY_REGISTER:
        {
            yalvm_u8 dst, src1, unused;
            yalvm_bytecode_unpack_registers(code, &dst, &src1, &unused);
            yalvm_register_copy(&ctx->registers[dst], &ctx->registers[src1]);
        }
            break;
        /* Load String */
        case YALVM_BYTECODE_LOAD_STRING:
        {
            yalvm_u8 dst_reg;
            yalvm_u16 val;
            yalvm_bytecode_unpack_dst_value(code, &dst_reg, &val);
            ctx->registers[dst_reg].ptr.value = (void*)ctx->strings[val];
            break;
        }
        default:
            return YALVM_ERROR_UNKNOW_INSTRUCTION;
        }

    }
    return YALVM_ERROR_NONE;
}

yalvm_u32
yalvm_ctx_num_globals32(const yalvm_ctx_t* ctx)
{
    return (ctx->header) ? ctx->header->n_globals32 : 0;
}

yalvm_u32
yalvm_ctx_num_globals64(const yalvm_ctx_t* ctx)
{
    return (ctx->header) ? ctx->header->n_globals64 : 0;
}

yalvm_u32
yalvm_ctx_num_functions(const yalvm_ctx_t* ctx)
{
    return (ctx->header) ? ctx->header->n_functions : 0;
}

const yalvm_bin_global32_t*
yalvm_ctx_globals32(const yalvm_ctx_t* ctx)
{
    return (ctx->header) ? ctx->globals32 : NULL;
}

const yalvm_bin_global64_t*
yalvm_ctx_globals64(const yalvm_ctx_t* ctx)
{
    return (ctx->header) ? ctx->globals64 : NULL;
}
