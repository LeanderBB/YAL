#ifndef __YAL_BUILTINTYPE_H__
#define __YAL_BUILTINTYPE_H__

#include "yal/types/type.h"

namespace yal
{

class BuiltinType : public Type
{
public:

    YAL_TYPE_IMPL_HDR(BuiltinType)

    enum Types
    {
        kVoid,
        kBool,
        kInt32,
        kInt64,
        kUInt32,
        kUInt64,
        kFloat32,
        kFloat64,
        kStringConstant,
        kTotal
    };

    static BuiltinType* GetBuiltinType(const Types type);

    static BuiltinType* GetBuiltinTypeForConstantType(const ConstantType type);

    BuiltinType(const VmType vmType,
                const yal_u32 typeFlags,
                const yal_u32 operatorFlags,
                const Types builtinDataType);

    bool isPromotableTo(const Type* t) const;

    BuiltinType* builtinType() const
    {
        return GetBuiltinType(_builtinType);
    }

    virtual ~BuiltinType(){}

    virtual const char* typeString() const override;

protected:
    const Types _builtinType;
};


}

#endif
