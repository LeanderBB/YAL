#ifndef __YAL_TYPEREGISTRY_H__
#define __YAL_TYPEREGISTRY_H__

#include "yal/types/type.h"
#include "yal/ast/astnodetypes.h"
#include "yal/util/str_utils.h"
#include "yal/util/stl_utils.h"

namespace yal
{

class FunctionDeclBaseNode;
class Module;
class TypeRegistry
{
public:
    TypeRegistry(Module& module);

    ArrayType* registerArray(const Type* valueType);

    FunctionType* registerFunction(FunctionDeclBaseNode* node);

    Type* registerUndefined(const char* typeString);

    Type* typeForId(const Type::TypeId_t& t) const;

    Type* typeForName(const char* name) const;

    void manageBuiltinType(Type* t);

private:

    bool canRegisterNewType(const char* name) const;

    void generateTypeId(Type::TypeId_t &id) const;

    void registerType(const char *name,
                      Type* t);

private:
    typedef std::unique_ptr<Type> TypePtr_t;
    typedef StrHashMap<Type*> TypeMap_t;
    typedef std::vector<TypePtr_t> TypeVec_t;
    Module& _module;
    TypeVec_t _types;
    TypeMap_t _typesByName;
    TypeVec_t _managedBuiltinTypes;
};

}
#endif
