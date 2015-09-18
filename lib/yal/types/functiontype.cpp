#include "yal/types/functiontype.h"
#include "yal/ast/functionnode.h"
#include "yal/ast/argumentdeclnode.h"

namespace yal
{

FunctionType::FunctionType(const TypeId_t id,
                           const FunctionDeclNode* funcDecl):
    Type(id, kVmTypeFunction),
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
    return decl_node->type();
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

}
