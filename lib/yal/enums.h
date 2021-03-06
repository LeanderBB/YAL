#ifndef __YAL_ENUMS_H__
#define __YAL_ENUMS_H__

namespace yal
{

enum ConstantType
{
    kConstantTypeNone = 0,
    kConstantTypeText,
    kConstantTypeInt32,
    kConstantTypeUInt32,
    kConstantTypeInt64,
    kConstantTypeUInt64,
    kConstantTypeBool,
    kConstantTypeFloat32,
    kConstantTypeFloat64,
    kConstantTypeMax
};

enum OperatorType
{
    kOperatorTypeCopy           = 1 << 0,
    kOperatorTypePlus           = 1 << 1,
    kOperatorTypeMinus          = 1 << 2,
    kOperatorTypeMult           = 1 << 3,
    kOperatorTypeDiv            = 1 << 4,
    kOperatorTypeAnd            = 1 << 5,
    kOperatorTypeOr             = 1 << 6,
    kOperatorTypeNot            = 1 << 7,
    kOperatorTypeBitAnd         = 1 << 8,
    kOperatorTypeBitOr          = 1 << 9,
    kOperatorTypeBitXor         = 1 << 10,
    kOperatorTypeBitNot         = 1 << 11,
    kOperatorTypeBitShiftLeft   = 1 << 12,
    kOperatorTypeBitShiftRight  = 1 << 13,
    kOperatorTypeGt             = 1 << 14,
    kOperatorTypeGe             = 1 << 15,
    kOperatorTypeLe             = 1 << 16,
    kOperatorTypeLt             = 1 << 17,
    kOperatorTypeEq             = 1 << 18,
    kOperatorTypeNe             = 1 << 19,
    kOperatorTypeArray          = 1 << 20,
    kOperatorTypeMod            = 1 << 21
};

enum OperatorMask
{
    kOperatorMaskArithemetic = (kOperatorTypePlus | kOperatorTypeMinus
        | kOperatorTypeMult | kOperatorTypeDiv),
    kOperatorMaskBit = (kOperatorTypeBitAnd | kOperatorTypeBitOr | kOperatorTypeBitXor
        | kOperatorTypeBitNot | kOperatorTypeBitShiftLeft | kOperatorTypeBitShiftRight | kOperatorTypeMod),
    kOperatorMaskLogic = (kOperatorTypeAnd | kOperatorTypeOr | kOperatorTypeNot),
    kOperatorMaskComparators = (kOperatorTypeGt | kOperatorTypeGe | kOperatorTypeLt
        | kOperatorTypeLe | kOperatorTypeEq | kOperatorTypeNe),
    kOperatorMaskBoolean = kOperatorMaskLogic,
    kOperatorMaskIntegers = (kOperatorMaskArithemetic | kOperatorMaskBit | kOperatorMaskLogic | kOperatorMaskComparators),
    kOperatorMaskDecimals = (kOperatorMaskArithemetic | kOperatorMaskLogic | kOperatorMaskComparators),
    kOperatorMaskSingle = (kOperatorTypeBitNot | kOperatorTypeNot | kOperatorTypeMinus)
};

enum VmType
{
    kVmTypeInt32,
    kVmTypeUInt32,
    kVmTypeInt64,
    kVmTypeUInt64,
    kVmTypeFloat32,
    kVmTypeFloat64,
    kVmTypeObject,
    kVmTypeArray,
    kVmTypeString,
    kVmTypeStringConstant,
    kVmTypeDictionary,
    kVmTypeFunction,
    kVmTypeTotal
};

enum Attribute
{
    kAttributeNativeImpl = 1 << 0,
    kAttributeExternCallable = 1 << 1
};

const char*
OperatorTypeToStr(const enum OperatorType val);

bool
OperatorRequiresInteger(const enum OperatorType val);

bool
OperatorRequiresSignedType(const enum OperatorType val);

const char*
ConstantTypeToStr(const ConstantType type);

bool
ConstantTypeIsNumber(const ConstantType type);

bool
ConstantTypeIsDecimal(const ConstantType type);

bool
ConstantTypeIsInteger(const ConstantType type);

bool
ConstantTypeIsSigned(const ConstantType type);

bool
ConstantTypeIs32Bits(const ConstantType type);

bool
ConstantTypeIs64Bits(const ConstantType type);

}
#endif

