#include "yal/module/modulefunction.h"
#include "yal/symbols/functionsym.h"

namespace yal
{
ModuleFunction::ModuleFunction(const FunctionSym* symbol,
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

DataType
ModuleFunction::returnType() const
{
    return _symbol->returnType();
}

FunctionDeclNode *ModuleFunction::functionNode() const
{
    return _astNode;
}

}
