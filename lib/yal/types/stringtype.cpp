#include "yal/types/stringtype.h"

namespace  yal
{

YAL_TYPLE_IMPL_SRC(StringType, 0x02249b1e)



static StringType sStringType;

StringType*
StringType::GetType()
{
    return &sStringType;
}

StringType::StringType():
    Type(LanguageTypeId(), kVmTypeString, StringType::ImpId)
{
    _typeFlags = kTypeFlagIsBuiltinType
            | kTypeFlagIsObjectType
            | kTypeFlagIsStringType
            | kTypeFlagIsPtrSized;

    _operatorFlags = kOperatorTypeCopy;
}

bool
StringType::isPromotableTo(const Type *t) const
{
    const StringType* other = cast_type<const StringType>(t);
    return other != nullptr;
}

}


