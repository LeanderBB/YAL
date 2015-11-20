#include "yal/types/anytype.h"

namespace yal
{

YAL_TYPLE_IMPL_SRC(AnyType, 0xf418b1e1)

static AnyType sAnyType;
AnyType* AnyType::GetType()
{
    return &sAnyType;
}

AnyType::AnyType():
    Type(LanguageTypeId(), kVmTypeTotal, ImpId)
{

}

const char*
AnyType::typeString() const
{
    return "any";
}

}

