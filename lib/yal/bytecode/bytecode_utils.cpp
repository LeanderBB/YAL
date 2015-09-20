#include "yal/bytecode/bytecode_utils.h"
#include "yal/types/type.h"
namespace yal
{

yalvm_bytecode_inst_t
GlobalLoadForType(const Type *type)
{
    if (type->isPointerSized())
    {
        YAL_ASSERT(false && "Missing VM instructions");
        return YALVM_BYTECODE_TOTAL;
    }
    else if (type->is32BitSized())
    {
        return YALVM_BYTECODE_LOAD_GLOBAL_32;
    }
    else if(type->is64BitSized())
    {
        return YALVM_BYTECODE_LOAD_GLOBAL_64;
    }

    return YALVM_BYTECODE_TOTAL;
}

yalvm_bytecode_inst_t
SingeOperatorByteCodeInst(const OperatorType operatorType,
                          const Type* dataType)
{
    switch (operatorType)
    {
    case kOperatorTypeMinus:
    {
        switch(dataType->vmType())
        {
        case kVmTypeInt32:
            return YALVM_BYTECODE_NEG_I;
        case kVmTypeFloat32:
            return YALVM_BYTECODE_NEG_F;
        case kVmTypeInt64:
            return YALVM_BYTECODE_NEG_IL;
        case kVmTypeFloat64:
            return YALVM_BYTECODE_NEG_FL;
        default:
            return YALVM_BYTECODE_TOTAL;
        }
    }
    case kOperatorTypeNot:
    {
        return YALVM_BYTECODE_NOT;
    }
    case kOperatorTypeBitNot:
    {
        switch(dataType->vmType())
        {
        case kVmTypeInt32:
        case kVmTypeUInt32:
            return YALVM_BYTECODE_BIT_NOT_32;
        case kVmTypeInt64:
        case kVmTypeUInt64:
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
DualOperatorByteCodeInst(const OperatorType operatorType,
                         const Type* type)
{
    const VmType dataType = type->vmType();
    switch (operatorType)
    {
    case kOperatorTypePlus:
    {
        switch(dataType)
        {
        case kVmTypeInt32:
            return YALVM_BYTECODE_ADD_I;
        case kVmTypeUInt32:
            return YALVM_BYTECODE_ADD_U;
        case kVmTypeFloat32:
            return YALVM_BYTECODE_ADD_F;
        case kVmTypeInt64:
            return YALVM_BYTECODE_ADD_IL;
        case kVmTypeUInt64:
            return YALVM_BYTECODE_ADD_UL;
        case kVmTypeFloat64:
            return YALVM_BYTECODE_ADD_FL;
        default:
            return YALVM_BYTECODE_TOTAL;
        }
    }
    case kOperatorTypeMinus:
    {
        switch(dataType)
        {
        case kVmTypeInt32:
            return YALVM_BYTECODE_SUB_I;
        case kVmTypeUInt32:
            return YALVM_BYTECODE_SUB_U;
        case kVmTypeFloat32:
            return YALVM_BYTECODE_SUB_F;
        case kVmTypeInt64:
            return YALVM_BYTECODE_SUB_IL;
        case kVmTypeUInt64:
            return YALVM_BYTECODE_SUB_UL;
        case kVmTypeFloat64:
            return YALVM_BYTECODE_SUB_FL;
        default:
            return YALVM_BYTECODE_TOTAL;
        }
    }
    case kOperatorTypeMult:
    {
        switch(dataType)
        {
        case kVmTypeInt32:
            return YALVM_BYTECODE_MUL_I;
        case kVmTypeUInt32:
            return YALVM_BYTECODE_MUL_U;
        case kVmTypeFloat32:
            return YALVM_BYTECODE_MUL_F;
        case kVmTypeInt64:
            return YALVM_BYTECODE_MUL_IL;
        case kVmTypeUInt64:
            return YALVM_BYTECODE_MUL_UL;
        case kVmTypeFloat64:
            return YALVM_BYTECODE_MUL_FL;
        default:
            return YALVM_BYTECODE_TOTAL;
        }
    }
    case kOperatorTypeDiv:
    {
        switch(dataType)
        {
        case kVmTypeInt32:
            return YALVM_BYTECODE_DIV_I;
        case kVmTypeUInt32:
            return YALVM_BYTECODE_DIV_U;
        case kVmTypeFloat32:
            return YALVM_BYTECODE_DIV_F;
        case kVmTypeInt64:
            return YALVM_BYTECODE_DIV_IL;
        case kVmTypeUInt64:
            return YALVM_BYTECODE_DIV_UL;
        case kVmTypeFloat64:
            return YALVM_BYTECODE_DIV_FL;
        default:
            return YALVM_BYTECODE_TOTAL;
        }
    }
    case kOperatorTypeAnd:
    {
        return YALVM_BYTECODE_AND;
    }
    case kOperatorTypeOr:
    {
        return YALVM_BYTECODE_OR;
    }
    case kOperatorTypeBitAnd:
    {
        switch(dataType)
        {
        case kVmTypeInt32:
        case kVmTypeUInt32:
            return YALVM_BYTECODE_BIT_AND_32;
        case kVmTypeInt64:
        case kVmTypeUInt64:
            return YALVM_BYTECODE_BIT_AND_64;
        default:
            return YALVM_BYTECODE_TOTAL;
        }
    }
    case kOperatorTypeBitOr:
    {
        switch(dataType)
        {
        case kVmTypeInt32:
        case kVmTypeUInt32:
            return YALVM_BYTECODE_BIT_OR_32;
        case kVmTypeInt64:
        case kVmTypeUInt64:
            return YALVM_BYTECODE_BIT_OR_64;
        default:
            return YALVM_BYTECODE_TOTAL;
        }
    }
    case kOperatorTypeBitXor:
    {
        switch(dataType)
        {
        case kVmTypeInt32:
        case kVmTypeUInt32:
            return YALVM_BYTECODE_BIT_XOR_32;
        case kVmTypeInt64:
        case kVmTypeUInt64:
            return YALVM_BYTECODE_BIT_XOR_64;
        default:
            return YALVM_BYTECODE_TOTAL;
        }
    }
    case kOperatorTypeBitShiftLeft:
    {
        switch(dataType)
        {
        case kVmTypeInt32:
        case kVmTypeUInt32:
            return YALVM_BYTECODE_SHIFTL_32;
        case kVmTypeInt64:
        case kVmTypeUInt64:
            return YALVM_BYTECODE_SHIFTL_64;
        default:
            return YALVM_BYTECODE_TOTAL;
        }
    }
    case kOperatorTypeBitShiftRight:
    {
        switch(dataType)
        {
        case kVmTypeInt32:
        case kVmTypeUInt32:
            return YALVM_BYTECODE_SHIFTR_32;
        case kVmTypeInt64:
        case kVmTypeUInt64:
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
CompareOperatorByteCodeInst(const OperatorType operatorType,
                            const Type* type)
{
    const VmType dataType = type->vmType();

    switch (operatorType)
    {
    case kOperatorTypeGt:
    {
        switch(dataType)
        {
        case kVmTypeInt32:
            return YALVM_BYTECODE_COMPARE_GT_I;
        case kVmTypeUInt32:
            return YALVM_BYTECODE_COMPARE_GT_U;
        case kVmTypeFloat32:
            return YALVM_BYTECODE_COMPARE_GT_F;
        case kVmTypeInt64:
            return YALVM_BYTECODE_COMPARE_GT_IL;
        case kVmTypeUInt64:
            return YALVM_BYTECODE_COMPARE_GT_UL;
        case kVmTypeFloat64:
            return YALVM_BYTECODE_COMPARE_GT_FL;
        default:
            return YALVM_BYTECODE_TOTAL;
        }
    }
    case kOperatorTypeGe:
    {
        switch(dataType)
        {
        case kVmTypeInt32:
            return YALVM_BYTECODE_COMPARE_GE_I;
        case kVmTypeUInt32:
            return YALVM_BYTECODE_COMPARE_GE_U;
        case kVmTypeFloat32:
            return YALVM_BYTECODE_COMPARE_GE_F;
        case kVmTypeInt64:
            return YALVM_BYTECODE_COMPARE_GE_IL;
        case kVmTypeUInt64:
            return YALVM_BYTECODE_COMPARE_GE_UL;
        case kVmTypeFloat64:
            return YALVM_BYTECODE_COMPARE_GE_FL;
        default:
            return YALVM_BYTECODE_TOTAL;
        }
    }
    case kOperatorTypeLe:
    {
        switch(dataType)
        {
        case kVmTypeInt32:
            return YALVM_BYTECODE_COMPARE_LE_I;
        case kVmTypeUInt32:
            return YALVM_BYTECODE_COMPARE_LE_U;
        case kVmTypeFloat32:
            return YALVM_BYTECODE_COMPARE_LE_F;
        case kVmTypeInt64:
            return YALVM_BYTECODE_COMPARE_LE_IL;
        case kVmTypeUInt64:
            return YALVM_BYTECODE_COMPARE_LE_UL;
        case kVmTypeFloat64:
            return YALVM_BYTECODE_COMPARE_LE_FL;
        default:
            return YALVM_BYTECODE_TOTAL;
        }
    }
    case kOperatorTypeLt:
    {
        switch(dataType)
        {
        case kVmTypeInt32:
            return YALVM_BYTECODE_COMPARE_LT_I;
        case kVmTypeUInt32:
            return YALVM_BYTECODE_COMPARE_LT_U;
        case kVmTypeFloat32:
            return YALVM_BYTECODE_COMPARE_LT_F;
        case kVmTypeInt64:
            return YALVM_BYTECODE_COMPARE_LT_IL;
        case kVmTypeUInt64:
            return YALVM_BYTECODE_COMPARE_LT_UL;
        case kVmTypeFloat64:
            return YALVM_BYTECODE_COMPARE_LT_FL;
        default:
            return YALVM_BYTECODE_TOTAL;
        }
    }
    case kOperatorTypeEq:
    {
        switch(dataType)
        {
        case kVmTypeInt32:
            return YALVM_BYTECODE_COMPARE_EQ_I;
        case kVmTypeUInt32:
            return YALVM_BYTECODE_COMPARE_EQ_U;
        case kVmTypeFloat32:
            return YALVM_BYTECODE_COMPARE_EQ_F;
        case kVmTypeInt64:
            return YALVM_BYTECODE_COMPARE_EQ_IL;
        case kVmTypeUInt64:
            return YALVM_BYTECODE_COMPARE_EQ_UL;
        case kVmTypeFloat64:
            return YALVM_BYTECODE_COMPARE_EQ_FL;
        default:
            return YALVM_BYTECODE_TOTAL;
        }
    }
    case kOperatorTypeNe:
    {
        switch(dataType)
        {
        case kVmTypeInt32:
            return YALVM_BYTECODE_COMPARE_NE_I;
        case kVmTypeUInt32:
            return YALVM_BYTECODE_COMPARE_NE_U;
        case kVmTypeFloat32:
            return YALVM_BYTECODE_COMPARE_NE_F;
        case kVmTypeInt64:
            return YALVM_BYTECODE_COMPARE_NE_IL;
        case kVmTypeUInt64:
            return YALVM_BYTECODE_COMPARE_NE_UL;
        case kVmTypeFloat64:
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

yalvm_bytecode_inst_t
AssignOperatorByteCodeInst(const OperatorType operatorType,
                           const Type* type)
{
    const VmType dataType = type->vmType();
    switch (operatorType)
    {
    case kOperatorTypeCopy:
    {
        return YALVM_BYTECODE_COPY_REGISTER;
    }
    case kOperatorTypePlus:
    {
        switch(dataType)
        {
        case kVmTypeInt32:
            return YALVM_BYTECODE_ADD_I;
        case kVmTypeUInt32:
            return YALVM_BYTECODE_ADD_U;
        case kVmTypeFloat32:
            return YALVM_BYTECODE_ADD_F;
        case kVmTypeInt64:
            return YALVM_BYTECODE_ADD_IL;
        case kVmTypeUInt64:
            return YALVM_BYTECODE_ADD_UL;
        case kVmTypeFloat64:
            return YALVM_BYTECODE_ADD_FL;
        default:
            return YALVM_BYTECODE_TOTAL;
        }
    }
    case kOperatorTypeMinus:
    {
        switch(dataType)
        {
        case kVmTypeInt32:
            return YALVM_BYTECODE_SUB_I;
        case kVmTypeUInt32:
            return YALVM_BYTECODE_SUB_U;
        case kVmTypeFloat32:
            return YALVM_BYTECODE_SUB_F;
        case kVmTypeInt64:
            return YALVM_BYTECODE_SUB_IL;
        case kVmTypeUInt64:
            return YALVM_BYTECODE_SUB_UL;
        case kVmTypeFloat64:
            return YALVM_BYTECODE_SUB_FL;
        default:
            return YALVM_BYTECODE_TOTAL;
        }
    }
    case kOperatorTypeMult:
    {
        switch(dataType)
        {
        case kVmTypeInt32:
            return YALVM_BYTECODE_MUL_I;
        case kVmTypeUInt32:
            return YALVM_BYTECODE_MUL_U;
        case kVmTypeFloat32:
            return YALVM_BYTECODE_MUL_F;
        case kVmTypeInt64:
            return YALVM_BYTECODE_MUL_IL;
        case kVmTypeUInt64:
            return YALVM_BYTECODE_MUL_UL;
        case kVmTypeFloat64:
            return YALVM_BYTECODE_MUL_FL;
        default:
            return YALVM_BYTECODE_TOTAL;
        }
    }
    case kOperatorTypeDiv:
    {
        switch(dataType)
        {
        case kVmTypeInt32:
            return YALVM_BYTECODE_DIV_I;
        case kVmTypeUInt32:
            return YALVM_BYTECODE_DIV_U;
        case kVmTypeFloat32:
            return YALVM_BYTECODE_DIV_F;
        case kVmTypeInt64:
            return YALVM_BYTECODE_DIV_IL;
        case kVmTypeUInt64:
            return YALVM_BYTECODE_DIV_UL;
        case kVmTypeFloat64:
            return YALVM_BYTECODE_DIV_FL;
        default:
            return YALVM_BYTECODE_TOTAL;
        }
    }
    case kOperatorTypeBitAnd:
    {
        switch(dataType)
        {
        case kVmTypeInt32:
        case kVmTypeUInt32:
            return YALVM_BYTECODE_BIT_AND_32;
        case kVmTypeInt64:
        case kVmTypeUInt64:
            return YALVM_BYTECODE_BIT_AND_64;
        default:
            return YALVM_BYTECODE_TOTAL;
        }
    }
    case kOperatorTypeBitOr:
    {
        switch(dataType)
        {
        case kVmTypeInt32:
        case kVmTypeUInt32:
            return YALVM_BYTECODE_BIT_OR_32;
        case kVmTypeInt64:
        case kVmTypeUInt64:
            return YALVM_BYTECODE_BIT_OR_64;
        default:
            return YALVM_BYTECODE_TOTAL;
        }
    }
    case kOperatorTypeBitXor:
    {
        switch(dataType)
        {
        case kVmTypeInt32:
        case kVmTypeUInt32:
            return YALVM_BYTECODE_BIT_XOR_32;
        case kVmTypeInt64:
        case kVmTypeUInt64:
            return YALVM_BYTECODE_BIT_XOR_64;
        default:
            return YALVM_BYTECODE_TOTAL;
        }
    }
    case kOperatorTypeBitShiftLeft:
    {
        switch(dataType)
        {
        case kVmTypeInt32:
        case kVmTypeUInt32:
            return YALVM_BYTECODE_SHIFTL_32;
        case kVmTypeInt64:
        case kVmTypeUInt64:
            return YALVM_BYTECODE_SHIFTL_64;
        default:
            return YALVM_BYTECODE_TOTAL;
        }
    }
    case kOperatorTypeBitShiftRight:
    {
        switch(dataType)
        {
        case kVmTypeInt32:
        case kVmTypeUInt32:
            return YALVM_BYTECODE_SHIFTR_32;
        case kVmTypeInt64:
        case kVmTypeUInt64:
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
LoadGlobalByteCodeInst(const Type *dataType)
{
    if (dataType->isBuiltinType())
    {
        if(dataType->is32BitSized())
        {
            return YALVM_BYTECODE_LOAD_GLOBAL_32;
        }
        else if (dataType->is64BitSized())
        {
            return YALVM_BYTECODE_LOAD_GLOBAL_64;
        }
        else
        {
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
StoreGlobalByteCodeInst(const Type *dataType)
{
    if (dataType->isBuiltinType())
    {
        if (dataType->is32BitSized())
        {
            return YALVM_BYTECODE_STORE_GLOBAL_32;
        }
        else if (dataType->is64BitSized())
        {
            return YALVM_BYTECODE_STORE_GLOBAL_64;

        }
        else
        {
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
PrintByteCodeInst(const Type *dataType)
{
    if (dataType->isBuiltinType())
    {
        switch(dataType->vmType())
        {
        case kVmTypeInt32:
            return YALVM_BYTECODE_PRINT_I;
        case kVmTypeUInt32:
            return YALVM_BYTECODE_PRINT_U;
        case kVmTypeFloat32:
            return YALVM_BYTECODE_PRINT_F;
        case kVmTypeUInt64:
            return YALVM_BYTECODE_PRINT_UL;
        case kVmTypeInt64:
            return YALVM_BYTECODE_PRINT_IL;
        case kVmTypeFloat64:
            return YALVM_BYTECODE_PRINT_FL;

 /* FIXME: String support for print instructions?!
        case kVmTypeText:
            return YALVM_BYTECODE_PRINT_STR;*/
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
