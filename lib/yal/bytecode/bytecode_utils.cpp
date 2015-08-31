#include "yal/bytecode/bytecode_utils.h"

namespace yal
{

yalvm_bytecode_inst_t
GlobalLoadForType(const ConstantType type)
{
    switch(type)
    {
    case kConstantTypeInt32:
    case kConstantTypeUInt32:
    case kConstantTypeFloat32:
        return YALVM_BYTECODE_LOAD_GLOBAL_32;
    case kConstantTypeInt64:
    case kConstantTypeUInt64:
    case kConstantTypeFloat64:
        return YALVM_BYTECODE_LOAD_GLOBAL_64;
    default:
        return YALVM_BYTECODE_TOTAL;
    }
}

yalvm_bytecode_inst_t
SingeOperatorByteCodeInst(const SingleOperatorType operatorType,
                          const ConstantType dataType)
{
    switch (operatorType)
    {
    case kSingleOperatorTypeNeg:
    {
        switch(dataType)
        {
        case kConstantTypeInt32:
            return YALVM_BYTECODE_NEG_I;
        case kConstantTypeFloat32:
            return YALVM_BYTECODE_NEG_F;
        case kConstantTypeInt64:
            return YALVM_BYTECODE_NEG_IL;
        case kConstantTypeFloat64:
            return YALVM_BYTECODE_NEG_FL;
        default:
            return YALVM_BYTECODE_TOTAL;
        }
    }
    case kSingleOperatorTypeNot:
    {
        return YALVM_BYTECODE_NOT;
    }
    case kSingleOperatorTypeTil:
    {
        switch(dataType)
        {
        case kConstantTypeInt32:
        case kConstantTypeUInt32:
            return YALVM_BYTECODE_BIT_NOT_32;
        case kConstantTypeInt64:
        case kConstantTypeUInt64:
            return YALVM_BYTECODE_BIT_NOT_64;
        default:
            return YALVM_BYTECODE_TOTAL;
        }
    }
    default:
        YAL_ASSERT(false && "Should not be reached");
        return YALVM_BYTECODE_TOTAL;
    }
}

yalvm_bytecode_inst_t
DualOperatorByteCodeInst(const DualOperatorType operatorType,
                         const ConstantType dataType)
{
    switch (operatorType)
    {
    case kDualOperatorTypeAdd:
    {
        switch(dataType)
        {
        case kConstantTypeInt32:
            return YALVM_BYTECODE_ADD_I;
        case kConstantTypeUInt32:
            return YALVM_BYTECODE_ADD_U;
        case kConstantTypeFloat32:
            return YALVM_BYTECODE_ADD_F;
        case kConstantTypeInt64:
            return YALVM_BYTECODE_ADD_IL;
        case kConstantTypeUInt64:
            return YALVM_BYTECODE_ADD_UL;
        case kConstantTypeFloat64:
            return YALVM_BYTECODE_ADD_FL;
        default:
            return YALVM_BYTECODE_TOTAL;
        }
    }
    case kDualOperatorTypeSub:
    {
        switch(dataType)
        {
        case kConstantTypeInt32:
            return YALVM_BYTECODE_SUB_I;
        case kConstantTypeUInt32:
            return YALVM_BYTECODE_SUB_U;
        case kConstantTypeFloat32:
            return YALVM_BYTECODE_SUB_F;
        case kConstantTypeInt64:
            return YALVM_BYTECODE_SUB_IL;
        case kConstantTypeUInt64:
            return YALVM_BYTECODE_SUB_UL;
        case kConstantTypeFloat64:
            return YALVM_BYTECODE_SUB_FL;
        default:
            return YALVM_BYTECODE_TOTAL;
        }
    }
    case kDualOperatorTypeMult:
    {
        switch(dataType)
        {
        case kConstantTypeInt32:
            return YALVM_BYTECODE_MUL_I;
        case kConstantTypeUInt32:
            return YALVM_BYTECODE_MUL_U;
        case kConstantTypeFloat32:
            return YALVM_BYTECODE_MUL_F;
        case kConstantTypeInt64:
            return YALVM_BYTECODE_MUL_IL;
        case kConstantTypeUInt64:
            return YALVM_BYTECODE_MUL_UL;
        case kConstantTypeFloat64:
            return YALVM_BYTECODE_MUL_FL;
        default:
            return YALVM_BYTECODE_TOTAL;
        }
    }
    case kDualOperatorTypeDiv:
    {
        switch(dataType)
        {
        case kConstantTypeInt32:
            return YALVM_BYTECODE_DIV_I;
        case kConstantTypeUInt32:
            return YALVM_BYTECODE_DIV_U;
        case kConstantTypeFloat32:
            return YALVM_BYTECODE_DIV_F;
        case kConstantTypeInt64:
            return YALVM_BYTECODE_DIV_IL;
        case kConstantTypeUInt64:
            return YALVM_BYTECODE_DIV_UL;
        case kConstantTypeFloat64:
            return YALVM_BYTECODE_DIV_FL;
        default:
            return YALVM_BYTECODE_TOTAL;
        }
    }
    case kDualOperatorTypeAnd:
    {
        return YALVM_BYTECODE_AND;
    }
    case kDualOperatorTypeOr:
    {
        return YALVM_BYTECODE_OR;
    }
    case kDualOperatorTypeBitAnd:
    {
        switch(dataType)
        {
        case kConstantTypeInt32:
        case kConstantTypeUInt32:
            return YALVM_BYTECODE_BIT_AND_32;
        case kConstantTypeInt64:
        case kConstantTypeUInt64:
            return YALVM_BYTECODE_BIT_AND_64;
        default:
            return YALVM_BYTECODE_TOTAL;
        }
    }
    case kDualOperatorTypeBitOr:
    {
        switch(dataType)
        {
        case kConstantTypeInt32:
        case kConstantTypeUInt32:
            return YALVM_BYTECODE_BIT_OR_32;
        case kConstantTypeInt64:
        case kConstantTypeUInt64:
            return YALVM_BYTECODE_BIT_OR_64;
        default:
            return YALVM_BYTECODE_TOTAL;
        }
    }
    case kDualOperatorTypeBitXor:
    {
        switch(dataType)
        {
        case kConstantTypeInt32:
        case kConstantTypeUInt32:
            return YALVM_BYTECODE_BIT_XOR_32;
        case kConstantTypeInt64:
        case kConstantTypeUInt64:
            return YALVM_BYTECODE_BIT_XOR_64;
        default:
            return YALVM_BYTECODE_TOTAL;
        }
    }
    case kDualOperatorTypeShiftLeft:
    {
        switch(dataType)
        {
        case kConstantTypeInt32:
        case kConstantTypeUInt32:
            return YALVM_BYTECODE_SHIFTL_32;
        case kConstantTypeInt64:
        case kConstantTypeUInt64:
            return YALVM_BYTECODE_SHIFTL_64;
        default:
            return YALVM_BYTECODE_TOTAL;
        }
    }
    case kDualOperatorTypeShiftRight:
    {
        switch(dataType)
        {
        case kConstantTypeInt32:
        case kConstantTypeUInt32:
            return YALVM_BYTECODE_SHIFTR_32;
        case kConstantTypeInt64:
        case kConstantTypeUInt64:
            return YALVM_BYTECODE_SHIFTR_64;
        default:
            return YALVM_BYTECODE_TOTAL;
        }
    }

    default:
        YAL_ASSERT(false && "Should not be reached");
        return YALVM_BYTECODE_TOTAL;
    }
}

yalvm_bytecode_inst_t
CompareOperatorByteCodeInst(const CompareOperatorType operatorType,
                            const ConstantType dataType)
{
    {
        switch (operatorType)
        {
        case kCompareOperatorTypeGT:
        {
            switch(dataType)
            {
            case kConstantTypeInt32:
                return YALVM_BYTECODE_COMPARE_GT_I;
            case kConstantTypeUInt32:
                return YALVM_BYTECODE_COMPARE_GT_U;
            case kConstantTypeFloat32:
                return YALVM_BYTECODE_COMPARE_GT_F;
            case kConstantTypeInt64:
                return YALVM_BYTECODE_COMPARE_GT_IL;
            case kConstantTypeUInt64:
                return YALVM_BYTECODE_COMPARE_GT_UL;
            case kConstantTypeFloat64:
                return YALVM_BYTECODE_COMPARE_GT_FL;
            default:
                return YALVM_BYTECODE_TOTAL;
            }
        }
        case kCompareOperatorTypeGE:
        {
            switch(dataType)
            {
            case kConstantTypeInt32:
                return YALVM_BYTECODE_COMPARE_GE_I;
            case kConstantTypeUInt32:
                return YALVM_BYTECODE_COMPARE_GE_U;
            case kConstantTypeFloat32:
                return YALVM_BYTECODE_COMPARE_GE_F;
            case kConstantTypeInt64:
                return YALVM_BYTECODE_COMPARE_GE_IL;
            case kConstantTypeUInt64:
                return YALVM_BYTECODE_COMPARE_GE_UL;
            case kConstantTypeFloat64:
                return YALVM_BYTECODE_COMPARE_GE_FL;
            default:
                return YALVM_BYTECODE_TOTAL;
            }
        }
        case kCompareOperatorTypeLE:
        {
            switch(dataType)
            {
            case kConstantTypeInt32:
                return YALVM_BYTECODE_COMPARE_LE_I;
            case kConstantTypeUInt32:
                return YALVM_BYTECODE_COMPARE_LE_U;
            case kConstantTypeFloat32:
                return YALVM_BYTECODE_COMPARE_LE_F;
            case kConstantTypeInt64:
                return YALVM_BYTECODE_COMPARE_LE_IL;
            case kConstantTypeUInt64:
                return YALVM_BYTECODE_COMPARE_LE_UL;
            case kConstantTypeFloat64:
                return YALVM_BYTECODE_COMPARE_LE_FL;
            default:
                return YALVM_BYTECODE_TOTAL;
            }
        }
        case kCompareOperatorTypeLT:
        {
            switch(dataType)
            {
            case kConstantTypeInt32:
                return YALVM_BYTECODE_COMPARE_LT_I;
            case kConstantTypeUInt32:
                return YALVM_BYTECODE_COMPARE_LT_U;
            case kConstantTypeFloat32:
                return YALVM_BYTECODE_COMPARE_LT_F;
            case kConstantTypeInt64:
                return YALVM_BYTECODE_COMPARE_LT_IL;
            case kConstantTypeUInt64:
                return YALVM_BYTECODE_COMPARE_LT_UL;
            case kConstantTypeFloat64:
                return YALVM_BYTECODE_COMPARE_LT_FL;
            default:
                return YALVM_BYTECODE_TOTAL;
            }
        }
        case kCompareOperatorTypeEQ:
        {
            switch(dataType)
            {
            case kConstantTypeInt32:
                return YALVM_BYTECODE_COMPARE_EQ_I;
            case kConstantTypeUInt32:
                return YALVM_BYTECODE_COMPARE_EQ_U;
            case kConstantTypeFloat32:
                return YALVM_BYTECODE_COMPARE_EQ_F;
            case kConstantTypeInt64:
                return YALVM_BYTECODE_COMPARE_EQ_IL;
            case kConstantTypeUInt64:
                return YALVM_BYTECODE_COMPARE_EQ_UL;
            case kConstantTypeFloat64:
                return YALVM_BYTECODE_COMPARE_EQ_FL;
            default:
                return YALVM_BYTECODE_TOTAL;
            }
        }
        case kCompareOperatorTypeNE:
        {
            switch(dataType)
            {
            case kConstantTypeInt32:
                return YALVM_BYTECODE_COMPARE_NE_I;
            case kConstantTypeUInt32:
                return YALVM_BYTECODE_COMPARE_NE_U;
            case kConstantTypeFloat32:
                return YALVM_BYTECODE_COMPARE_NE_F;
            case kConstantTypeInt64:
                return YALVM_BYTECODE_COMPARE_NE_IL;
            case kConstantTypeUInt64:
                return YALVM_BYTECODE_COMPARE_NE_UL;
            case kConstantTypeFloat64:
                return YALVM_BYTECODE_COMPARE_NE_FL;
            default:
                return YALVM_BYTECODE_TOTAL;
            }
        }
        default:
            YAL_ASSERT(false && "Should not be reached");
            return YALVM_BYTECODE_TOTAL;
        }
    }
}

yalvm_bytecode_inst_t
AssignOperatorByteCodeInst(const AssignOperatorType operatorType,
                           const ConstantType dataType)
{
    {
        switch (operatorType)
        {
        case kAssignOperatorTypeSet:
        {
            return YALVM_BYTECODE_COPY_REGISTER;
        }
        case kAssignOperatorTypePlus:
        {
            switch(dataType)
            {
            case kConstantTypeInt32:
                return YALVM_BYTECODE_ADD_I;
            case kConstantTypeUInt32:
                return YALVM_BYTECODE_ADD_U;
            case kConstantTypeFloat32:
                return YALVM_BYTECODE_ADD_F;
            case kConstantTypeInt64:
                return YALVM_BYTECODE_ADD_IL;
            case kConstantTypeUInt64:
                return YALVM_BYTECODE_ADD_UL;
            case kConstantTypeFloat64:
                return YALVM_BYTECODE_ADD_FL;
            default:
                return YALVM_BYTECODE_TOTAL;
            }
        }
        case kAssignOperatorTypeMinus:
        {
            switch(dataType)
            {
            case kConstantTypeInt32:
                return YALVM_BYTECODE_SUB_I;
            case kConstantTypeUInt32:
                return YALVM_BYTECODE_SUB_U;
            case kConstantTypeFloat32:
                return YALVM_BYTECODE_SUB_F;
            case kConstantTypeInt64:
                return YALVM_BYTECODE_SUB_IL;
            case kConstantTypeUInt64:
                return YALVM_BYTECODE_SUB_UL;
            case kConstantTypeFloat64:
                return YALVM_BYTECODE_SUB_FL;
            default:
                return YALVM_BYTECODE_TOTAL;
            }
        }
        case kAssignOperatorTypeMult:
        {
            switch(dataType)
            {
            case kConstantTypeInt32:
                return YALVM_BYTECODE_MUL_I;
            case kConstantTypeUInt32:
                return YALVM_BYTECODE_MUL_U;
            case kConstantTypeFloat32:
                return YALVM_BYTECODE_MUL_F;
            case kConstantTypeInt64:
                return YALVM_BYTECODE_MUL_IL;
            case kConstantTypeUInt64:
                return YALVM_BYTECODE_MUL_UL;
            case kConstantTypeFloat64:
                return YALVM_BYTECODE_MUL_FL;
            default:
                return YALVM_BYTECODE_TOTAL;
            }
        }
        case kAssignOperatorTypeDiv:
        {
            switch(dataType)
            {
            case kConstantTypeInt32:
                return YALVM_BYTECODE_DIV_I;
            case kConstantTypeUInt32:
                return YALVM_BYTECODE_DIV_U;
            case kConstantTypeFloat32:
                return YALVM_BYTECODE_DIV_F;
            case kConstantTypeInt64:
                return YALVM_BYTECODE_DIV_IL;
            case kConstantTypeUInt64:
                return YALVM_BYTECODE_DIV_UL;
            case kConstantTypeFloat64:
                return YALVM_BYTECODE_DIV_FL;
            default:
                return YALVM_BYTECODE_TOTAL;
            }
        }
        case kAssignOperatorTypeAnd:
        {
            switch(dataType)
            {
            case kConstantTypeInt32:
            case kConstantTypeUInt32:
                return YALVM_BYTECODE_BIT_AND_32;
            case kConstantTypeInt64:
            case kConstantTypeUInt64:
                return YALVM_BYTECODE_BIT_AND_64;
            default:
                return YALVM_BYTECODE_TOTAL;
            }
        }
        case kAssignOperatorTypeOr:
        {
            switch(dataType)
            {
            case kConstantTypeInt32:
            case kConstantTypeUInt32:
                return YALVM_BYTECODE_BIT_OR_32;
            case kConstantTypeInt64:
            case kConstantTypeUInt64:
                return YALVM_BYTECODE_BIT_OR_64;
            default:
                return YALVM_BYTECODE_TOTAL;
            }
        }
        case kAssignOperatorTypeXor:
        {
            switch(dataType)
            {
            case kConstantTypeInt32:
            case kConstantTypeUInt32:
                return YALVM_BYTECODE_BIT_XOR_32;
            case kConstantTypeInt64:
            case kConstantTypeUInt64:
                return YALVM_BYTECODE_BIT_XOR_64;
            default:
                return YALVM_BYTECODE_TOTAL;
            }
        }
        case kAssignOperatorTypeShiftLeft:
        {
            switch(dataType)
            {
            case kConstantTypeInt32:
            case kConstantTypeUInt32:
                return YALVM_BYTECODE_SHIFTL_32;
            case kConstantTypeInt64:
            case kConstantTypeUInt64:
                return YALVM_BYTECODE_SHIFTL_64;
            default:
                return YALVM_BYTECODE_TOTAL;
            }
        }
        case kAssignOperatorTypeShiftRight:
        {
            switch(dataType)
            {
            case kConstantTypeInt32:
            case kConstantTypeUInt32:
                return YALVM_BYTECODE_SHIFTR_32;
            case kConstantTypeInt64:
            case kConstantTypeUInt64:
                return YALVM_BYTECODE_SHIFTR_64;
            default:
                return YALVM_BYTECODE_TOTAL;
            }
        }
        default:
            YAL_ASSERT(false && "Should not be reached");
            return YALVM_BYTECODE_TOTAL;
        }
    }
}

yalvm_bytecode_inst_t
LoadGlobalByteCodeInst(const DataType dataType)
{
    if (dataType.isBuiltinType())
    {
        switch(dataType.data.builtin)
        {
        case kConstantTypeInt32:
        case kConstantTypeUInt32:
        case kConstantTypeFloat32:
        case kConstantTypeBool:
            return YALVM_BYTECODE_LOAD_GLOBAL_32;
        case kConstantTypeUInt64:
        case kConstantTypeInt64:
        case kConstantTypeFloat64:
        case kConstantTypeText:
            return YALVM_BYTECODE_LOAD_GLOBAL_64;
        case kConstantTypeId:
        default:
            YAL_ASSERT(false && "Should not be reached");
            return YALVM_BYTECODE_TOTAL;
        }
    }
    else
    {
        return YALVM_BYTECODE_LOAD_GLOBAL_64;
    }
}

yalvm_bytecode_inst_t
StoreGlobalByteCodeInst(const DataType dataType)
{
    if (dataType.isBuiltinType())
    {
        switch(dataType.data.builtin)
        {
        case kConstantTypeInt32:
        case kConstantTypeUInt32:
        case kConstantTypeFloat32:
        case kConstantTypeBool:
            return YALVM_BYTECODE_STORE_GLOBAL_32;
        case kConstantTypeUInt64:
        case kConstantTypeInt64:
        case kConstantTypeFloat64:
        case kConstantTypeText:
            return YALVM_BYTECODE_STORE_GLOBAL_64;
        case kConstantTypeId:
        default:
            YAL_ASSERT(false && "Should not be reached");
            return YALVM_BYTECODE_TOTAL;
        }
    }
    else
    {
        return YALVM_BYTECODE_STORE_GLOBAL_64;
    }
}

yalvm_bytecode_inst_t
PrintByteCodeInst(const DataType dataType)
{
    if (dataType.isBuiltinType())
    {
        switch(dataType.data.builtin)
        {
        case kConstantTypeInt32:
            return YALVM_BYTECODE_PRINT_I;
        case kConstantTypeBool:
        case kConstantTypeUInt32:
            return YALVM_BYTECODE_PRINT_U;
        case kConstantTypeFloat32:
            return YALVM_BYTECODE_PRINT_F;
        case kConstantTypeUInt64:
            return YALVM_BYTECODE_PRINT_UL;
        case kConstantTypeInt64:
            return YALVM_BYTECODE_PRINT_IL;
        case kConstantTypeFloat64:
            return YALVM_BYTECODE_PRINT_FL;
        case kConstantTypeText:
            return YALVM_BYTECODE_PRINT_STR;
        default:
            YAL_ASSERT(false && "Should not be reached");
            return YALVM_BYTECODE_TOTAL;
        }
    }
    else
    {
        return YALVM_BYTECODE_TOTAL;
    }
}


}
