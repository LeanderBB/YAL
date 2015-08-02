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
    return _symbol->readCount() > 0 || _symbol->callCount() > 0;
}

ConstantType
ModuleFunction::returnType() const
{
    return _symbol->returnType();
}

FunctionDeclNode *ModuleFunction::functionNode() const
{
    return _astNode;
}

}
