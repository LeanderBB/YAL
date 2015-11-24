#ifndef __YAL_ARRAYTYPE_H__
#define __YAL_ARRAYTYPE_H__

#include "yal/types/type.h"
#include <string>
namespace yal
{

class Module;
class TypeRegistry;
class Scope;

class ArrayType : public Type
{
public:

    YAL_TYPE_IMPL_HDR(ArrayType)

    static void RegisterBuiltinFunctions(Module& module,
                                         TypeRegistry& typeReg,
                                         Scope& globalScope);

    static std::string GenTypeString(const Type* valueType);

    static std::string GenTypeStringNative(const Type* valueType);

    ArrayType(const TypeId_t &id,
              const Type* valueType);

    virtual ~ArrayType(){}

    virtual bool isPromotableTo(const Type *t) const override;

    virtual const char* typeString() const override
    {
        return _arrayTypeString.c_str();
    }

    const Type* valueType() const
    {
        return _valueType;
    }

    virtual const char* nativeTypeString() const override
    {
        return  _nativeArrayTypeString.c_str();
    }

    virtual const char* builtinFunctionSymName(const char* name) const override;

private:
    const std::string _arrayTypeString;
    const std::string _nativeArrayTypeString;
    const Type* _valueType;
};

}
#endif
