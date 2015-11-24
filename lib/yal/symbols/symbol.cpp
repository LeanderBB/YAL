#include "yal/symbols/symbol.h"
#include "yal/ast/functionnode.h"
#include "yal/ast/variabledeclnode.h"
#include "yal/ast/argumentdeclnode.h"
#include "yal/types/typehdrs.h"

namespace yal
{

Symbol::Symbol(const char* name,
               const Scope* scope,
               Type *type,
               const yal_u32 flags):
    _symName(name),
    _pSymScope(scope),
    _type(type),
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

Type*
Symbol::symbolType() const
{
    return _type;
}

bool
Symbol::isVariable() const
{
    return _symbolFlags & kFlagVariable;
}

bool
Symbol::isFunction() const
{
    return _type->isFunctionType();
}

bool
Symbol::isNativeFunction() const
{
    return _type->isNativeImpl() && _type->isFunctionType();
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

bool
Symbol::isReference() const
{
    return _symbolFlags & kFlagReference;
}

bool
Symbol::isFunctionParam() const
{
    return _symbolFlags & kFlagFunctionParam;
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
