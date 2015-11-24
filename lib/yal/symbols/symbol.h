#ifndef __YAL_SYMBOL_H__
#define __YAL_SYMBOL_H__

#include "yal/yal.h"
#include "yal/enums.h"
#include <string>
namespace yal
{

typedef yal_u32 SymbolType_t;

class Type;
class Scope;
class AstBaseNode;
class Symbol
{
public:

    enum Flags
    {
        kFlagAssignable     = 1 << 0,
        kFlagReadOnly       = 1 << 1,
        kFlagGlobalSymbol   = 1 << 2,
        kFlagTemporary      = 1 << 3,
        kFlagNewObject      = 1 << 4,
        kFlagAssigned       = 1 << 5,
        kFlagReturnValue    = 1 << 6,
        kFlagReference      = 1 << 7,
        kFlagFunctionParam  = 1 << 8,
        kFlagVariable       = 1 << 9
    };

    Symbol(const char* name,
           const Scope* scope,
           Type* Type,
           const yal_u32 flags = 0);

    virtual ~Symbol() {}

    const char* symbolName() const
    {
        return _symName.c_str();
    }

    const Scope* scope() const
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

    yal_u32 symbolFlags() const
    {
        return _symbolFlags;
    }

    bool isAssignable() const;

    bool isReadOnly() const;

    Type* symbolType() const;

    bool isVariable() const;

    bool isFunction() const;

    bool isNativeFunction() const;

    bool isGlobalSymbol() const;

    bool isGlobalVariable() const;

    bool isTemporary() const;

    bool isNewObject() const;

    bool isAssigned() const;

    bool isReturnValue() const;

    bool isReference() const;

    bool isFunctionParam() const;

    void markAssigned();

    void markReturnValue();

    void addFlags(const yal_u32 flags);

protected:
    const std::string _symName;
    const Scope* _pSymScope;
    Type* _type;
    yal_u32 _readCount;
    yal_u32 _writeCount;
    yal_u32 _callCount;
    yal_u32 _symbolFlags;
};

}
#endif
