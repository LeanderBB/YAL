#include "yal/module/moduleconstant.h"
#include <algorithm>
namespace yal
{

ModuleConstant::ModuleConstant(const ConstantValue& value):
    _constant(value)
{

}

ModuleConstant::~ModuleConstant()
{

}
/*
void
ModuleConstant::addUse(const ModuleFunction* function)
{
    auto it = std::find(_usedByFunctions.begin(),
                        _usedByFunctions.end(),
                        function);
    if (it == _usedByFunctions.end())
    {
        _usedByFunctions.push_back(function);
    }
}

void
ModuleConstant::removeUse(const ModuleFunction* function)
{
    auto it = std::find(_usedByFunctions.begin(),
                        _usedByFunctions.end(),
                        function);
    if (it != _usedByFunctions.end())
    {
        _usedByFunctions.erase(it);
    }
}

bool
ModuleConstant::hasUse() const
{
    return !_usedByFunctions.empty();
}
*/

}

