#ifndef __YAL_SYMBOL_H__
#define __YAL_SYMBOL_H__

#include "yal/yal.h"
#include "yal/enums.h"

namespace yal
{

typedef yal_u32 SymbolType_t;

class Type;
class SymbolTable;
class AstBaseNode;
class Symbol
{
public:

    enum Flags
    {
        kFlagAssignable     = 1 << 0,
        kFlagReadOnly       = 1 << 1,
        kFlagGlobalSymbol   = 1 << 2
    };

\
    Symbol(const char* name,
           const yal_u32 scopeLevel,
           AstBaseNode* astNode,
           const yal_u32 flags = 0);

    virtual ~Symbol() {}

    const char* symbolName() const
    {
        return _symName;
    }

    void setScope(const SymbolTable* scope) const;

    const SymbolTable* scope() const
    {
        return _pSymScope;
    }

    void touchRead();

    void touchWrite();

    void touchCall();

    yal_u32 readCount() const
    {
        return _readCount;
    }

    yal_u32 writeCount() const
    {
        return _writeCount;
    }

    yal_u32 callCount() const
    {
        return _callCount;
    }

    yal_u32 scopeLevel() const
    {
        return _scopeLevel;
    }

    yal_u32 symbolFlags() const
    {
        return _symbolFlags;
    }

    bool isAssignable() const;

    bool isReadOnly() const;

    AstBaseNode* astNode() const
    {
        return _astNode;
    }

    bool isVariable() const;

    bool isFunction() const;

    bool isGlobalSymbol() const
    {
        return _symbolFlags & kFlagGlobalSymbol;
    }

protected:
    const char* _symName;
    mutable const SymbolTable* _pSymScope;
    AstBaseNode* _astNode;
    yal_u32 _readCount;
    yal_u32 _writeCount;
    yal_u32 _callCount;
    const yal_u32 _scopeLevel;
    yal_u32 _symbolFlags;
};

}
#endif
