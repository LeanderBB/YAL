#include "yal/types/undefined.h"


namespace yal
{

YAL_TYPLE_IMPL_SRC(UndefinedType, 0x8d99d174)

UndefinedType::UndefinedType(const TypeId_t& id,
                             const char *typeString):
    Type(id, kVmTypeTotal, UndefinedType::ImpId),
    _typeString(typeString)
{

}

}
