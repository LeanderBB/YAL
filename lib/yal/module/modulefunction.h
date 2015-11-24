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
class ModuleFunction : public ModuleIndexable
{
public:
    ModuleFunction(const Symbol* symbol,
                   FunctionDeclBaseNode* node);

    virtual ~ModuleFunction();

    const char* functionName() const;

    const char* nativeFunctionName() const;

    bool wasUsed() const;

    const Symbol* symbol() const
    {
        return _symbol;
    }
    
    FunctionDeclBaseNode* functionNode() const
    {
        return _node;
    }

protected:
    const Symbol* _symbol;
    FunctionDeclBaseNode* _node;
    std::string _nativeFunctionName;
};



}

#endif
