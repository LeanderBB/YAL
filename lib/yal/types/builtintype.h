#ifndef __YAL_BUILTINTYPE_H__
#define __YAL_BUILTINTYPE_H__

#include "yal/types/type.h"

namespace yal
{

class BuiltinType : public Type
{
public:

    YAL_TYPE_IMPL_HDR(BuiltinType)

    enum
    {
        kVoid,
        kBool,
        kInt32,
        kInt64,
        kUInt32,
        kUint64,
        kFloat32,
        kFloat64,
        kTotal
    };

    static BuiltinType* GetBuiltinType(const yal_u32 type);

    BuiltinType(const VmType vmType,
                const yal_u32 typeFlags,
                const yal_u32 operatorFlags);

    virtual ~BuiltinType(){}
};

}

#endif
