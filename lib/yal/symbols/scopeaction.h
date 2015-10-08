#ifndef __YAL_SCOPEACTION__
#define __YAL_SCOPEACTION__

#include "yal/yal.h"

namespace yal
{

// fwd declare classes
#define YAL_SCOPE_ACTION_LIST_FUNC(x) class x;
#include "yal/symbols/scopeactionlist.h"
#undef YAL_SCOPE_ACTION_LIST_FUNC


class ScopeActionVisitor
{
public:
    virtual ~ScopeActionVisitor() {}

#define YAL_SCOPE_ACTION_LIST_FUNC(x)\
    virtual void visitOnEnter(const x& action) = 0; \
    virtual void visitOnExit(const x& action) = 0;
#include "yal/symbols/scopeactionlist.h"
#undef YAL_SCOPE_ACTION_LIST_FUNC
};

class ScopeAction
{
public:
    virtual ~ScopeAction() {}

    virtual void acceptOnEnter(ScopeActionVisitor& vistor) const = 0;

    virtual void acceptOnExit(ScopeActionVisitor& visitor) const = 0;

};

#define YAL_SCOPE_ACTION_IMPL_HDR(type) \
    inline virtual void acceptOnEnter(yal::ScopeActionVisitor& visitor) const override \
{ \
    visitor.visitOnEnter(*this); \
} \
    inline virtual void acceptOnExit(yal::ScopeActionVisitor& visitor) const override \
{ \
    visitor.visitOnExit(*this); \
} \

}

#endif

