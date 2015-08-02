#ifndef __YAL_MODULECONSTANT_H__
#define __YAL_MODULECONSTANT_H__

#include "yal/enums.h"
#include "yal/module/moduleindexable.h"
#include "yal/util/constantvalue.h"
#include <vector>

namespace yal
{
class ModuleConstant : public ModuleIndexable
{
public:
    ModuleConstant(const ConstantValue& value);

    virtual ~ModuleConstant();

    const ConstantValue& value() const
    {
        return _constant;
    }

    bool matchesValue(const ConstantValue& val) const
    {
        return _constant == val;
    }

protected:
    const ConstantValue _constant;
};

}
#endif
