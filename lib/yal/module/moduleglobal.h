#ifndef __YAL_MODULEGLOBAL_H__
#define __YAL_MODULEGLOBAL_H__

#include "yal/enums.h"
#include "yal/module/moduleindexable.h"

namespace yal
{

class VariableSym;

/// Represents a global variable in a module
class ModuleGlobal : public ModuleIndexable
{
public:
    ModuleGlobal(const VariableSym* symbol);

    virtual ~ModuleGlobal();

    const char* variableName() const;

    bool wasUsed() const;

    ConstantType variableType() const;

protected:
    const VariableSym* _symbol;
};

}
#endif
