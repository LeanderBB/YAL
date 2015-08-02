#ifndef __YAL_MODULEFUNCTION_H__
#define __YAL_MODULEFUNCTION_H__

#include "yal/enums.h"
#include "yal/module/moduleindexable.h"

namespace yal
{
class FunctionSym;
class FunctionDeclNode;
/// Represents a function in a module
class ModuleFunction : public ModuleIndexable
{
public:
    ModuleFunction(const FunctionSym* symbol,
                   FunctionDeclNode* astNode);

    virtual ~ModuleFunction();

    const char* functionName() const;

    bool wasUsed() const;

    ConstantType returnType() const;

    FunctionDeclNode* functionNode() const;

protected:
    const FunctionSym* _symbol;
    FunctionDeclNode* _astNode;
};

}

#endif
