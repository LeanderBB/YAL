#ifndef __YAL_FUNCTIONTYPE_H__
#define __YAL_FUNCTIONTYPE_H__

#include "yal/types/type.h"
#include <vector>
namespace yal
{
class FunctionDeclBaseNode;
class FunctionType : public Type
{
public:

    YAL_TYPE_IMPL_HDR(FunctionType)

    FunctionType(const TypeId_t id);

    virtual ~FunctionType() {}

    virtual bool hasReturnValue() const = 0;

    virtual bool hasArguments() const = 0;

    virtual bool isObjectFunction() const = 0;

    virtual yal_u32 argumentCount() const = 0;

    virtual Type* typeOfArgument(const yal_u32 argIdx) const = 0;

    virtual Type* typeOfReturnValue() const = 0;

    virtual Type* typeOfObject() const = 0;

    virtual bool isPromotableTo(const Type *t) const override;

};


class AstFunctionType: public FunctionType
{
public:
    AstFunctionType(const TypeId_t id,
                    const FunctionDeclBaseNode* funcDecl);

    virtual ~AstFunctionType() {}

    bool hasReturnValue() const override final;

    bool hasArguments() const override final;

    bool isObjectFunction() const override final;

    yal_u32 argumentCount() const override final;

    Type* typeOfArgument(const yal_u32 argIdx) const override final;

    Type* typeOfReturnValue() const override final;

    Type* typeOfObject() const override final;

private:
    const FunctionDeclBaseNode* _pFunctionDecl;
};


class GeneratedFunctionType: public FunctionType
{
public:

    GeneratedFunctionType(const TypeId_t id,
                          Type* objectType,
                          Type* returnType,
                          const bool isNative);

    GeneratedFunctionType(const TypeId_t id,
                          Type* objectType,
                          Type* returnType,
                          const bool isNative,
                          std::vector<Type*>&& argTypes);

    virtual ~GeneratedFunctionType() {}

    bool hasReturnValue() const override final;

    bool hasArguments() const override final;

    bool isObjectFunction() const override final;

    yal_u32 argumentCount() const override final;

    Type* typeOfArgument(const yal_u32 argIdx) const override final;

    Type* typeOfReturnValue() const override final;

    Type* typeOfObject() const override final;
    
private:
    Type* _objectType;
    Type* _returnType;
    const std::vector<Type*> _argTypes;
};


}
#endif
