#include "yal/module/moduleglobal.h"
#include "yal/ast/astbasenode.h"
namespace yal
{

ModuleGlobal::ModuleGlobal(const Symbol *symbol):
    _symbol(symbol)
{

}

ModuleGlobal::~ModuleGlobal()
{

}

const char*
ModuleGlobal::variableName() const
{
    return _symbol->symbolName();
}

bool
ModuleGlobal::wasUsed() const
{
    return _symbol->readCount() > 0 || _symbol->writeCount() > 0;
}

Type *ModuleGlobal::variableType() const
{
    return _symbol->symbolType();
}

}
