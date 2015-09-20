#include "yal/types/functiontype.h"
#include "yal/ast/functionnode.h"
#include "yal/ast/argumentdeclnode.h"

namespace yal
{

YAL_TYPLE_IMPL_SRC(FunctionType, 0xed3f0f48)

FunctionType::FunctionType(const TypeId_t id,
                           const FunctionDeclNode* funcDecl):
    Type(id, kVmTypeFunction, FunctionType::ImpId),
    _pFunctionDecl(funcDecl)
{
    _typeFlags = kTypeFlagIsFunctionType;
}

bool
FunctionType::hasReturnValue() const
{
    return _pFunctionDecl->hasReturnValue();
}

bool
FunctionType::hasArguments() const
{
    return _pFunctionDecl->hasFunctionArguments();
}

bool
FunctionType::isObjectFunction() const
{
    //TODO: Implement when needed
    return false;
}

yal_u32
FunctionType::argumentCount() const
{
    return (hasArguments()) ? _pFunctionDecl->functionArguments()->argumentCount() : 0;
}

Type*
FunctionType::typeOfArgument(const yal_u32 argIdx) const
{
    const ArgumentDeclNode* decl_node = _pFunctionDecl->functionArguments()->arguments()[argIdx];
    return decl_node->nodeType();
}

Type*
FunctionType::typeOfReturnValue() const
{
    return (hasReturnValue()) ? _pFunctionDecl->returnValueType() : nullptr;
}

Type*
FunctionType::typeOfObject() const
{
    //TODO: implement when needed
    return nullptr;
}

bool
FunctionType::isPromotableTo(const Type*) const
{
    return false;
}

}
