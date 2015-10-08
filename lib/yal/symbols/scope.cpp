#include "yal/symbols/scope.h"
#include <algorithm>

namespace yal
{

Scope::Scope(const Scope* parent,
             const yal_u32 lvl):
    _parent(parent),
    _symTable(this),
    _childScopes(),
    _scopeLvl(lvl)
{

}

Scope*
Scope::createChildScope()
{
    _childScopes.emplace_back(new Scope(this, _scopeLvl + 1));
    return _childScopes.rbegin()->get();
}

void
Scope::addScopeAction(const ScopeAction* action)
{
    _scopeActions.emplace_back(action);
}


void
Scope::onScopeEnter(ScopeActionVisitor& visitor) const
{
    for(auto& action : _scopeActions)
    {
        action->acceptOnEnter(visitor);
    }
}

void
Scope::onScopeExit(ScopeActionVisitor& visitor) const
{
    for(auto it = _scopeActions.rbegin(); it != _scopeActions.rend(); ++it)
    {
        (*it)->acceptOnExit(visitor);
    }
}

}
