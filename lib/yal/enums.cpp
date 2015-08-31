#include "yal/enums.h"

namespace yal
{

const char*
AssignOperatorTypeToSt(const enum AssignOperatorType val)
{
    switch (val)
    {
    case kAssignOperatorTypeSet:
        return "AssignOperatorTypeSe";
    case kAssignOperatorTypePlus:
        return "AssignOperatorTypePlus";
    case kAssignOperatorTypeMinus:
        return "AssignOperatorTypeMinus";
    case kAssignOperatorTypeMult:
        return "AssignOperatorTypeMult";
    case kAssignOperatorTypeDiv:
        return "AssignOperatorTypeDiv";
    case kAssignOperatorTypeAnd:
        return "AssignOperatorTypeAnd";
    case kAssignOperatorTypeOr:
        return "kAssignOperatorTypeOr";
    case kAssignOperatorTypeXor:
        return "AssignOperatorTypeXor";
    case kAssignOperatorTypeShiftLeft:
        return "AssignOperatorTypeShiftLeft";
    case kAssignOperatorTypeShiftRight:
        return "AssignOperatorTypeShiftRight";
    default:
        return "Uknown Assingn Operator Type";
    }
}

const char*
ConstantTypeToStr(const enum ConstantType val)
{
    switch (val)
    {
    case kConstantTypeId:
        return "ConstantTypeId";
    case kConstantTypeText:
        return "ConstantTypeText";
    case kConstantTypeUInt32:
        return "ConstantTypeUInt32";
    case kConstantTypeUInt64:
        return "ConstantTypUInt64";
    case kConstantTypeInt64:
        return "ConstantTypeInt64";
    case kConstantTypeInt32:
        return "ConstantTypeInt32";
    case kConstantTypeBool:
        return "ConstantTypeBool";
    case kConstantTypeFloat32:
        return "ConstantTypeFloat32";
    case kConstantTypeFloat64:
        return "ConstantTypeFloat64";
    default:
        return "Uknown Contant Type";
    }
}

const char*
SingleOperatorTypeToStr(const enum SingleOperatorType val)
{
    switch (val)
    {
    case kSingleOperatorTypeBlock:
        return "SingleOperatorTypeBlock";
    case kSingleOperatorTypeNeg:
        return "SingleOperatorTypeNeg";
    case kSingleOperatorTypeNot:
        return "SingleOperatorTypeNot";
    case kSingleOperatorTypeTil:
        return "SingleOperatorTypeTil";
    default:
        return "Uknown Single Operator Type";
    }
}

const char*
DualOperatorTypeToStr(const enum DualOperatorType val)
{
    switch (val)
    {
    case kDualOperatorTypeAdd:
        return "DualOperatorTypeAdd";
    case kDualOperatorTypeSub:
        return "DualOperatorTypeSub";
    case kDualOperatorTypeMult:
        return "DualOperatorTypeMult";
    case kDualOperatorTypeDiv:
        return "DualOperatorTypeDiv";
    case kDualOperatorTypeAnd:
        return "DualOperatorTypeAnd";
    case kDualOperatorTypeOr:
        return "DualOperatorTypeOr";
    case kDualOperatorTypeBitAnd:
        return "DualOperatorTypeBitAnd";
    case kDualOperatorTypeBitOr:
        return "DualOperatorTypeBitOr";
    case kDualOperatorTypeBitXor:
        return "DualOperatorTypeBitXor";
    case kDualOperatorTypeShiftLeft:
        return "DualOperatorTypeShiftLeft";
    case kDualOperatorTypeShiftRight:
        return "DualOperatorTypeShiftRight";
    default:
        return "Uknown Dual Operator Type";
    }
}

const char*
CompareOperatorToStr(const enum CompareOperatorType val)
{
    switch (val)
    {
    case kCompareOperatorTypeGT:
        return "CompareOperatorTypeGT";
    case kCompareOperatorTypeGE:
        return "CompareOperatorTypeGE";
    case kCompareOperatorTypeLE:
        return "CompareOperatorTypeLE";
    case kCompareOperatorTypeLT:
        return "CompareOperatorTypeLT";
    case kCompareOperatorTypeEQ:
        return "CompareOperatorTypeEQ";
    case kCompareOperatorTypeNE:
        return "CompareOperatorTypeNE";
    default:
        return "Uknown Compare Operator Type";
    }
}

bool
ConstantTypeIsNumber(const ConstantType type)
{
    switch(type)
    {
    case kConstantTypeInt32:
    case kConstantTypeUInt32:
    case kConstantTypeInt64:
    case kConstantTypeUInt64:
    case kConstantTypeFloat32:
    case kConstantTypeFloat64:
        return true;
    default:
        return false;
    }
}

bool
ConstantTypeIsDecimal(const ConstantType type)
{
    switch(type)
    {
    case kConstantTypeFloat32:
    case kConstantTypeFloat64:
        return true;
    default:
        return false;
    }
}

bool
ConstantTypeIsInteger(const ConstantType type)
{
    switch(type)
    {
    case kConstantTypeInt32:
    case kConstantTypeUInt32:
    case kConstantTypeInt64:
    case kConstantTypeUInt64:
        return true;
    default:
        return false;
    }
}

bool
ConstantTypeIsSigned(const ConstantType type)
{
    switch(type)
    {
    case kConstantTypeInt32:
    case kConstantTypeInt64:
    case kConstantTypeFloat32:
    case kConstantTypeFloat64:
        return true;
    default:
        return false;
    }
}

bool
ConstantTypeIs32Bits(const ConstantType type)
{
    switch(type)
    {
    case kConstantTypeBool:
    case kConstantTypeFloat32:
    case kConstantTypeInt32:
    case kConstantTypeUInt32:
        return true;
    default:
        return false;
    }
}

bool
ConstantTypeIs64Bits(const ConstantType type)
{
    switch(type)
    {
    case kConstantTypeFloat64:
    case kConstantTypeInt64:
    case kConstantTypeUInt64:
    case kConstantTypeText:
        return true;
    default:
        return false;
    }
}

bool
DualOperatorTypeRequiresIntegerArgs(const DualOperatorType op)
{
    switch(op)
    {
    case kDualOperatorTypeAdd:
    case kDualOperatorTypeSub:
    case kDualOperatorTypeMult:
    case kDualOperatorTypeDiv:
        return false;
    default:
        return true;
    }
}

bool
SingleOperatorTypeRequiresIntegerArgs(const SingleOperatorType op)
{
    return op == kSingleOperatorTypeTil;
}

}
