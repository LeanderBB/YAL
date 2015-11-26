#include "yalvm/yalvm_string.h"


yalvm_object_t*
yalvm_string_create_empty(void)
{
    yalvm_object_t* object = yalvm_object_alloc(sizeof(yalvm_string_t));
    if (object)
    {
        yalvm_string_t* string = (yalvm_string_t*) object->ptr;
        yalvm_array_base_init(&string->array, 1, 16);
    }
    return object;
}

yalvm_object_t*
yalvm_string_create_with_constant(const char* text)
{
    yalvm_object_t* object = yalvm_object_alloc(sizeof(yalvm_string_t));
    if (object)
    {
        const yalvm_u32 string_len = (yalvm_u32) strlen(text);
        yalvm_string_t* string = (yalvm_string_t*) object->ptr;
        yalvm_array_base_init(&string->array, 1, string_len + 1);
        string->array.size = string_len;
        memcpy(string->array.data, text, string_len + 1);
    }
    return object;
}

void
yalvm_string_destroy(yalvm_object_t* obj)
{
    yalvm_string_t* string = (yalvm_string_t*) obj->ptr;
    yalvm_array_base_destroy(&string->array);
    yalvm_object_dealloc(obj);
}

const char*
yalvm_string_str(const yalvm_object_t* obj)
{
    const yalvm_string_t* string = (const yalvm_string_t*) obj->ptr;
    return (const char*)string->array.data;
}
