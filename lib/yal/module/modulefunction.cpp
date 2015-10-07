#include "yal/module/modulefunction.h"
#include "yal/ast/functionnode.h"
#include "yal/types/functiontype.h"

namespace yal
{
ModuleFunction::ModuleFunction(const Symbol *symbol,
                               FunctionDeclNode* astNode):
    _symbol(symbol),
    _astNode(astNode)
{

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
    //return _symbol->readCount() > 0 || _symbol->callCount() > 0;
    return true;
}

Type*
ModuleFunction::returnType() const
{
    const FunctionType* fn_type = cast_type<FunctionType>(_symbol->astNode()->nodeType());
    YAL_ASSERT(fn_type);
    return fn_type->typeOfReturnValue();
}

FunctionDeclNode *ModuleFunction::functionNode() const
{
    return _astNode;
}

}
