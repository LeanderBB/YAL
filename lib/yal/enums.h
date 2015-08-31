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

