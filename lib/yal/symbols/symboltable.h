#ifndef __YAL_SYMBOLTABLE_H__
#define __YAL_SYMBOLTABLE_H__

#include <unordered_map>
#include <memory>
#include <vector>
#include <cstring>
#include "yal/util/str_utils.h"
#include "yal/symbols/symbol.h"

namespace yal
{
class Scope;
class SymbolTable
{
public:

    typedef std::unique_ptr<Symbol> SymbolPtr_t;
    typedef yal::StrHashMap<SymbolPtr_t> SymbolMap_t;

    SymbolTable(const Scope* scope);

    ~SymbolTable();

    Symbol* declareSymbol(const char* name,
                       AstBaseNode* astNode,
                       const yal_u32 flags);

    Symbol *resolveSymbol(const char* name) const;

    const Scope* scope() const
    {
        return _scope;
    }

    const SymbolTable* parentTable() const;

    SymbolMap_t::const_iterator symbolsBegin() const
    {
        return _symbols.begin();
    }

    SymbolMap_t::const_iterator symbolsEnd() const
    {
        return _symbols.end();
    }

protected:
    const Scope* _scope;
    SymbolMap_t _symbols;
};

}
#endif
