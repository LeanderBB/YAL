#include "yal/symbols/objectscopeaction.h"
#include "yal/symbols/symbol.h"
#include "yal/ast/astbasenode.h"
#include "yal/types/type.h"
namespace yal
{
ObjectScopeAction::ObjectScopeAction(const Symbol* symbol):
    ScopeAction(),
    _symbol(symbol)
{
    YAL_ASSERT(symbol->astNode()->nodeType()->isObjectType());
}

}

