#include "yal/types/typeregistry.h"
#include "yal/module/module.h"
#include "yal/ast/functionnode.h"
#include "yal/types/typehdrs.h"
namespace yal
{

TypeRegistry::TypeRegistry(Module &module):
    _module(module),
    _types(),
    _typesByName()
{

}

ArrayType*
TypeRegistry::registerArray(const Type* valueType)
{
    const std::string type_string = ArrayType::GenTypeString(valueType);

    ArrayType* result = nullptr;
    if (!canRegisterNewType(type_string.c_str()))
    {
        Type* type = typeForName(type_string.c_str());
        result = cast_type<ArrayType>(type);
        YAL_ASSERT(type);
    }
    else
    {
        Type::TypeId_t id;
        generateTypeId(id);
        result = new ArrayType(id, valueType);
        registerType(result->typeString(), result);
    }
    return result;
}

FunctionType*
TypeRegistry::registerFunction(FunctionDeclBaseNode* node)
{
    if (!canRegisterNewType(node->nativeFunctionName()))
    {
        return nullptr;
    }

    Type::TypeId_t id;
    FunctionType* result = nullptr;
    generateTypeId(id);
    result= new AstFunctionType(id, node);
    registerType(node->nativeFunctionName(), result);
    node->setNodeType(result);
    return result;
}

Type*
TypeRegistry::registerUndefined(const char* typeString)
{
    if (!canRegisterNewType(typeString))
    {
        return nullptr;
    }

    Type::TypeId_t id;
    generateTypeId(id);
    Type* result = new UndefinedType(id, typeString);
    registerType(typeString, result);
    return result;
}

Type*
TypeRegistry::typeForId(const Type::TypeId_t& t) const
{
    for (auto&v: _types)
    {
        if (v->typeId() == t)
        {
            return v.get();
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
    id.split.dataTypeId = static_cast<yal_u32>(_types.size());
}

void
TypeRegistry::registerType(const char* name,
                           Type* t)
{
    _types.push_back(TypePtr_t(t));
    YAL_ASSERT(_types.size() == t->typeId().split.dataTypeId + 1);
    _typesByName.insert(std::make_pair(name, t));
}

}
