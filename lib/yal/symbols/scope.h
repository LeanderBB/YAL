#ifndef __YAL_SCOPE_H__
#define __YAL_SCOPE_H__

#include "yal/yal.h"
#include "yal/symbols/symboltable.h"
#include "yal/symbols/scopeaction.h"
#include <vector>
namespace yal
{

class Scope
{

public:


    Scope(const Scope* parent,
          const yal_u32 lvl);

    Scope* createChildScope();

    void addScopeAction(const ScopeAction* action);

    void onScopeEnter(ScopeActionVisitor& visitor) const;

    void onScopeExit(ScopeActionVisitor& visitor) const;

    const Scope* parent() const
    {
        return _parent;
    }

    SymbolTable const& symbolTable() const
    {
        return _symTable;
    }

    bool isRootScope() const
    {
        return _parent == nullptr;
    }

    inline Symbol* declareSymbol(const char* name,
                                 Type* type,
                                 const yal_u32 flags)
    {
        return _symTable.declareSymbol(name, type, flags);
    }

    inline Symbol *resolveSymbol(const char* name) const
    {
        return _symTable.resolveSymbol(name);
    }

    inline void eraseSymbol(const Symbol* sym)
    {
        _symTable.eraseSymbol(sym);
    }

    inline Symbol* declareTemporarySymbol(Type* type,
                                          const yal_u32 flags = 0)
    {
        return  _symTable.declareTemporarySymbol(type, flags);
    }

    yal_u32 level() const
    {
        return _scopeLvl;
    }

protected:
    typedef std::vector<std::unique_ptr<const ScopeAction>> ScopeActionsVec_t;
    typedef std::vector<std::unique_ptr<Scope>> ChildVec_t;
    const Scope* _parent;
    SymbolTable _symTable;
    ChildVec_t _childScopes;
    ScopeActionsVec_t _scopeActions;
    const yal_u32 _scopeLvl;
};

}
#endif
