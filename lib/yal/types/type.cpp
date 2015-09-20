#include "yal/types/type.h"

namespace yal
{

yal_u32
LanguageModuleId()
{
    return 0xFFFFFFFF;
}

yal_u32
LanguageDataTypeId()
{
    return 0xFFFFFFFF;
}

TypeId
LanguageTypeId()
{
    TypeId id;
    id.split.dataTypeId = LanguageDataTypeId();
    id.split.moduleId = LanguageModuleId();
    return id;
}

Type::Type(const TypeId_t& id,
           const VmType vmType,
           const Id_t impId):
    _impid(impId),
    _typeId(id),
    _vmType(vmType),
    _typeFlags(0),
    _operatorFlags(0)
{

}

bool
Type::isVoidType() const
{
    return _typeFlags & kTypeFlagIsVoidType;
}

bool
Type::isBuiltinType() const
{
    return _typeFlags & kTypeFlagIsBuiltinType;
}

bool
Type::isStringType() const
{
     return _typeFlags & kTypeFlagIsStringType;
}

bool
Type::isFunctionType() const
{
     return _typeFlags & kTypeFlagIsFunctionType;
}

bool
Type::isObjectType() const
{
    return _typeFlags & kTypeFlagIsObjectType;
}

bool
Type::isArrayType() const
{
    return _typeFlags & kTypeFlagIsArrayType;
}

bool
Type::isDictionaryType() const
{
    return _typeFlags & kTypeFlagIsDictionaryType;
}

bool
Type::isInteger() const
{
    return _typeFlags & kTypeFlagIsInteger;
}

bool
Type::isSignedInteger() const
{
    return isInteger() &&  (_typeFlags & kTypeFlagIsSignedType);
}

bool
Type::isUnsignedInteger() const
{
        return isInteger() &&  (_typeFlags & kTypeFlagIsUnsignedType);
}

bool
Type::isDecimal() const
{
    return _typeFlags & kTypeFlagIsDecimal;
}

bool
Type::isBoolean() const
{
    return _typeFlags & kTypeFlagIsBoolean;
}

bool
Type::isCompoundType() const
{
    return _typeFlags & kTypeFlagIsCompoundType;
}

bool
Type::is32BitSized() const
{
    return _typeFlags & kTypeFlagIs32BitsSized;
}

bool
Type::is64BitSized() const
{
    return _typeFlags & kTypeFlagIs64BitsSized;
}

bool
Type::isPointerSized() const
{
    return _typeFlags & kTypeFlagIsPtrSized;
}

bool
Type::isPromotableToBoolean() const
{
    return _typeFlags & kTypeFlagBooleanPromotable;
}

bool
Type::acceptsOperator(const OperatorType op) const
{
    return _operatorFlags & op;
}


}
