#include "yal/symbols/symboltable.h"
#include "yal/symbols/scope.h"
#include <sstream>
namespace yal
{

static std::string
make_tmp_name(const Scope* scope,
              const yal_u32 counter)
{
    std::stringstream stream;
    stream << "tmp_sym@" << scope->level() <<":"<<counter;
    return stream.str();
}

SymbolTable::SymbolTable(const Scope *scope):
    _scope(scope),
    _symbols(),
    _tmpCounter(0)
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

Symbol*
SymbolTable::declareTemporarySymbol(AstBaseNode* astNode,
                                    const yal_u32 symFlags)
{
    const yal_u32 flags = symFlags | Symbol::kFlagTemporary | Symbol::kFlagAssignable;
    const std::string tmp_name (make_tmp_name(_scope, _tmpCounter++));
    SymbolPtr_t sym (new Symbol(tmp_name.c_str(), _scope, astNode, flags));
    SymbolPtr_t& symbol = _symbols[sym->symbolName()];
    if (!symbol)
    {
        symbol = std::move(sym);
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

void
SymbolTable::eraseSymbol(const Symbol *sym)
{
    _symbols.erase(sym->symbolName());
}
}
