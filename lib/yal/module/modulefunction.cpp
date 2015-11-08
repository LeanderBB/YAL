#include "yal/module/modulefunction.h"
#include "yal/ast/functionnode.h"
#include "yal/types/functiontype.h"

namespace yal
{
ModuleFunctionBase::ModuleFunctionBase(const Symbol *symbol,
                                       FunctionDeclBaseNode *astNode):
    _symbol(symbol),
    _astNode(astNode)
{

}

ModuleFunctionBase::~ModuleFunctionBase()
{

}

const char*
ModuleFunctionBase::functionName() const
{
    return _symbol->symbolName();
}

bool
ModuleFunctionBase::wasUsed() const
{
    //TODO: Provide annotations to override behaviour
    //return _symbol->readCount() > 0 || _symbol->callCount() > 0;
    return true;
}

Type*
ModuleFunctionBase::returnType() const
{
    const FunctionType* fn_type = cast_type<FunctionType>(_symbol->astNode()->nodeType());
    YAL_ASSERT(fn_type);
    return fn_type->typeOfReturnValue();
}


ModuleFunction::ModuleFunction(const Symbol *symbol,
                               FunctionDeclNode* astNode):
    ModuleFunctionBase(symbol, astNode)
{

}

FunctionDeclNode *
ModuleFunction::functionNode() const
{
    return ast_cast<FunctionDeclNode>(_astNode);
}

ModuleFunctionNative::ModuleFunctionNative(const Symbol *symbol,
                                           FunctionDeclNativeNode* astNode):
    ModuleFunctionBase(symbol, astNode)
{

}

FunctionDeclNativeNode
*ModuleFunctionNative::functionNode() const
{
    return ast_cast<FunctionDeclNativeNode>(_astNode);
}

}
