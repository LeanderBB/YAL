#include "yal/types/arraytype.h"
#include "yal/types/typehdrs.h"
namespace yal
{

YAL_TYPLE_IMPL_SRC(ArrayType, 0xb7125643)

std::string
ArrayType::GenTypeString(const Type* valueType)
{
    return '[' + std::string(valueType->typeString())+ ']';
}

std::string
ArrayType::GenTypeStringNative(const Type* valueType)
{
    return "array_" + std::string(valueType->typeString());
}

ArrayType::ArrayType(const TypeId_t& id,
                     const Type* valueType):
    Type(id, kVmTypeArray, ImpId),
    _arrayTypeString(GenTypeString(valueType)),
    _nativeArrayTypeString(GenTypeStringNative(valueType)),
    _valueType(valueType)
{
    _typeFlags = kTypeFlagIsPtrSized | kTypeFlagIsObjectType
        | kTypeFlagIsArrayType | kTypeFlagIsCompoundType | kTypeFlagIsBuiltinType;
    _operatorFlags = kOperatorTypeArray;
}

bool
ArrayType::isPromotableTo(const Type *t) const
{
    const ArrayType* other = cast_type<const ArrayType>(t);
    if (other)
    {
        if (cast_type<const AnyType>(other->valueType())) return true;
        return other->valueType()->impId() == _valueType->impId();
    }
    return false;
}


}

