#ifndef __YALVM_BYTECODE_H__
#define __YALVM_BYTECODE_H__


#include "yalvm/yalvm.h"

YALVM_MODULE_BGN

enum yalvm_bytecode_inst
{
    /* End of execution */
    YALVM_BYTECODE_HALT = 0,
    /* Load Global */
    YALVM_BYTECODE_LOAD_GLOBAL_32,
    YALVM_BYTECODE_LOAD_GLOBAL_64,
    /* Store Global */
    YALVM_BYTECODE_STORE_GLOBAL_32,
    YALVM_BYTECODE_STORE_GLOBAL_64,
    /* Load value from memory */
    YALVM_BYTECODE_LOAD_MEMORY,
    /* Store value in memory */
    YALVM_BYTECODE_STORE_MEMORY,
    /* Load value from bytecode */
    YALVM_BYTECODE_LOAD_VALUE,
    /* Load constant  */
    YALVM_BYTECODE_LOAD_CONST_32,
    YALVM_BYTECODE_LOAD_CONST_64,
    /* Summation instruction */
    YALVM_BYTECODE_ADD_I,
    YALVM_BYTECODE_ADD_IL,
    YALVM_BYTECODE_ADD_U,
    YALVM_BYTECODE_ADD_UL,
    YALVM_BYTECODE_ADD_F,
    YALVM_BYTECODE_ADD_FL,
    /* Substraction instruction */
    YALVM_BYTECODE_SUB_I,
    YALVM_BYTECODE_SUB_IL,
    YALVM_BYTECODE_SUB_U,
    YALVM_BYTECODE_SUB_UL,
    YALVM_BYTECODE_SUB_F,
    YALVM_BYTECODE_SUB_FL,
    /* Multiplication instruction */
    YALVM_BYTECODE_MUL_I,
    YALVM_BYTECODE_MUL_IL,
    YALVM_BYTECODE_MUL_U,
    YALVM_BYTECODE_MUL_UL,
    YALVM_BYTECODE_MUL_F,
    YALVM_BYTECODE_MUL_FL,
    /* Division instruction */
    YALVM_BYTECODE_DIV_I,
    YALVM_BYTECODE_DIV_IL,
    YALVM_BYTECODE_DIV_U,
    YALVM_BYTECODE_DIV_UL,
    YALVM_BYTECODE_DIV_F,
    YALVM_BYTECODE_DIV_FL,
    /* Negative (-var) */
    YALVM_BYTECODE_NEG_I,
    YALVM_BYTECODE_NEG_IL,
    YALVM_BYTECODE_NEG_F,
    YALVM_BYTECODE_NEG_FL,
    /* Bit and */
    YALVM_BYTECODE_BIT_AND_32,
    YALVM_BYTECODE_BIT_AND_64,
    /* Bit or */
    YALVM_BYTECODE_BIT_OR_32,
    YALVM_BYTECODE_BIT_OR_64,
    /* Bit xor */
    YALVM_BYTECODE_BIT_XOR_32,
    YALVM_BYTECODE_BIT_XOR_64,
    /* Bit not */
    YALVM_BYTECODE_BIT_NOT_32,
    YALVM_BYTECODE_BIT_NOT_64,
    /* Shift Left */
    YALVM_BYTECODE_SHIFTL_32,
    YALVM_BYTECODE_SHIFTL_64,
    /* Shift Right */
    YALVM_BYTECODE_SHIFTR_32,
    YALVM_BYTECODE_SHIFTR_64,
    /* Logical And */
    YALVM_BYTECODE_AND,
    /* Logical Or */
    YALVM_BYTECODE_OR,
    /* Logical not */
    YALVM_BYTECODE_NOT,
    /* Compare Greater */
    YALVM_BYTECODE_COMPARE_GT_I,
    YALVM_BYTECODE_COMPARE_GT_IL,
    YALVM_BYTECODE_COMPARE_GT_U,
    YALVM_BYTECODE_COMPARE_GT_UL,
    YALVM_BYTECODE_COMPARE_GT_F,
    YALVM_BYTECODE_COMPARE_GT_FL,
    /* Compare Greater or Equal */
    YALVM_BYTECODE_COMPARE_GE_I,
    YALVM_BYTECODE_COMPARE_GE_IL,
    YALVM_BYTECODE_COMPARE_GE_U,
    YALVM_BYTECODE_COMPARE_GE_UL,
    YALVM_BYTECODE_COMPARE_GE_F,
    YALVM_BYTECODE_COMPARE_GE_FL,
    /* Compare Lesser */
    YALVM_BYTECODE_COMPARE_LT_I,
    YALVM_BYTECODE_COMPARE_LT_IL,
    YALVM_BYTECODE_COMPARE_LT_U,
    YALVM_BYTECODE_COMPARE_LT_UL,
    YALVM_BYTECODE_COMPARE_LT_F,
    YALVM_BYTECODE_COMPARE_LT_FL,
    /* Compare Lesser or Equal */
    YALVM_BYTECODE_COMPARE_LE_I,
    YALVM_BYTECODE_COMPARE_LE_IL,
    YALVM_BYTECODE_COMPARE_LE_U,
    YALVM_BYTECODE_COMPARE_LE_UL,
    YALVM_BYTECODE_COMPARE_LE_F,
    YALVM_BYTECODE_COMPARE_LE_FL,
    /* Compare Equal */
    YALVM_BYTECODE_COMPARE_EQ_I,
    YALVM_BYTECODE_COMPARE_EQ_IL,
    YALVM_BYTECODE_COMPARE_EQ_U,
    YALVM_BYTECODE_COMPARE_EQ_UL,
    YALVM_BYTECODE_COMPARE_EQ_F,
    YALVM_BYTECODE_COMPARE_EQ_FL,
    /* Compare not Equal*/
    YALVM_BYTECODE_COMPARE_NE_I,
    YALVM_BYTECODE_COMPARE_NE_IL,
    YALVM_BYTECODE_COMPARE_NE_U,
    YALVM_BYTECODE_COMPARE_NE_UL,
    YALVM_BYTECODE_COMPARE_NE_F,
    YALVM_BYTECODE_COMPARE_NE_FL,
    /* Uncoditional Jmp */
    YALVM_BYTECODE_JUMP,
    /* Conditional Jmp */
    YALVM_BYTECODE_JUMP_TRUE,
    YALVM_BYTECODE_JUMP_FALSE,
    /* Load Function */
    YALVM_BYTECODE_LOAD_FUNCTION,
    /* Push Function Argument */
    YALVM_BYTECODE_PUSH_ARG,
    /* Call function */
    YALVM_BYTECODE_CALL,
    /* Return Instruction */
    YALVM_BYTECODE_RETURN,
    /* Copy Register */
    YALVM_BYTECODE_COPY_REGISTER,
    /* Total instruction count */
    YALVM_BYTECODE_TOTAL
};
typedef enum yalvm_bytecode_inst yalvm_bytecode_inst_t;

typedef yalvm_u32 yalvm_bytecode_t;

/* bytecode parameters mask */
#define YALVM_BYTECODE_MASK_INST     0x000000FF
#define YALVM_BYTECODE_MASK_REG_DST  0x0000FF00
#define YALVM_BYTECODE_MASK_REG_SRC1 0x00FF0000
#define YALVM_BYTECODE_MASK_REG_SRC2 0xFF000000
#define YALVM_BYTECODE_MASK_VALUE_16 0xFFFF0000
#define YALVM_BYTECODE_MASK_VALUE_24 0xFFFFFF00

/* bytecode parameters shift in bytecode*/
enum yalvm_bytecode_shift
{
    YALVM_BYTECODE_SHIFT_INST     = 0,
    YALVM_BYTECODE_SHIFT_REG_DST  = 8,
    YALVM_BYTECODE_SHIFT_REG_SRC1 = 16,
    YALVM_BYTECODE_SHIFT_REG_SRC2 = 24,
    YALVM_BYTECODE_SHIFT_VALUE_16 = 16,
    YALVM_BYTECODE_SHIFT_VALUE_24 = 8
};

/* Pack/unpack helper macros */
#define YALVM_BYTECODE_READ_INST(bc)    ((bc & YALVM_BYTECODE_MASK_INST)     >> YALVM_BYTECODE_SHIFT_INST)
#define YALVM_BYTECODE_READ_RDST(bc)    ((bc & YALVM_BYTECODE_MASK_REG_DST)  >> YALVM_BYTECODE_SHIFT_REG_DST)
#define YALVM_BYTECODE_READ_RSRC1(bc)   ((bc & YALVM_BYTECODE_MASK_REG_SRC1) >> YALVM_BYTECODE_SHIFT_REG_SRC1)
#define YALVM_BYTECODE_READ_RSRC2(bc)   ((bc & YALVM_BYTECODE_MASK_REG_SRC2) >> YALVM_BYTECODE_SHIFT_REG_SRC2)
#define YALVM_BYTECODE_READ_VALUE16(bc) ((bc & YALVM_BYTECODE_MASK_VALUE_16) >> YALVM_BYTECODE_SHIFT_VALUE_16)
#define YALVM_BYTECODE_READ_VALUE24(bc) ((bc & YALVM_BYTECODE_MASK_VALUE_24) >> YALVM_BYTECODE_SHIFT_VALUE_24)

#define YALVM_BYTECODE_WRITE_INST(bc)    ((bc << YALVM_BYTECODE_SHIFT_INST)     & YALVM_BYTECODE_MASK_INST)
#define YALVM_BYTECODE_WRITE_RDST(bc)    ((bc << YALVM_BYTECODE_SHIFT_REG_DST)  & YALVM_BYTECODE_MASK_REG_DST)
#define YALVM_BYTECODE_WRITE_RSRC1(bc)   ((bc << YALVM_BYTECODE_SHIFT_REG_SRC1) & YALVM_BYTECODE_MASK_REG_SRC1)
#define YALVM_BYTECODE_WRITE_RSRC2(bc)   ((bc << YALVM_BYTECODE_SHIFT_REG_SRC2) & YALVM_BYTECODE_MASK_REG_SRC2)
#define YALVM_BYTECODE_WRITE_VALUE16(bc) ((bc << YALVM_BYTECODE_SHIFT_VALUE_16) & YALVM_BYTECODE_MASK_VALUE_16)
#define YALVM_BYTECODE_WRITE_VALUE24(bc) ((bc << YALVM_BYTECODE_SHIFT_VALUE_24) & YALVM_BYTECODE_MASK_VALUE_24)


YALVM_INLINE
yalvm_bytecode_t
yalvm_bytecode_pack_instruction(const yalvm_bytecode_inst_t inst)
{
    yalvm_bytecode_t code = 0;
    code |= YALVM_BYTECODE_WRITE_INST(inst);
    return code;
}


YALVM_INLINE
yalvm_bytecode_t
yalvm_bytecode_pack_one_register(const yalvm_bytecode_inst_t inst,
                                 const yalvm_u8 dst_reg)
{
    yalvm_bytecode_t code = 0;
    code |= YALVM_BYTECODE_WRITE_INST(inst);
    code |= YALVM_BYTECODE_WRITE_RDST(dst_reg);
    return code;
}

YALVM_INLINE
yalvm_bytecode_t
yalvm_bytecode_pack_two_registers(const yalvm_bytecode_inst_t inst,
                                  const yalvm_u8 dst_reg,
                                  const yalvm_u8 src_reg1)
{
    yalvm_bytecode_t code = 0;
    code |= YALVM_BYTECODE_WRITE_INST(inst);
    code |= YALVM_BYTECODE_WRITE_RDST(dst_reg);
    code |= YALVM_BYTECODE_WRITE_RSRC1(src_reg1);
    return code;
}


YALVM_INLINE
yalvm_bytecode_t
yalvm_bytecode_pack_three_registers(const yalvm_bytecode_inst_t inst,
                                    const yalvm_u8 dst_reg,
                                    const yalvm_u8 src_reg1,
                                    const yalvm_u8 src_reg2)
{
    yalvm_bytecode_t code = 0;
    code |= YALVM_BYTECODE_WRITE_INST(inst);
    code |= YALVM_BYTECODE_WRITE_RDST(dst_reg);
    code |= YALVM_BYTECODE_WRITE_RSRC1(src_reg1);
    code |= YALVM_BYTECODE_WRITE_RSRC2(src_reg2);
    return code;
}

YALVM_INLINE
yalvm_bytecode_t
yalvm_bytecode_pack_dst_value(const yalvm_bytecode_inst_t inst,
                              const yalvm_u8 dst_reg,
                              const yalvm_u16 value)
{
    yalvm_bytecode_t code = 0;
    code |= YALVM_BYTECODE_WRITE_INST(inst);
    code |= YALVM_BYTECODE_WRITE_RDST(dst_reg);
    code |= YALVM_BYTECODE_WRITE_VALUE16(value);
    return code;
}

YALVM_INLINE
yalvm_bytecode_t
yalvm_bytecode_pack_dst_value_signed(const yalvm_bytecode_inst_t inst,
                                     const yalvm_u8 dst_reg,
                                     const yalvm_i16 value)
{
    yalvm_bytecode_t code = 0;
    code |= YALVM_BYTECODE_WRITE_INST(inst);
    code |= YALVM_BYTECODE_WRITE_RDST(dst_reg);
    code |= YALVM_BYTECODE_WRITE_VALUE16(value);
    return code;
}

YALVM_INLINE
yalvm_bytecode_t
yalvm_bytecode_pack_value(const yalvm_bytecode_inst_t inst,
                          const yalvm_u32 value)
{
    yalvm_bytecode_t code = 0;
    code |= YALVM_BYTECODE_WRITE_INST(inst);
    code |= YALVM_BYTECODE_WRITE_VALUE24(value);
    return code;
}


YALVM_INLINE
yalvm_bytecode_inst_t
yalvm_bytecode_unpack_instruction(const yalvm_bytecode_t code)
{
    return (yalvm_bytecode_inst_t) YALVM_BYTECODE_READ_INST(code);
}

YALVM_INLINE
void
yalvm_bytecode_unpack_register(const yalvm_bytecode_t code,
                                yalvm_u8* dst_reg)
{
    *dst_reg =  YALVM_BYTECODE_READ_RDST(code);
}


YALVM_INLINE
void
yalvm_bytecode_unpack_registers(const yalvm_bytecode_t code,
                                yalvm_u8* dst_reg,
                                yalvm_u8* src_reg1,
                                yalvm_u8* src_reg2)
{
    *dst_reg =  YALVM_BYTECODE_READ_RDST(code);
    *src_reg1 = YALVM_BYTECODE_READ_RSRC1(code);
    *src_reg2 = YALVM_BYTECODE_READ_RSRC2(code);
}

YALVM_INLINE
void
yalvm_bytecode_unpack_dst_value(const yalvm_bytecode_t code,
                                yalvm_u8* dst_reg,
                                yalvm_u16* value)
{
    *dst_reg =  YALVM_BYTECODE_READ_RDST(code);
    *value = YALVM_BYTECODE_READ_VALUE16(code);
}

YALVM_INLINE
void
yalvm_bytecode_unpack_dst_value_signed(const yalvm_bytecode_t code,
                                       yalvm_u8* dst_reg,
                                       yalvm_i16* value)
{
    *dst_reg =  YALVM_BYTECODE_READ_RDST(code);
    *value = YALVM_BYTECODE_READ_VALUE16(code);
}

YALVM_INLINE
void
yalvm_bytecode_unpack_value(const yalvm_bytecode_t code,
                            yalvm_u32* value)
{
    *value = YALVM_BYTECODE_READ_VALUE24(code);
}

const char*
yalvm_bytecode_inst_to_str(const yalvm_bytecode_inst_t inst);


YALVM_MODULE_END

#endif
