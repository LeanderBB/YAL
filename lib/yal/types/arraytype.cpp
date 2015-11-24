#include "yal/types/arraytype.h"
#include "yal/types/typehdrs.h"
#include <string>
#include "yal/types/typeregistry.h"
#include "yal/symbols/scope.h"
#include "yal/module/module.h"
namespace yal
{

YAL_TYPLE_IMPL_SRC(ArrayType, 0xb7125643)


static const char* ArrayBuiltinSymbols[] = {
    "length", "array_any_length",
};

static_assert(YAL_ARRAY_SIZE(ArrayBuiltinSymbols) % 2 == 0,
              "ArrayBuiltinSymbols must be a pair value");



void
ArrayType::RegisterBuiltinFunctions(Module& module,
                                    TypeRegistry& typeReg,
                                    Scope& globalScope)
{
    GeneratedFunctionType* array_len_function = new GeneratedFunctionType(LanguageTypeId(),
                                                                          typeReg.registerArray(AnyType::GetType()),
                                                                          BuiltinType::GetBuiltinType(BuiltinType::kUInt32),
                                                                          true);
    Symbol* array_len_sym = globalScope.declareSymbol("array_any_length", array_len_function, 0);
    YAL_ASSERT(array_len_sym);

    module.addFunction(new ModuleFunction(array_len_sym, nullptr));
}

std::string
ArrayType::GenTypeString(const Type* valueType)
{
    return '[' + std::string(valueType->typeString())+ ']';
}

std::string
ArrayType::GenTypeStringNative(const Type* valueType)
{
    return "array_" + std::string(valueType->typeString());
}

ArrayType::ArrayType(const TypeId_t& id,
                     const Type* valueType):
    Type(id, kVmTypeArray, ImpId),
    _arrayTypeString(GenTypeString(valueType)),
    _nativeArrayTypeString(GenTypeStringNative(valueType)),
    _valueType(valueType)
{
    _typeFlags = kTypeFlagIsPtrSized | kTypeFlagIsObjectType
        | kTypeFlagIsArrayType | kTypeFlagIsCompoundType | kTypeFlagIsBuiltinType;
    _operatorFlags = kOperatorTypeArray;
}

bool
ArrayType::isPromotableTo(const Type *t) const
{
    const ArrayType* other = cast_type<const ArrayType>(t);
    if (other)
    {
        if (cast_type<const AnyType>(other->valueType())) return true;
        return other->valueType()->impId() == _valueType->impId();
    }
    return false;
}

const char*
ArrayType::builtinFunctionSymName(const char* name) const
{
    static const yal_u32 array_size = YAL_ARRAY_SIZE(ArrayBuiltinSymbols);
    const size_t name_len = strlen(name);
    for (yal_u32 i = 0; i < array_size; i+=2)
    {
        const char* fn_name = ArrayBuiltinSymbols[i];
        const char* fn_sym_name = ArrayBuiltinSymbols[i + 1];
        if (strncmp(fn_name, name, name_len) == 0)
        {
            return fn_sym_name;
        }
    }
    return nullptr;
}

}

