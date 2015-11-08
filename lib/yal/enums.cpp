#include "yal/enums.h"
#include "yal/yal.h"

namespace yal
{

const char*
OperatorTypeToStr(const enum OperatorType val)
{
    switch(val)
    {
    case kOperatorTypeCopy:
        return "=";
    case kOperatorTypePlus:
        return "+";
    case kOperatorTypeMinus:
        return "-";
    case kOperatorTypeMult:
    return "*";
    case kOperatorTypeDiv:
        return "/";
    case kOperatorTypeAnd:
        return "and";
    case kOperatorTypeOr:
        return "or";
    case kOperatorTypeNot:
        return "not";
    case kOperatorTypeBitAnd:
        return "&";
    case kOperatorTypeBitOr:
        return "|";
    case kOperatorTypeBitXor:
        return "^";
    case kOperatorTypeBitNot:
        return "~";
    case kOperatorTypeBitShiftLeft:
        return "<<";
    case kOperatorTypeBitShiftRight:
        return ">>";
    case kOperatorTypeGt:
        return ">";
    case kOperatorTypeGe:
        return ">=";
    case kOperatorTypeLe:
        return "<=";
    case kOperatorTypeLt:
        return "<";
    case kOperatorTypeEq:
        return "==";
    case kOperatorTypeNe:
        return "!=";
    case kOperatorTypeArray:
        return "[]";
    case kOperatorTypeMod:
        return "mod";
    default:
        return "Uknown";
    }
}

bool
OperatorRequiresInteger(const enum OperatorType val)
{
    switch(val)
    {
    case kOperatorTypeBitAnd:
    case kOperatorTypeBitOr:
    case kOperatorTypeBitXor:
    case kOperatorTypeBitNot:
    case kOperatorTypeBitShiftLeft:
    case kOperatorTypeBitShiftRight:
    case kOperatorTypeMod:
        return true;
    default:
        return false;
    }
}

bool
OperatorRequiresSignedType(const enum OperatorType val)
{
    switch(val)
    {
    case kOperatorTypeMinus:
        return true;
    default:
        return false;
    }
}

const char*
ConstantTypeToStr(const enum ConstantType val)
{
    switch (val)
    {
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
}
