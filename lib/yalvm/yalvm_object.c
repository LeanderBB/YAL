#include "yalvm/yalvm_object.h"
#include "yalvm/yalvm_ctx.h"

yalvm_object_t*
yalvm_object_alloc(const yalvm_size size,
                   const yalvm_u32 objdesc_idx)
{
    yalvm_object_t* result = yalvm_malloc(sizeof(yalvm_object_t) + size);
    if (result)
    {
        result->nref = 1;
        result->ptr = YALVM_PTR_ADD(result, sizeof(yalvm_object_t));
        result->objdesc_idx = objdesc_idx;
    }
    return result;
}

void
yalvm_object_acquire(yalvm_object_t* obj)
{
    ++obj->nref;
}

yalvm_bool
yalvm_object_release(yalvm_object_t* obj,
                     struct yalvm_ctx *ctx)
{
    (void) ctx;
    --obj->nref;
    if (obj->nref <= 0)
    {
        /* call dtor */

        /* free memory */
        yalvm_free(obj);
        return yalvm_true;
    }
    return yalvm_false;
}
