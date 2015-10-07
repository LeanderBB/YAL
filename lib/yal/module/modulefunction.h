#ifndef __YAL_MODULEFUNCTION_H__
#define __YAL_MODULEFUNCTION_H__

#include "yal/enums.h"
#include "yal/module/moduleindexable.h"
#include "yal/symbols/symbol.h"

namespace yal
{
class Type;
class FunctionSym;
class FunctionDeclNode;
/// Represents a function in a module
class ModuleFunction : public ModuleIndexable
{
public:
    ModuleFunction(const Symbol* symbol,
                   FunctionDeclNode* astNode);

    virtual ~ModuleFunction();

    const char* functionName() const;

    bool wasUsed() const;

    Type* returnType() const;

    FunctionDeclNode* functionNode() const;

protected:
    const Symbol* _symbol;
    FunctionDeclNode* _astNode;
};

}

#endif
