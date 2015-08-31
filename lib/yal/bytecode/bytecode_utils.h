#ifndef __YAL_BYTECODE_UTILS_H__
#define __YAL_BYTECODE_UTILS_H__

#include "yal/yal.h"
#include "yal/enums.h"
#include <yalvm/yalvm_bytecode.h>
#include "yal/symbols/symbol.h"

namespace yal
{


yalvm_bytecode_inst_t
GlobalLoadForType(const ConstantType type);

yalvm_bytecode_inst_t
SingeOperatorByteCodeInst(const SingleOperatorType operatorType,
                          const ConstantType dataType);

yalvm_bytecode_inst_t
DualOperatorByteCodeInst(const DualOperatorType operatorType,
                         const ConstantType dataType);

yalvm_bytecode_inst_t
CompareOperatorByteCodeInst(const CompareOperatorType operatorType,
                            const ConstantType dataType);


yalvm_bytecode_inst_t
AssignOperatorByteCodeInst(const AssignOperatorType operatorType,
                           const ConstantType dataType);

yalvm_bytecode_inst_t
LoadGlobalByteCodeInst(const DataType dataType);

yalvm_bytecode_inst_t
StoreGlobalByteCodeInst(const DataType dataType);

yalvm_bytecode_inst_t
PrintByteCodeInst(const DataType dataType);

}

#endif
