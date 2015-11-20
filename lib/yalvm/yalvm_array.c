#include "yalvm/yalvm_array.h"

static yalvm_bool
yalvm_array_base_grow(yalvm_array_base_t* array,
                      const yalvm_u32 elem_size)
{
    const yalvm_u32 new_size = array->size + 1;

    /* check overflow */
    if (new_size < array->size)
    {
        return yalvm_false;
    }

    if (new_size >= array->capacity)
    {
        const yalvm_u32 new_capacity = (array->capacity)
                ? array->capacity * 1.25
                : 8;
        if (new_capacity < array->capacity)
        {
            return yalvm_false;
        }

        void* new_ptr = yalvm_realloc(array->data, new_capacity * elem_size);
        if (!new_ptr)
        {
            return yalvm_false;
        }
        array->capacity = new_capacity;
        array->data = new_ptr;
    }
    return yalvm_true;
}

void
yalvm_array_base_init(yalvm_array_base_t* array,
                      const yalvm_u32 elem_size,
                      const yalvm_u32 initial_capacity)
{
    array->size = 0;
    array->capacity = initial_capacity * elem_size;
    array->data = (array->capacity) ? yalvm_malloc(elem_size * initial_capacity) : NULL;
}

void
yalvm_array_base_destroy(yalvm_array_base_t* array)
{
    array->size = 0;
    array->capacity = 0;
    if (array->data)
    {
        yalvm_free(array->data);
    }
    array->data = NULL;
}

void*
yalvm_array_base_get(const yalvm_array_base_t* array,
                     const yalvm_u32 elem_size,
                     const yalvm_u32 idx)

{
    return (idx < array->size)
            ? YALVM_PTR_ADD(array->data, elem_size * idx)
            : NULL;
}

yalvm_bool
yalvm_array_base_replace(yalvm_array_base_t* array,
                         const yalvm_u32 elem_size,
                         const yalvm_u32 idx,
                         const void* data)
{
    if (idx < array->size)
    {
        void* elem_ptr = YALVM_PTR_ADD(array->data, elem_size * idx);
        yalvm_memcpy(elem_ptr, data, elem_size);
        return yalvm_true;
    }
    return yalvm_false;
}

yalvm_bool
yalvm_array_base_insert(yalvm_array_base_t* array,
                        const yalvm_u32 elem_size,
                        const yalvm_u32 idx,
                        const void* data)
{
    if (idx > array->size)
    {
        return yalvm_false;
    }

    if (!yalvm_array_base_grow(array, elem_size))
    {
        return yalvm_false;
    }

    void* elem_ptr = YALVM_PTR_ADD(array->data, elem_size * idx);
    if (idx < array->size)
    {
        void* dst_ptr = YALVM_PTR_ADD(array->data, elem_size * (idx + 1));
        void* end_ptr = YALVM_PTR_ADD(array->data, elem_size * array->size);
        const yalvm_size content_size = ((yalvm_size)end_ptr - (yalvm_size)elem_ptr);
        yalvm_memmove(dst_ptr, elem_ptr, content_size);
    }
    yalvm_memcpy(elem_ptr, data, elem_size);
    ++array->size;

    return yalvm_true;
}

yalvm_bool
yalvm_array_base_append(yalvm_array_base_t* array,
                        const yalvm_u32 elem_size,
                        const void* data)
{
    if (!yalvm_array_base_grow(array, elem_size))
    {
        return yalvm_false;
    }

    void* end_ptr = YALVM_PTR_ADD(array->data, elem_size * array->size);
    yalvm_memcpy(end_ptr, data, elem_size);
    ++array->size;
    return yalvm_true;
}

yalvm_bool
yalvm_array_base_remove(yalvm_array_base_t* array,
                        const yalvm_u32 elem_size,
                        const yalvm_u32 idx)
{
    if (idx >= array->size)
    {
        return yalvm_false;
    }

    if (idx != array->size - 1)
    {
        void* elem_ptr = YALVM_PTR_ADD(array->data, elem_size * idx);
        void* nxt_elem_ptr = YALVM_PTR_ADD(array->data, elem_size * (idx + 1));
        void* end_ptr = YALVM_PTR_ADD(array->data, elem_size * array->size);
        const yalvm_size content_size = ((yalvm_size)end_ptr - (yalvm_size)nxt_elem_ptr);
        yalvm_memmove(elem_ptr, nxt_elem_ptr, content_size);
    }
    --array->size;
    return yalvm_true;
}

yalvm_bool
yalvm_array_base_clone(yalvm_array_base_t* dst,
                       const yalvm_array_base_t* src,
                       const yalvm_u32 elem_size)
{
    yalvm_array_base_destroy(dst);
    yalvm_array_base_init(dst, elem_size, src->capacity);
    yalvm_memcpy(dst->data, src->data, elem_size * src->size);
    return yalvm_true;
}

yalvm_object_t*
yalvm_array_create32(const yalvm_u32 intial_capacity)
{
    yalvm_object_t* object = yalvm_object_alloc(sizeof(yalvm_array_t));
    if (object)
    {
        yalvm_array_t* array = (yalvm_array_t*) object->ptr;
        yalvm_array_init(array, sizeof(yalvm_u32), intial_capacity);
    }
    return object;
}

yalvm_object_t*
yalvm_array_create64(const yalvm_u32 intial_capacity)
{
    yalvm_object_t* object = yalvm_object_alloc(sizeof(yalvm_array_t));
    if (object)
    {
        yalvm_array_t* array = (yalvm_array_t*) object->ptr;
        yalvm_array_init(array, sizeof(yalvm_u64), intial_capacity);
    }
    return object;
}

yalvm_object_t*
yalvm_array_createObj(const yalvm_u32 intial_capacity)
{
    yalvm_object_t* object = yalvm_object_alloc(sizeof(yalvm_array_t));
    if (object)
    {
        yalvm_array_t* array = (yalvm_array_t*) object->ptr;
        yalvm_array_init(array, sizeof(void*), intial_capacity);
    }
    return object;
}

void
yalvm_array_destroy(yalvm_object_t* object)
{
    yalvm_array_t* array = (yalvm_array_t*) object->ptr;
    yalvm_array_destroy_imp(array);
    yalvm_object_dealloc(object);
}
