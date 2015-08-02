#ifndef __YALVM_REGISTER_H__
#define __YALVM_REGISTER_H__

#include "yalvm/yalvm.h"

YALVM_MODULE_BGN

typedef union
{
    struct
    {
        yalvm_u32 value;
    } reg32;
    struct
    {
        yalvm_u64 value;
    } reg64;
    struct
    {
        yalvm_size value;
    } ptr;
} yalvm_register_t;

YALVM_INLINE yalvm_i32*
yalvm_register_to_i32(yalvm_register_t* reg)
{
    union
    {
        yalvm_u32* in;
        yalvm_i32* out;
    } u;
    u.in = &reg->reg32.value;
    return u.out;
}

YALVM_INLINE yalvm_u32*
yalvm_register_to_u32(yalvm_register_t* reg)
{
    return &reg->reg32.value;
}

YALVM_INLINE yalvm_f32*
yalvm_register_to_f32(yalvm_register_t* reg)
{
    union
    {
        yalvm_u32* in;
        yalvm_f32* out;
    } u;
    u.in = &reg->reg32.value;
    return u.out;
}

YALVM_INLINE yalvm_i64*
yalvm_register_to_i64(yalvm_register_t* reg)
{
    union
    {
        yalvm_u64* in;
        yalvm_i64* out;
    } u;
    u.in = &reg->reg64.value;
    return u.out;
}

YALVM_INLINE yalvm_u64*
yalvm_register_to_u64(yalvm_register_t* reg)
{
    return &reg->reg64.value;
}

YALVM_INLINE yalvm_f64*
yalvm_register_to_f64(yalvm_register_t* reg)
{
    union
    {
        yalvm_u64* in;
        yalvm_f64* out;
    } u;
    u.in = &reg->reg64.value;
    return u.out;
}

YALVM_INLINE void
yalvm_register_copy(yalvm_register_t* dst,
                    const yalvm_register_t* src)
{
    dst->reg64.value = src->reg64.value;
}

YALVM_MODULE_END

#endif

