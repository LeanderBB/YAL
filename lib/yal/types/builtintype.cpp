#include "yal/types/builtintype.h"
#include "yal/types/typehdrs.h"
namespace yal
{

static BuiltinType sTypeVoid(
        kVmTypeTotal,
        kTypeFlagIsBuiltinType | kTypeFlagIsVoidType,
        0,
        BuiltinType::kVoid);

static BuiltinType sTypeBool(
        kVmTypeUInt32,
        kTypeFlagIsBuiltinType | kTypeFlagIsInteger
        | kTypeFlagIs32BitsSized | kTypeFlagIsUnsignedType
        | kTypeFlagBooleanPromotable,
        kOperatorMaskBoolean,
        BuiltinType::kBool);

static BuiltinType sTypeInt32(
        kVmTypeInt32,
        kTypeFlagIsBuiltinType | kTypeFlagIsInteger
        | kTypeFlagIs32BitsSized | kTypeFlagIsSignedType
        | kTypeFlagBooleanPromotable,
        kOperatorMaskIntegers,
        BuiltinType::kInt32);

static BuiltinType sTypeUInt32(
        kVmTypeUInt32,
        kTypeFlagIsBuiltinType | kTypeFlagIsInteger
        | kTypeFlagIs32BitsSized | kTypeFlagIsUnsignedType
        | kTypeFlagBooleanPromotable,
        kOperatorMaskIntegers,
        BuiltinType::kUInt32);

static BuiltinType sTypeInt64(
        kVmTypeInt64,
        kTypeFlagIsBuiltinType | kTypeFlagIsInteger
        | kTypeFlagIs64BitsSized | kTypeFlagIsSignedType
        | kTypeFlagBooleanPromotable,
        kOperatorMaskIntegers,
        BuiltinType::kInt64);

static BuiltinType sTypeUInt64(
        kVmTypeUInt64,
        kTypeFlagIsBuiltinType | kTypeFlagIsInteger
        | kTypeFlagIs64BitsSized | kTypeFlagIsUnsignedType
        | kTypeFlagBooleanPromotable,
        kOperatorMaskIntegers,
        BuiltinType::kUInt64);

static BuiltinType sTypeFloat32(
        kVmTypeFloat32,
        kTypeFlagIsBuiltinType | kTypeFlagIsDecimal
        | kTypeFlagIs32BitsSized | kTypeFlagIsSignedType
        | kTypeFlagBooleanPromotable,
        kOperatorMaskDecimals,
        BuiltinType::kFloat32);

static BuiltinType sTypeFloat64(
        kVmTypeFloat64,
        kTypeFlagIsBuiltinType | kTypeFlagIsDecimal
        | kTypeFlagIs64BitsSized | kTypeFlagIsSignedType
        | kTypeFlagBooleanPromotable,
        kOperatorMaskDecimals,
        BuiltinType::kFloat64);

static BuiltinType sTypeStringConstant(
        kVmTypeStringConstant,
        kTypeFlagIsBuiltinType | kTypeFlagIsStringConstant
        | kTypeFlagIsPtrSized,
        0,
        BuiltinType::kStringConstant);

YAL_TYPLE_IMPL_SRC(BuiltinType, 0x3aac95af)

BuiltinType*
BuiltinType::GetBuiltinType(const Types type)
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
        &sTypeFloat64,
        &sTypeStringConstant
    };

    static_assert(YAL_ARRAY_SIZE(s_static_types) == kTotal,
                  "Missing builtin type from array initialization");

    return (type < kTotal) ? s_static_types[type] : nullptr;
}

BuiltinType*
BuiltinType::GetBuiltinTypeForConstantType(const ConstantType type)
{
    switch(type)
    {
    case kConstantTypeNone:
        return &sTypeVoid;
    case kConstantTypeBool:
        return &sTypeBool;
    case kConstantTypeUInt32:
        return &sTypeUInt32;
    case kConstantTypeInt32:
        return &sTypeInt32;
    case kConstantTypeInt64:
        return &sTypeInt64;
    case kConstantTypeUInt64:
        return &sTypeUInt64;
    case kConstantTypeFloat32:
        return &sTypeFloat32;
    case kConstantTypeFloat64:
        return &sTypeFloat64;
    case kConstantTypeText:
        return &sTypeStringConstant;
    default:
        YAL_ASSERT(false && "Shouldn't be reached");
        return nullptr;
    }
}


BuiltinType::BuiltinType(const VmType vmType,
                         const yal_u32 typeFlags,
                         const yal_u32 operatorFlags,
                         const Types builtinDataType):
    Type(LanguageTypeId(), vmType, BuiltinType::ImpId),
    _builtinType(builtinDataType)
{
    _typeFlags = typeFlags;
    _operatorFlags = operatorFlags;
}


static const bool
sTypePromotionTable [BuiltinType::kTotal][BuiltinType::kTotal] =
{
    //void  bool    i32     i64     u32     u64      f32     f64
    {false, false,  false,  false,  false,  false,  false,  false}, // void
    {false, true ,  false,  false,  false,  false,  false,  false}, // bool
    {false, true ,  true,   true,   false,  false,  false,  false}, // i32
    {false, true ,  false,  true,   false,  false,  false,  false}, // i64
    {false, true ,  false,  false,  true,   true ,  false,  false}, // u32
    {false, true ,  false,  false,  false,  true ,  false,  false}, // u64
    {false, false,  false,  false,  false,  false,  true ,  true }, // f32
    {false, false,  false,  false,  false,  false,  false,  true }  // f64
};

bool
BuiltinType::isPromotableTo(const Type* t) const
{
    if (cast_type<const AnyType>(t)) return true;
    const BuiltinType* other = cast_type<const BuiltinType>(t);
    return (other) ? sTypePromotionTable[_builtinType][other->_builtinType] : false;
}

const char*
BuiltinType::typeString() const
{
    static const char* s_static_strings[] =
    {
        "void",
        "bool",
        "int32",
        "int64",
        "uint32",
        "uint64",
        "float32",
        "float64",
        "string (constant)"
    };

    static_assert(YAL_ARRAY_SIZE(s_static_strings) == kTotal,
                  "Missing builtin type from array initialization");

    return (_builtinType < kTotal) ? s_static_strings[_builtinType] : "Unknown";
}

}

