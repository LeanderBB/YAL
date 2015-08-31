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
class SymbolTable
{
public:

    typedef std::unique_ptr<Symbol> SymbolPtr_t;
    typedef yal::StrHashMap<SymbolPtr_t> SymbolMap_t;

    SymbolTable(const SymbolTable* parentTable,
                const yal_u32 level);

    ~SymbolTable();

    bool declareSymbol(Symbol *pSym);

    Symbol *resolveSymbol(const char* name) const;

    const SymbolTable* parent() const
    {
        return _parent;
    }

    void addChild(const SymbolTable* table);

    yal_u32 level() const
    {
        return _level;
    }

    SymbolMap_t::const_iterator symbolsBegin() const
    {
        return _symbols.begin();
    }

    SymbolMap_t::const_iterator symbolsEnd() const
    {
        return _symbols.end();
    }

protected:
    typedef std::unique_ptr<const SymbolTable> SymbolTablePtr_t;
    typedef std::vector<SymbolTablePtr_t> SymbolTableStack_t;

    const SymbolTable* _parent;
    SymbolMap_t _symbols;
    SymbolTableStack_t _children;
    const yal_u32 _level;
};

}
#endif
