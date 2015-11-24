#include "yal/types/functiontype.h"
#include "yal/ast/functionnode.h"
#include "yal/ast/argumentdeclnode.h"

namespace yal
{

YAL_TYPLE_IMPL_SRC(FunctionType, 0xed3f0f48)

FunctionType::FunctionType(const TypeId_t id):
    Type(id, kVmTypeFunction, FunctionType::ImpId)
{
    _typeFlags = kTypeFlagIsFunctionType;

}


bool
FunctionType::isPromotableTo(const Type*) const
{
    return false;
}

AstFunctionType::AstFunctionType(const TypeId_t id,
                           const FunctionDeclBaseNode *funcDecl):
FunctionType(id),
_pFunctionDecl(funcDecl)
{
    if (ast_typeof<FunctionDeclNativeNode>(funcDecl))
    {
        _typeFlags |= kTypeFlagIsNativeImpl;
    }
}

bool
AstFunctionType::hasReturnValue() const
{
    return _pFunctionDecl->hasReturnValue();
}

bool
AstFunctionType::hasArguments() const
{
    return _pFunctionDecl->hasFunctionArguments();
}

bool
AstFunctionType::isObjectFunction() const
{
    return _pFunctionDecl->isObjectFunction();
}

yal_u32
AstFunctionType::argumentCount() const
{
    return (hasArguments()) ? _pFunctionDecl->functionArguments()->argumentCount() : 0;
}

Type*
AstFunctionType::typeOfArgument(const yal_u32 argIdx) const
{
    const ArgumentDeclNode* decl_node = _pFunctionDecl->functionArguments()->arguments()[argIdx];
    return decl_node->nodeType();
}

Type*
AstFunctionType::typeOfReturnValue() const
{
    return  _pFunctionDecl->returnValueType();
}

Type*
AstFunctionType::typeOfObject() const
{
    return _pFunctionDecl->objectType();
}


GeneratedFunctionType::GeneratedFunctionType(const TypeId_t id,
                                             Type* objectType,
                                             Type* returnType,
                                             const bool isNative):
    FunctionType(id),
    _objectType(objectType),
    _returnType(returnType)
{
    if (isNative)
    {
        _typeFlags |= kTypeFlagIsNativeImpl;
    }
}

GeneratedFunctionType::GeneratedFunctionType(const TypeId_t id,
                                             Type* objectType,
                                             Type* returnType,
                                             const bool isNative,
                                             std::vector<Type*>&& argTypes):
    FunctionType(id),
    _objectType(objectType),
    _returnType(returnType),
    _argTypes(argTypes)
{
    if (isNative)
    {
        _typeFlags |= kTypeFlagIsNativeImpl;
    }
}


bool
GeneratedFunctionType::hasReturnValue() const
{
    return _returnType != nullptr;
}

bool
GeneratedFunctionType::hasArguments() const
{
    return !_argTypes.empty();
}

bool
GeneratedFunctionType::isObjectFunction() const
{
    return _objectType != nullptr;
}

yal_u32
GeneratedFunctionType::argumentCount() const
{
    return _argTypes.size();
}

Type*
GeneratedFunctionType::typeOfArgument(const yal_u32 argIdx) const
{
    YAL_ASSERT(argIdx < _argTypes.size());
    return _argTypes[argIdx];
}

Type*
GeneratedFunctionType::typeOfReturnValue() const
{
    return  _returnType;
}

Type*
GeneratedFunctionType::typeOfObject() const
{
    return _objectType;
}


}
