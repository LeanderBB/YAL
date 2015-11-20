#ifndef __YALVM_ARRAY_H__
#define __YALVM_ARRAY_H__

#include "yalvm/yalvm.h"
#include "yalvm/yalvm_object.h"

YALVM_MODULE_BGN

typedef struct yalvm_array_base
{
    yalvm_u32   size;
    yalvm_u32   capacity;
    void*       data;
} yalvm_array_base_t;


void
yalvm_array_base_init(yalvm_array_base_t* array,
                      const yalvm_u32 elem_size,
                      const yalvm_u32 initial_capacity);

void
yalvm_array_base_destroy(yalvm_array_base_t* array);

void*
yalvm_array_base_get(const yalvm_array_base_t* array,
                     const yalvm_u32 elem_size,
                     const yalvm_u32 idx);

yalvm_bool
yalvm_array_base_replace(yalvm_array_base_t* array,
                         const yalvm_u32 elem_size,
                         const yalvm_u32 idx,
                         const void* data);

yalvm_bool
yalvm_array_base_insert(yalvm_array_base_t* array,
                        const yalvm_u32 elem_size,
                        const yalvm_u32 idx,
                        const void* data);

yalvm_bool
yalvm_array_base_append(yalvm_array_base_t* array,
                        const yalvm_u32 elem_size,
                        const void* data);

yalvm_bool
yalvm_array_base_remove(yalvm_array_base_t* array,
                        const yalvm_u32 elem_size,
                        const yalvm_u32 idx);

yalvm_bool
yalvm_array_base_clone(yalvm_array_base_t* dst,
                       const yalvm_array_base_t* src,
                       const yalvm_u32 elem_size);

typedef struct
{
    yalvm_array_base_t  array;
    yalvm_u32           elem_size;
} yalvm_array_t;



yalvm_object_t*
yalvm_array_create32(const yalvm_u32 intial_capacity);

yalvm_object_t*
yalvm_array_create64(const yalvm_u32 intial_capacity);

yalvm_object_t*
yalvm_array_createObj(const yalvm_u32 intial_capacity);

void
yalvm_array_destroy(yalvm_object_t* obj);

YALVM_INLINE void
yalvm_array_init(yalvm_array_t* array,
                     const yalvm_u32 elem_size,
                     const yalvm_u32 initial_capacity)
{
    YALVM_ASSERT(elem_size > 0);
    array->elem_size = elem_size;
    yalvm_array_base_init(&array->array, elem_size, initial_capacity);
}

YALVM_INLINE void
yalvm_array_destroy_imp(yalvm_array_t* array)
{
    yalvm_array_base_destroy(&array->array);
    array->elem_size = 0;
}

YALVM_INLINE void*
yalvm_array_get(const yalvm_array_t* array,
                    const yalvm_u32 idx)
{
    return yalvm_array_base_get(&array->array, array->elem_size, idx);
}

YALVM_INLINE yalvm_bool
yalvm_array_replace(yalvm_array_t* array,
                        const yalvm_u32 idx,
                        const void* data)
{
    return yalvm_array_base_replace(&array->array, array->elem_size, idx, data);
}

YALVM_INLINE yalvm_bool
yalvm_array_insert(yalvm_array_t* array,
                       const yalvm_u32 idx,
                       const void* data)
{
    return yalvm_array_base_insert(&array->array, array->elem_size, idx, data);
}

YALVM_INLINE yalvm_bool
yalvm_array_append(yalvm_array_t* array,
                       const void* data)
{
    return yalvm_array_base_append(&array->array, array->elem_size, data);
}

YALVM_INLINE yalvm_bool
yalvm_array_remove(yalvm_array_t* array,
                       const yalvm_u32 idx)
{
    return yalvm_array_base_remove(&array->array, array->elem_size, idx);
}

YALVM_INLINE yalvm_bool
yalvm_array_clone(yalvm_array_t* dst,
                      const yalvm_array_t* src)
{
    const yalvm_bool result = yalvm_array_base_clone(&dst->array, &src->array,
                                                     src->elem_size);
    if (result)
    {
        dst->elem_size = src->elem_size;
    }
    return result;
}

YALVM_INLINE yalvm_u32
yalvm_array_len(const yalvm_array_t* array)
{
    return array->array.size;
}


YALVM_MODULE_END

#endif
