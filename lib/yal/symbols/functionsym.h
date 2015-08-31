#ifndef __YAL_FUNCTIONSYM_H__
#define __YAL_FUNCTIONSYM_H__

#include <yal/symbols/symbol.h>

namespace yal
{

class FunctionSym : public Symbol
{
public:

    static const SymbolType_t SymbolType;

    FunctionSym(const char* functionName,
                const ConstantType returnType);

    ~FunctionSym();

    bool addArgument(const DataType argumentType);

    virtual SymbolType_t symbolType() const override;

    virtual DataType returnType() const override;

    virtual bool isCallable() const override;

    virtual bool supportsVariableArguments() const override;

    virtual yal_u32 argumentCount() const override;

    virtual DataType argumentTypeOf(const yal_u32 argIdx) const override;

private:
    const DataType _returnType;
    DataType _argumentTypes[kMaxFunctionArgs];
    yal_u32 _nArguments;
};

}
#endif
