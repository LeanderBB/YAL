#include "yal/types/builtintype.h"

namespace yal
{

static BuiltinType sTypeVoid(
        kVmTypeTotal,
        0,
        0);

static BuiltinType sTypeBool(
        kVmTypeUInt32,
        kTypeFlagIsBuiltinType | kTypeFlagIsInteger
        | kTypeFlagIs32BitsSized | kTypeFlagIsUnsignedType
        | kTypeFlagBooleanPromotable,
        kOperatorMaskBoolean);

static BuiltinType sTypeInt32(
        kVmTypeInt32,
        kTypeFlagIsBuiltinType | kTypeFlagIsInteger
        | kTypeFlagIs32BitsSized | kTypeFlagIsSignedType
        | kTypeFlagBooleanPromotable,
        kOperatorMaskIntegers);

static BuiltinType sTypeUInt32(
        kVmTypeUInt32,
        kTypeFlagIsBuiltinType | kTypeFlagIsInteger
        | kTypeFlagIs32BitsSized | kTypeFlagIsUnsignedType
        | kTypeFlagBooleanPromotable,
        kOperatorMaskIntegers);

static BuiltinType sTypeInt64(
        kVmTypeInt64,
        kTypeFlagIsBuiltinType | kTypeFlagIsInteger
        | kTypeFlagIs64BitsSized | kTypeFlagIsSignedType
        | kTypeFlagBooleanPromotable,
        kOperatorMaskIntegers);

static BuiltinType sTypeUInt64(
        kVmTypeUInt64,
        kTypeFlagIsBuiltinType | kTypeFlagIsInteger
        | kTypeFlagIs64BitsSized | kTypeFlagIsUnsignedType
        | kTypeFlagBooleanPromotable,
        kOperatorMaskIntegers);

static BuiltinType sTypeFloat32(
        kVmTypeFloat32,
        kTypeFlagIsBuiltinType | kTypeFlagIsDecimal
        | kTypeFlagIs32BitsSized | kTypeFlagIsSignedType
        | kTypeFlagBooleanPromotable,
        kOperatorMaskDecimals);

static BuiltinType sTypeFloat64(
        kVmTypeFloat64,
        kTypeFlagIsBuiltinType | kTypeFlagIsDecimal
        | kTypeFlagIs64BitsSized | kTypeFlagIsSignedType
        | kTypeFlagBooleanPromotable,
        kOperatorMaskDecimals);

BuiltinType*
BuiltinType::GetBuiltinType(const yal_u32 type)
{
    static BuiltinType* s_static_types[] =
    {
        &sTypeVoid,
        &sTypeBool,
        &sTypeInt32,
        &sTypeInt64,
        &sTypeUInt32,
        &sTypeUInt64,
        &sTypeFloat32,
        &sTypeFloat64
    };

    static_assert(YAL_ARRAY_SIZE(s_static_types) == kTotal,
                  "Missing builtin type from array initialization");

    return (type < kTotal) ? s_static_types[type] : nullptr;
}


BuiltinType::BuiltinType(const VmType vmType,
                         const yal_u32 typeFlags,
                         const yal_u32 operatorFlags):
    Type(LanguageTypeId(), vmType)
{
    _typeFlags = typeFlags;
    _operatorFlags = operatorFlags;
}

}

