#ifndef __YAL_ENUMS_H__
#define __YAL_ENUMS_H__

namespace yal
{
enum AssignOperatorType
{
    kAssignOperatorTypeSet,
    kAssignOperatorTypePlus,
    kAssignOperatorTypeMinus,
    kAssignOperatorTypeMult,
    kAssignOperatorTypeDiv,
    kAssignOperatorTypeAnd,
    kAssignOperatorTypeOr,
    kAssignOperatorTypeXor,
    kAssignOperatorTypeShiftLeft,
    kAssignOperatorTypeShiftRight
};

enum ConstantType
{
    kConstantTypeNone = 0,
    kConstantTypeId,
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

enum DualOperatorType
{
    kDualOperatorTypeAdd,
    kDualOperatorTypeSub,
    kDualOperatorTypeMult,
    kDualOperatorTypeDiv,
    kDualOperatorTypeAnd,
    kDualOperatorTypeOr,
    kDualOperatorTypeBitAnd,
    kDualOperatorTypeBitOr,
    kDualOperatorTypeBitXor,
    kDualOperatorTypeShiftLeft,
    kDualOperatorTypeShiftRight
};

enum CompareOperatorType
{
    kCompareOperatorTypeGT,
    kCompareOperatorTypeGE,
    kCompareOperatorTypeLE,
    kCompareOperatorTypeLT,
    kCompareOperatorTypeEQ,
    kCompareOperatorTypeNE,
};

enum SingleOperatorType
{
    kSingleOperatorTypeBlock,
    kSingleOperatorTypeNeg,
    kSingleOperatorTypeNot,
    kSingleOperatorTypeTil
};

enum SymbolDataOrigin
{
    kSymbolDataOriginBuiltin,
    kSymbolDataOriginCustom
};

enum Operator
{
    kOperatorCopy           = 1 << 0,
    kOperatorPlus           = 1 << 1,
    kOperatorMinus          = 1 << 2,
    kOperatorMult           = 1 << 3,
    kOperatorDiv            = 1 << 4,
    kOperatorAnd            = 1 << 5,
    kOperatorOr             = 1 << 6,
    kOperatorNot            = 1 << 7,
    kOperatorBitAnd         = 1 << 8,
    kOperatorBitOr          = 1 << 9,
    kOperatorBitXor         = 1 << 10,
    kOperatorBitNot         = 1 << 11,
    kOperatorBitShiftLeft   = 1 << 12,
    kOperatorBitShiftRight  = 1 << 13,
    kOperatorGt             = 1 << 14,
    kOperatorGe             = 1 << 15,
    kOperatorLe             = 1 << 16,
    kOperatorLt             = 1 << 17,
    kOperatorEq             = 1 << 18,
    kOperatorNe             = 1 << 19,
    kOperatorArray          = 1 << 20
};

enum OperatorMask
{
    kOperatorMaskArithemetic = (kOperatorPlus | kOperatorMinus
        | kOperatorMult | kOperatorDiv),
    kOperatorMaskBit = (kOperatorBitAnd | kOperatorBitOr | kOperatorBitXor
        | kOperatorBitNot | kOperatorBitShiftLeft | kOperatorBitShiftRight),
    kOperatorMaskLogic = (kOperatorAnd | kOperatorOr | kOperatorNot),
    kOperatorMaskComparators = (kOperatorGt | kOperatorGe | kOperatorLt
        | kOperatorLe | kOperatorEq | kOperatorNe),
    kOperatorMaskBoolean = kOperatorMaskLogic,
    kOperatorMaskIntegers = (kOperatorMaskArithemetic | kOperatorMaskBit | kOperatorMaskLogic | kOperatorMaskComparators),
    kOperatorMaskDecimals = (kOperatorMaskArithemetic | kOperatorMaskLogic | kOperatorMaskComparators),
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
    kVmTypeDictionary,
    kVmTypeFunction,
    kVmTypeTotal
};


const char*
AssignOperatorTypeToSt(const enum AssignOperatorType val);

const char*
ConstantTypeToStr(const enum ConstantType val);

const char*
SingleOperatorTypeToStr(const enum SingleOperatorType val);

const char*
DualOperatorTypeToStr(const enum DualOperatorType val);

const char*
CompareOperatorToStr(const enum CompareOperatorType val);

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

bool
DualOperatorTypeRequiresIntegerArgs(const DualOperatorType op);

bool
SingleOperatorTypeRequiresIntegerArgs(const SingleOperatorType op);
}
#endif

