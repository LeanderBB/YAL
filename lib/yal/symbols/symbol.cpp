#include "yal/symbols/symbol.h"
#include "yal/ast/functionnode.h"
#include "yal/ast/variabledeclnode.h"
#include "yal/ast/argumentdeclnode.h"
namespace yal
{

Symbol::Symbol(const char* name,
               const Scope* scope,
               AstBaseNode *astNode,
               const yal_u32 flags):
    _symName(name),
    _pSymScope(scope),
    _astNode(astNode),
    _readCount(0),
    _writeCount(0),
    _callCount(0),
    _symbolFlags(flags)
{

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
    return ast_typeof<VariableDeclNode>(_astNode)
            || ast_typeof<ArgumentDeclNode>(_astNode);
}

bool
Symbol::isFunction() const
{
    return ast_typeof<FunctionDeclNode>(_astNode)
            || ast_typeof<FunctionDeclNativeNode>(_astNode);
}

bool
Symbol::isNativeFunction() const
{
    return ast_typeof<FunctionDeclNativeNode>(_astNode);
}

bool
Symbol::isGlobalSymbol() const
{
    return _symbolFlags & kFlagGlobalSymbol;
}

bool
Symbol::isGlobalVariable() const
{
    return isVariable() && isGlobalSymbol();
}

bool
Symbol::isTemporary() const
{
    return _symbolFlags & kFlagTemporary;
}

bool
Symbol::isNewObject() const
{
    return _symbolFlags & kFlagNewObject;
}

bool
Symbol::isAssigned() const
{
    return _symbolFlags & kFlagAssigned;
}

bool
Symbol::isReturnValue() const
{
    return _symbolFlags & kFlagReturnValue;
}

void
Symbol::markAssigned()
{
    _symbolFlags |= kFlagAssigned;
}

void
Symbol::markReturnValue()
{
    _symbolFlags |= kFlagReturnValue;
}

void
Symbol::addFlags(const yal_u32 flag)
{
    _symbolFlags |= flag;
}

}
