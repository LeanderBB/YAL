#include "yal/symbols/symbol.h"

namespace yal
{

Symbol::Symbol(const char* name,
               const yal_u32 scopeLevel):
    _symName(name),
    _pSymScope(nullptr),
    _readCount(0),
    _writeCount(0),
    _callCount(0),
    _scopeLevel(scopeLevel)
{

}

void
Symbol::setScope(const SymbolTable* scope) const
{
    _pSymScope = scope;
}

void
Symbol::touchRead()
{
    ++_readCount;
}

void
Symbol::touchWrite()
{
    ++_writeCount;
}

void
Symbol::touchCall()
{
    ++_callCount;
}

}
