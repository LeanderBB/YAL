#ifndef __YAL_SYMBOL_H__
#define __YAL_SYMBOL_H__

#include "yal/yal.h"
#include "yal/enums.h"

namespace yal
{

typedef yal_u32 SymbolType_t;

class SymbolTable;
class Symbol
{
public:

    Symbol(const char* name,
           const yal_u32 scopeLevel);

    virtual ~Symbol() {}

    const char* symbolName() const
    {
        return _symName;
    }

    void setScope(const SymbolTable* scope) const;

    const SymbolTable* scope() const
    {
        return _pSymScope;
    }

    bool hasArguments() const
    {
        return argumentCount() != 0;
    }

    bool hasReturnValue() const
    {
        return returnType() != kConstantTypeNone;
    }

    void touchRead();

    void touchWrite();

    void touchCall();

    yal_u32 readCount() const
    {
        return _readCount;
    }

    yal_u32 writeCount() const
    {
        return _writeCount;
    }

    yal_u32 callCount() const
    {
        return _callCount;
    }

    yal_u32 scopeLevel() const
    {
        return _scopeLevel;
    }

    virtual SymbolType_t symbolType() const = 0;

    virtual ConstantType returnType() const = 0;

    virtual bool isCallable() const = 0;

    virtual bool supportsVariableArguments() const = 0;

    virtual yal_u32 argumentCount() const = 0;

    virtual ConstantType argumentTypeOf(const yal_u32 argIdx) const = 0;

protected:
    const char* _symName;
    mutable const SymbolTable* _pSymScope;
    yal_u32 _readCount;
    yal_u32 _writeCount;
    yal_u32 _callCount;
    const yal_u32 _scopeLevel;
};

template<class Sym>
inline bool symbol_typeof(const Symbol* pSymbol)
{
    return pSymbol->symbolType() == Sym::SymbolType;
}

template<class Sym>
inline Sym* symbol_cast(Symbol* pSymbol)
{
    return (symbol_typeof<Sym>(pSymbol)) ? static_cast<Sym*>(pSymbol) : nullptr;
}


}
#endif
