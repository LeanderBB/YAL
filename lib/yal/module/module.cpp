#include "yal/module/module.h"
#include "yal/module/moduleindexable.h"
#include "yal/util/outputformater.h"
#include "yal/types/type.h"
#include <cstring>
namespace yal
{

const yal_u32 ModuleIndexable::IndexUnused = 0xFFFFFFFF;
const yal_u32 ModuleIndexable::IndexMax = 0xFFFF;

Module::Module()
{

}

Module::~Module()
{

}

const ModuleFunction*
Module::function(const char *name) const
{
    auto it = _functionMap.find(name);
    return it != _functionMap.end() ? it->second.get() : nullptr;
}

bool
Module::addFunction(const ModuleFunction* function)
{
    FunctionPtr_t& ptr = _functionMap[function->functionName()];
    if (!ptr)
    {
        ptr = FunctionPtr_t(function);
        return true;
    }
    return false;
}

const ModuleGlobal*
Module::global(const char* name) const
{
    auto it = _globalMap.find(name);
    return it != _globalMap.end() ? it->second.get() : nullptr;
}

bool
Module::addGlobal(const ModuleGlobal* global)
{
    GlobalPtr_t& ptr = _globalMap[global->variableName()];
    if (!ptr)
    {
        ptr = GlobalPtr_t(global);
        return true;
    }
    return false;
}

ModuleConstant*
Module::constant(const ConstantValue& value) const
{
    if (value.type() == kConstantTypeText)
    {
        auto it = _strings.find(value.valueAsText());
        if (it != _strings.end())
        {
            return it->second.get();
        }
    }
    else
    {
        for (auto& v : _constants)
        {
            if (v->matchesValue(value))
            {
                return v.get();
            }
        }
    }
    return nullptr;
}

bool
Module::addConstant(ModuleConstant* constant)
{
    if (constant->value().type() == kConstantTypeText)
    {
        auto it = _strings.find(constant->value().valueAsText());
        if (it != _strings.end())
        {
            return false;
        }
        _strings.insert(std::make_pair(constant->value().valueAsText(),ConstantPtr_t(constant)));
    }
    else
    {
        for (auto& v : _constants)
        {
            if (v->matchesValue(constant->value()))
            {
                return false;
            }
        }
        _constants.push_back(ConstantPtr_t(constant));
    }
    return true;
}

void
Module::removeUnusedAndAssignIndices()
{
    _constants32.clear();
    _constants64.clear();
    _stringsVec.clear();
    _globals32.clear();
    _globals64.clear();
    _functions.clear();
    _totalStringSizeBytes = 0;

    //process functions
    _functions.reserve(_functionMap.size());
    for(auto func_it = _functionMap.begin();
        func_it != _functionMap.end();)
    {
        const FunctionPtr_t& function = func_it->second;
        if (function->wasUsed())
        {
            function->setModuleIndex(_functions.size());
            _functions.push_back(function.get());
            ++func_it;
        }
        else
        {
            func_it = _functionMap.erase(func_it);
        }
    }

    //process globals
    for(auto globals_it = _globalMap.begin();
        globals_it != _globalMap.end();)
    {
        const GlobalPtr_t& global = globals_it->second;
        //if (global->wasUsed())
        {
            if (global->variableType()->is32BitSized())
            {
                global->setModuleIndex(_globals32.size());
                _globals32.push_back(global.get());
            }
            else
            {
                YAL_ASSERT(global->variableType()->is64BitSized());
                global->setModuleIndex(_globals64.size());
                _globals64.push_back(global.get());
            }
            ++globals_it;
        }
        /*else
        {
            //TODO: dectect proper global usage?
            //globals_it = _globals.erase(globals_it);
            ++globals_it;
        }*/
    }

    // process constants
    for(auto constant_it = _constants.begin();
        constant_it != _constants.end();)
    {
        ConstantPtr_t& constant = *constant_it;

        if (constant->value().valueIs32Bits())
        {
            constant->setModuleIndex(_constants32.size());
            _constants32.push_back(constant.get());
        }
        else
        {
            constant->setModuleIndex(_constants64.size());
            _constants64.push_back(constant.get());
        }
        ++constant_it;
    }

    // process strings

    auto strings_end = _strings.end();
    _stringsVec.reserve(_strings.size());
    for(auto strings_it = _strings.begin(); strings_it != strings_end; ++strings_it)
    {
        strings_it->second->setModuleIndex(_stringsVec.size());
        _stringsVec.push_back(strings_it->second.get());
        _totalStringSizeBytes += strlen(strings_it->second->value().valueAsText()) + 1 + sizeof(yal_u32);
    }
}

}

