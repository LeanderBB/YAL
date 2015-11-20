#include "yalvm/yalvm_builtin.h"
#include "yalvm/yalvm_array.h"
#include "yalvm/yalvm_string.h"
#include "yalvm/yalvm_register.h"

/** Get array length */
void
yal_native_array_any_length_uint32(const yalvm_u32 reg_count,
                                   yalvm_register_t* registers,
                                   yalvm_register_t* return_register)
{
   (void) reg_count;
   yalvm_array_t* array = (yalvm_array_t*)registers[0].ptr.obj->ptr;
   return_register->reg32.u = array->array.size;
}
