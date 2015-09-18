#include "yal/types/typeregistry.h"
#include "yal/module/module.h"
#include "yal/ast/functionnode.h"
#include "yal/types/functiontype.h"

namespace yal
{

TypeRegistry::TypeRegistry(Module &module):
    _module(module),
    _types(),
    _typesByName()
{

}

FunctionType*
TypeRegistry::registerFunction(const FunctionDeclNode* node)
{
    if (!canRegisterNewType(node->functionName()))
    {
        return nullptr;
    }

    Type::TypeId_t id;
    FunctionType* result = nullptr;
    generateTypeId(id);
    result= new FunctionType(id, node);
    registerType(node->functionName(), result);
    return result;
}

Type*
TypeRegistry::typeForId(const Type::TypeId_t& t) const
{
    for (auto&v: _types)
    {
        if (v->typeId() == t)
        {
            return v;
        }
    }
    return nullptr;
}

Type*
TypeRegistry::typeForName(const char* name) const
{
    auto it = _typesByName.find(name);
    return (it != _typesByName.end()) ? it->second : nullptr;
}

bool
TypeRegistry::canRegisterNewType(const char* name) const
{
    return typeForName(name) == nullptr && _types.size() < LanguageDataTypeId();
}

void
TypeRegistry::generateTypeId(Type::TypeId_t& id) const
{
    id.split.moduleId = _module.moduleId();
    id.split.dataTypeId = _types.size();
}

void
TypeRegistry::registerType(const char* name,
                           Type* t)
{
    _types.push_back(t);
    YAL_ASSERT(_types.size() == t->typeId().split.dataTypeId + 1);
    _typesByName.insert(std::make_pair(name, t));
}

}
