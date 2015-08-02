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
                const ConstantType varType,
                const bool isGlobal,
                const yal_u32 scopeLevel);

    virtual ~VariableSym();

    virtual SymbolType_t symbolType() const override;

    virtual ConstantType returnType() const override;

    virtual bool isCallable() const override;

    virtual bool supportsVariableArguments() const override;

    virtual yal_u32 argumentCount() const override;

    virtual ConstantType argumentTypeOf(const yal_u32 argIdx) const override;

    bool isGlobal() const
    {
        return _isGlobal;
    }

private:
    const ConstantType _variableType;
    const bool _isGlobal;
};

}
#endif
