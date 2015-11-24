#include "yal/module/modulefunction.h"
#include "yal/ast/functionnode.h"
#include "yal/types/functiontype.h"

namespace yal
{
ModuleFunction::ModuleFunction(const Symbol *symbol,
                               FunctionDeclBaseNode* node):
    _symbol(symbol),
    _node(node)
{
    FunctionType* fn_type = cast_type<FunctionType>(symbol->symbolType());
    if (!node)
    {
    _nativeFunctionName = FunctionDeclBaseNode::GenFunctionNameNative(nullptr,
                                                                      symbol->symbolName(),
                                                                      fn_type->typeOfReturnValue());
    }
}

ModuleFunction::~ModuleFunction()
{

}

const char*
ModuleFunction::functionName() const
{
    return _symbol->symbolName();
}

bool
ModuleFunction::wasUsed() const
{
    //TODO: Provide annotations to override behaviour
    const bool was_used = _symbol->readCount() > 0 || _symbol->callCount() > 0;
    return _symbol->isNativeFunction() ? was_used : true;
}


const char*
ModuleFunction::nativeFunctionName() const
{
    return (_node) ? _node->nativeFunctionName() : _nativeFunctionName.c_str();
}

}
