#include "yalvm/yalvm_object.h"
#include "yalvm/yalvm_ctx.h"

yalvm_object_t*
yalvm_object_alloc(const yalvm_size size)
{
    yalvm_object_t* result = yalvm_malloc(sizeof(yalvm_object_t) + size);
    if (result)
    {
        result->nref = 1;
        result->ptr = YALVM_PTR_ADD(result, sizeof(yalvm_object_t));
    }
    return result;
}

void
yalvm_object_acquire(yalvm_object_t* obj)
{
    ++obj->nref;
}

yalvm_bool
yalvm_object_release(yalvm_object_t* obj)
{
    --obj->nref;
    return (obj->nref <= 0) ? yalvm_true : yalvm_false;
}
