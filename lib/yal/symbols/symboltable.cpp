#include "yal/symbols/symboltable.h"
#include "yal/symbols/scope.h"
namespace yal
{

SymbolTable::SymbolTable(const Scope *scope):
    _scope(scope),
    _symbols()
{

}

SymbolTable::~SymbolTable()
{

}

Symbol*
SymbolTable::declareSymbol(const char* name,
                           AstBaseNode* astNode,
                           const yal_u32 flags)
{
    SymbolPtr_t& symbol = _symbols[name];
    if (!symbol)
    {
        symbol = SymbolPtr_t(new Symbol(name, _scope, astNode, flags));
        return symbol.get();
    }
    return nullptr;
}

const SymbolTable*
SymbolTable::parentTable() const
{
    const Scope* parent_scope = _scope->parent();
    return (parent_scope) ? &parent_scope->symbolTable() : nullptr;
}

Symbol*
SymbolTable::resolveSymbol(const char* name) const
{
    auto it = _symbols.find(name);
    if (it == _symbols.end())
    {
        const SymbolTable* parent_syms = parentTable();
        if (parent_syms)
        {
            return parent_syms->resolveSymbol(name);
        }
    }
    else
    {
        return it->second.get();
    }
    return nullptr;
}

}
