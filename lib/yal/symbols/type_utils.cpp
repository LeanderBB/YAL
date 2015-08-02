#include "yal/symbols/type_utils.h"

namespace yal
{

static const bool
sTypePromotionTable [kConstantTypeMax][kConstantTypeMax] =
{
  // none   id      i32     u32     i64     u64     bool    f32     f64
    {true, false,   false,  false,  false,  false,  false,  false,  false}, // none
    {false, true ,  false,  false,  false,  false,  false,  false,  false}, // id
    {false, false,   true,  false,  false,  false,  true,   false,  false}, // i32
    {false, false,  false,  true,   false,  false,  true,   false,  false}, // u32
    {false, false,  false,  false,  true ,  false,  true,   false,  false}, // i64
    {false, false,  false,  false,  false,  true ,  true,   false,  false}, // u64
    {false, false,  true ,  true ,  true,   true,   true,   false,  false}, // bool
    {false, false,  false,  false,  false,  false,  false,  true ,  true }, // f32
    {false, false,  false,  false,  false,  false,  false,  false,  true }  // f64
};

bool
CanTypeBePromoted(const ConstantType from,
               const ConstantType to)
{
    if(from >= kConstantTypeMax || to >= kConstantTypeMax)
    {
        return false;
    }

    return sTypePromotionTable[from][to];
}

bool
IsValidBoolean(const ConstantType from)
{
    return sTypePromotionTable[from][kConstantTypeBool];
}


}

