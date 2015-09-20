#ifndef __YAL_MODULEGLOBAL_H__
#define __YAL_MODULEGLOBAL_H__

#include "yal/enums.h"
#include "yal/module/moduleindexable.h"
#include "yal/symbols/symbol.h"

namespace yal
{

class VariableSym;

/// Represents a global variable in a module
class ModuleGlobal : public ModuleIndexable
{
public:
    ModuleGlobal(const Symbol* symbol);

    virtual ~ModuleGlobal();

    const char* variableName() const;

    bool wasUsed() const;

    Type* variableType() const;

protected:
    const Symbol* _symbol;
};

}
#endif
