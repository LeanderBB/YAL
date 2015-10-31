#ifndef __YAL_OBJECTSCOPEACTION_H__
#define __YAL_OBJECTSCOPEACTION_H__

#include "yal/symbols/scopeaction.h"

namespace yal
{
class Symbol;
class ObjectScopeAction : public ScopeAction
{
public:

    YAL_SCOPE_ACTION_IMPL_HDR(ObjectScopeAction)

    ObjectScopeAction(const Symbol* symbol);

    const Symbol* symbol() const
    {
        return _symbol;
    }

private:
    const Symbol* _symbol;
};

}

#endif
