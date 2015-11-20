#ifndef __YALVM_REGISTER_H__
#define __YALVM_REGISTER_H__

#include "yalvm/yalvm.h"

YALVM_MODULE_BGN

struct yalvm_object;
typedef union yalvm_register
{
    union
    {
        yalvm_bool b;
        yalvm_f32 f;
        yalvm_i32 i;
        yalvm_u32 u;
        yalvm_u32 value;
    } reg32;
    union
    {
        yalvm_f64 fl;
        yalvm_i64 il;
        yalvm_u64 ul;
        yalvm_u64 value;
    } reg64;
    union
    {
        void* value;
        union yalvm_register* reg;
        struct yalvm_object* obj;
    } ptr;
} yalvm_register_t;

YALVM_INLINE yalvm_i32*
yalvm_register_to_i32(yalvm_register_t* reg)
{
    return &reg->reg32.i;
}

YALVM_INLINE yalvm_u32*
yalvm_register_to_u32(yalvm_register_t* reg)
{
    return &reg->reg32.u;
}

YALVM_INLINE yalvm_f32*
yalvm_register_to_f32(yalvm_register_t* reg)
{
    return &reg->reg32.f;
}

YALVM_INLINE yalvm_i64*
yalvm_register_to_i64(yalvm_register_t* reg)
{
    return &reg->reg64.il;
}

YALVM_INLINE yalvm_u64*
yalvm_register_to_u64(yalvm_register_t* reg)
{
    return &reg->reg64.ul;
}

YALVM_INLINE yalvm_f64*
yalvm_register_to_f64(yalvm_register_t* reg)
{
    return &reg->reg64.fl;
}

YALVM_INLINE void
yalvm_register_copy(yalvm_register_t* dst,
                    const yalvm_register_t* src)
{
    dst->reg64.value = src->reg64.value;
}

YALVM_MODULE_END

#endif

