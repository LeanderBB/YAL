#include "yal/symbols/symbol.h"

namespace yal
{

const DataType DataType::VoidType;

const char*
DataType::ToStr(const DataType& type)
{
    return (type.origin == kSymbolDataOriginBuiltin)
            ? ConstantTypeToStr(type.data.builtin)
            : type.data.custom->symbolName();
}

bool
DataType::IsInteger(const DataType& type)
{
    return (type.origin == kSymbolDataOriginBuiltin)
            ? ConstantTypeIsInteger(type.data.builtin)
            : false;
}

bool
DataType::IsNumber(const DataType& type)
{
    return (type.origin == kSymbolDataOriginBuiltin)
            ? ConstantTypeIsNumber(type.data.builtin)
            : false;
}

bool
DataType::IsSigned(const DataType& type)
{
    return (type.origin == kSymbolDataOriginBuiltin)
            ? ConstantTypeIsSigned(type.data.builtin)
            : false;
}

bool
DataType::Is32Bits(const DataType& type)
{
    return (type.origin == kSymbolDataOriginBuiltin)
            ? ConstantTypeIs32Bits(type.data.builtin)
            : false;
}

bool
DataType::Is64Bits(const DataType& type)
{
    return (type.origin == kSymbolDataOriginBuiltin)
            ? ConstantTypeIs64Bits(type.data.builtin)
            : false;
}

bool
DataType::IsRefCounted(const DataType& type)
{
    return (type.origin != kSymbolDataOriginBuiltin)
            ? type.data.custom->isRefCounted()
            : false;
}

DataType::DataType():
    origin(kSymbolDataOriginBuiltin)
{
    this->data.builtin = kConstantTypeNone;
}

DataType::DataType(const ConstantType type):
    origin(kSymbolDataOriginBuiltin)
{
    this->data.builtin = type;
}

DataType::DataType(const Symbol* type):
    origin(kSymbolDataOriginCustom)
{
    this->data.custom = type;
}

bool
DataType::operator == (const DataType& other) const
{
    if (this->origin != other.origin)
    {
        return false;
    }

    if (this->origin == kSymbolDataOriginBuiltin)
    {
        return this->data.builtin == other.data.builtin;
    }
    else
    {
        return this->data.custom == other.data.custom;
    }
}

bool
DataType::operator != (const DataType& other) const
{
    return !(*this == other);
}

Symbol::Symbol(const char* name,
               const yal_u32 scopeLevel):
    _symName(name),
    _pSymScope(nullptr),
    _readCount(0),
    _writeCount(0),
    _callCount(0),
    _scopeLevel(scopeLevel)
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

}
