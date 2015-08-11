#ifndef __YAL_BYTECODE_UTILS_H__
#define __YAL_BYTECODE_UTILS_H__

#include "yal/yal.h"
#include "yal/enums.h"
#include <yalvm/yalvm_bytecode.h>

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
LoadGlobalByteCodeInst(const ConstantType dataType);

yalvm_bytecode_inst_t
StoreGlobalByteCodeInst(const ConstantType dataType);

yalvm_bytecode_inst_t
PrintByteCodeInst(const ConstantType dataType);

}

#endif
