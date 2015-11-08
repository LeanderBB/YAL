#ifndef __YAL_MODULEFUNCTION_H__
#define __YAL_MODULEFUNCTION_H__

#include "yal/enums.h"
#include "yal/module/moduleindexable.h"
#include "yal/symbols/symbol.h"

namespace yal
{
class Type;
class FunctionSym;
class FunctionDeclBaseNode;
class FunctionDeclNode;
class FunctionDeclNativeNode;
class ModuleFunctionBase : public ModuleIndexable
{
public:
    ModuleFunctionBase(const Symbol* symbol,
                       FunctionDeclBaseNode* astNode);

    virtual ~ModuleFunctionBase();

    const char* functionName() const;

    bool wasUsed() const;

    Type* returnType() const;

    FunctionDeclBaseNode* functionNode() const {return _astNode;}

protected:
    const Symbol* _symbol;
    FunctionDeclBaseNode* _astNode;

};

/// Represents a function in a module
class ModuleFunction : public ModuleFunctionBase
{
public:
    ModuleFunction(const Symbol* symbol,
                   FunctionDeclNode* astNode);

    virtual ~ModuleFunction() {}

    FunctionDeclNode* functionNode() const;

protected:
    FunctionDeclNode* _astNode;
};

/// Represents a native function in a module
class ModuleFunctionNative : public ModuleFunctionBase
{
public:
    ModuleFunctionNative(const Symbol* symbol,
                         FunctionDeclNativeNode* astNode);

    virtual ~ModuleFunctionNative() {}

    FunctionDeclNativeNode* functionNode() const;

};


}

#endif
