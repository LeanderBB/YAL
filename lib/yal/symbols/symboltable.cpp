#include "yal/symbols/symboltable.h"

namespace yal
{

SymbolTable::SymbolTable(const SymbolTable *parentTable,
                         const yal_u32 level):
    _parent(parentTable),
    _symbols(),
    _children(),
    _level(level)
{

}

SymbolTable::~SymbolTable()
{

}

bool
SymbolTable::declareSymbol(Symbol *pSym)
{
    SymbolPtr_t& symbol = _symbols[pSym->symbolName()];
    if (!symbol)
    {
        symbol = SymbolPtr_t(pSym);
        pSym->setScope(this);
        return true;
    }
    return false;
}

Symbol*
SymbolTable::resolveSymbol(const char* name) const
{
    auto it = _symbols.find(name);
    if (it == _symbols.end())
    {
        if (_parent)
        {
            return _parent->resolveSymbol(name);
        }
    }
    else
    {
        return it->second.get();
    }
    return nullptr;
}

void
SymbolTable::addChild(const SymbolTable* table)
{
    YAL_ASSERT(table->parent() == this);
    _children.push_back(SymbolTablePtr_t(table));
}

}
