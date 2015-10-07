#ifndef __YAL_FUNCTIONTYPE_H__
#define __YAL_FUNCTIONTYPE_H__

#include "yal/types/type.h"

namespace yal
{
class FunctionDeclNode;
class FunctionType : public Type
{
public:

    YAL_TYPE_IMPL_HDR(FunctionType)

    FunctionType(const TypeId_t id,
                 const FunctionDeclNode* funcDecl);

    virtual ~FunctionType() {}

    bool hasReturnValue() const;

    bool hasArguments() const;

    bool isObjectFunction() const;

    yal_u32 argumentCount() const;

    Type* typeOfArgument(const yal_u32 argIdx) const;

    Type* typeOfReturnValue() const;

    Type* typeOfObject() const;

    virtual bool isPromotableTo(const Type *t) const;

private:
    const FunctionDeclNode* _pFunctionDecl;
};

}
#endif
