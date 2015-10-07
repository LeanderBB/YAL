#ifndef __YAL_MODULE_H__
#define __YAL_MODULE_H__

#include "yal/module/moduleconstant.h"
#include "yal/module/modulefunction.h"
#include "yal/module/moduleglobal.h"
#include "yal/util/str_utils.h"
#include "yal/util/outputsink.h"
#include <memory>

namespace yal
{
class Module
{
public:
    typedef std::unique_ptr<const ModuleFunction> FunctionPtr_t;
    typedef StrHashMap<FunctionPtr_t> FunctionMap_t;
    typedef std::vector<const ModuleConstant*> OrderedConstantVec_t;
    typedef std::vector<const ModuleGlobal*> OrderedGlobalVec_t;
    typedef std::vector<const ModuleFunction*> OrderedFunctionVec_t;
    typedef std::vector<const ModuleConstant*> OrderedStringsVec_t;

    Module();

    ~Module();

    const ModuleFunction* function(const char *name) const;

    bool addFunction(const ModuleFunction* function);

    const ModuleGlobal* global(const char* name) const;

    bool addGlobal(const ModuleGlobal* global);

    ModuleConstant* constant(const ConstantValue& value) const;

    bool addConstant(ModuleConstant* constant);

    void removeUnusedAndAssignIndices();

    const FunctionMap_t& functionMap() const
    {
        return _functionMap;
    }

    const OrderedConstantVec_t& constants32() const
    {
        return _constants32;
    }

    const OrderedGlobalVec_t& globals32() const
    {
        return _globals32;
    }

    const OrderedConstantVec_t& constants64() const
    {
        return _constants64;
    }

    const OrderedGlobalVec_t& globals64() const
    {
        return _globals64;
    }

    const OrderedGlobalVec_t& globalsPtr() const
    {
        return _globalsPtr;
    }

    const OrderedStringsVec_t& strings() const
    {
        return _stringsVec;
    }

    const OrderedFunctionVec_t& functions() const
    {
        return _functions;
    }

    size_t totalStringSizeBytes() const
    {
        return _totalStringSizeBytes;
    }

    yal_u32 moduleId() const
    {
        //TODO: generate this!!
        return 0;
    }

private:
    typedef std::unique_ptr<const ModuleGlobal> GlobalPtr_t;
    typedef StrHashMap<GlobalPtr_t> GlobalMap_t;
    typedef std::unique_ptr<ModuleConstant> ConstantPtr_t;
    typedef std::vector<ConstantPtr_t> ConstantVec_t;
    typedef yal::StrHashMap<ConstantPtr_t> StringMap_t;
    FunctionMap_t _functionMap;
    GlobalMap_t _globalMap;
    ConstantVec_t _constants;
    StringMap_t _strings;
    OrderedConstantVec_t _constants32;
    OrderedConstantVec_t _constants64;
    OrderedStringsVec_t _stringsVec;
    OrderedGlobalVec_t _globals32;
    OrderedGlobalVec_t _globals64;
    OrderedGlobalVec_t _globalsPtr;
    OrderedFunctionVec_t _functions;
    size_t _totalStringSizeBytes;
};

}
#endif
