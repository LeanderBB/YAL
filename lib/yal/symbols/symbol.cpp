#include "yal/symbols/symbol.h"
#include "yal/ast/functionnode.h"
#include "yal/ast/variabledeclnode.h"

namespace yal
{

Symbol::Symbol(const char* name,
               const yal_u32 scopeLevel,
               AstBaseNode *astNode,
               const yal_u32 flags):
    _symName(name),
    _pSymScope(nullptr),
    _astNode(astNode),
    _readCount(0),
    _writeCount(0),
    _callCount(0),
    _scopeLevel(scopeLevel),
    _symbolFlags(flags)
{

}

void
Symbol::setScope(const SymbolTable* scope) const
{
    _pSymScope = scope;
}

void
Symbol::touchRead()
{
    ++_readCount;
}

void
Symbol::touchWrite()
{
    ++_writeCount;
}

void
Symbol::touchCall()
{
    ++_callCount;
}

bool
Symbol::isAssignable() const
{
    return _symbolFlags & kFlagAssignable;
}

bool
Symbol::isReadOnly() const
{
    return _symbolFlags & kFlagReadOnly;
}

bool
Symbol::isVariable() const
{
    return ast_cast<VariableDeclNode>(_astNode) != nullptr;
}

bool
Symbol::isFunction() const
{
    return ast_cast<FunctionDeclNode>(_astNode) != nullptr;
}

}
