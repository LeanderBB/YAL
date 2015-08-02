#include "yal/symbols/functionsym.h"

namespace yal
{

const SymbolType_t FunctionSym::SymbolType = 0xe6ddf880;

FunctionSym::FunctionSym(const char *functionName,
                         const ConstantType returnType):
    Symbol(functionName, 0),
    _returnType(returnType),
    _argumentTypes(),
    _nArguments(0)
{

}

FunctionSym::~FunctionSym()
{

}

bool
FunctionSym::addArgument(const ConstantType argumentType)
{
    if (_nArguments >= kMaxFunctionArgs)
    {
        return false;
    }

    _argumentTypes[_nArguments] = argumentType;
    ++_nArguments;
    return true;
}

SymbolType_t
FunctionSym::symbolType() const
{
    return FunctionSym::SymbolType;
}

ConstantType
FunctionSym::returnType() const
{
    return _returnType;
}

bool
FunctionSym::isCallable() const
{
    return true;
}

bool
FunctionSym::supportsVariableArguments() const
{
    return false;
}

yal_u32
FunctionSym::argumentCount() const
{
    return _nArguments;
}

ConstantType
FunctionSym::argumentTypeOf(const yal_u32 argIdx) const
{
    if (argIdx >= _nArguments)
    {
        return kConstantTypeNone;
    }
    return _argumentTypes[argIdx];
}

}
