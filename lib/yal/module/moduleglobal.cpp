#include "yal/module/moduleglobal.h"
#include "yal/symbols/variablesym.h"
namespace yal
{

ModuleGlobal::ModuleGlobal(const VariableSym* symbol):
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

DataType
ModuleGlobal::variableType() const
{
    return _symbol->returnType();
}

}
