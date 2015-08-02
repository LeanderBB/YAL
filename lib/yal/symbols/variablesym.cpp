#include "yal/symbols/variablesym.h"

namespace yal
{

const SymbolType_t VariableSym::SymbolType = 0x17603f10;

VariableSym::VariableSym(const char* varName,
                         const ConstantType varType,
                         const bool isGlobal,
                         const yal_u32 scopeLevel):
    Symbol(varName, scopeLevel),
    _variableType(varType),
    _isGlobal(isGlobal)
{

}

VariableSym::~VariableSym()
{

}

SymbolType_t
VariableSym::symbolType() const
{
    return VariableSym::SymbolType;
}

ConstantType
VariableSym::returnType() const
{
    return _variableType;
}

bool
VariableSym::isCallable() const
{
    return false;
}

bool
VariableSym::supportsVariableArguments() const
{
    return false;
}

yal_u32
VariableSym::argumentCount() const
{
    return 0;
}

ConstantType
VariableSym::argumentTypeOf(const yal_u32 argIdx) const
{
    (void) argIdx;
    return _variableType;
}

}
