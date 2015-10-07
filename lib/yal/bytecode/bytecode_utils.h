#ifndef __YAL_BYTECODE_UTILS_H__
#define __YAL_BYTECODE_UTILS_H__

#include "yal/yal.h"
#include "yal/enums.h"
#include <yalvm/yalvm_bytecode.h>
#include "yal/symbols/symbol.h"

namespace yal
{


yalvm_bytecode_inst_t
GlobalLoadForType(const Type* type);

yalvm_bytecode_inst_t
SingeOperatorByteCodeInst(const OperatorType operatorType,
                          const Type* dataType);

yalvm_bytecode_inst_t
DualOperatorByteCodeInst(const OperatorType operatorType,
                         const Type* dataType);

yalvm_bytecode_inst_t
CompareOperatorByteCodeInst(const OperatorType operatorType,
                            const Type* dataType);


yalvm_bytecode_inst_t
AssignOperatorByteCodeInst(const OperatorType operatorType,
                           const Type* dataType);

yalvm_bytecode_inst_t
LoadGlobalByteCodeInst(const Type* dataType);

yalvm_bytecode_inst_t
StoreGlobalByteCodeInst(const Type* dataType);

yalvm_bytecode_inst_t
PrintByteCodeInst(const Type* dataType);

}

#endif
