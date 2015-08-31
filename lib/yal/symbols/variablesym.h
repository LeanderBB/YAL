#ifndef __YAL_VARIABLESYM_H__
#define __YAL_VARIABLESYM_H__

#include "yal/symbols/symbol.h"

namespace yal
{

class VariableSym : public Symbol
{
public:

    static const SymbolType_t SymbolType;

    VariableSym(const char* varName,
                const DataType varType,
                const bool isGlobal,
                const yal_u32 scopeLevel);

    virtual ~VariableSym();

    virtual SymbolType_t symbolType() const override;

    virtual DataType returnType() const override;

    virtual bool isCallable() const override;

    virtual bool supportsVariableArguments() const override;

    virtual yal_u32 argumentCount() const override;

    virtual DataType argumentTypeOf(const yal_u32 argIdx) const override;

    bool isGlobal() const
    {
        return _isGlobal;
    }

private:
    const DataType _variableType;
    const bool _isGlobal;
};

}
#endif
