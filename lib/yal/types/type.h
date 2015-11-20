#ifndef __YAL_TYPE_H__
#define __YAL_TYPE_H__

#include "yal/yal.h"
#include "yal/enums.h"

namespace yal
{

enum TypeFlags
{
    kTypeFlagIsBuiltinType      = 1 << 0,
    kTypeFlagIsStringType       = 1 << 1,
    kTypeFlagIsFunctionType     = 1 << 2,
    kTypeFlagIsObjectType       = 1 << 3,
    kTypeFlagIsArrayType        = 1 << 4,
    kTypeFlagIsDictionaryType   = 1 << 5,
    kTypeFlagIsInteger          = 1 << 6,
    kTypeFlagIsSignedType       = 1 << 7,
    kTypeFlagIsUnsignedType     = 1 << 8,
    kTypeFlagIsDecimal          = 1 << 9,
    kTypeFlagIsBoolean          = 1 << 10,
    kTypeFlagIsCompoundType     = 1 << 11,
    kTypeFlagIs32BitsSized      = 1 << 12,
    kTypeFlagIs64BitsSized      = 1 << 13,
    kTypeFlagIsPtrSized         = 1 << 14,
    kTypeFlagBooleanPromotable  = 1 << 15,
    kTypeFlagIsVoidType         = 1 << 16,
    kTypeFlagIsStringConstant   = 1 << 17,
    kTypeFlagIsUndefined        = 1 << 18,
    kTypeFlagIsImmutable        = 1 << 19,
    kTypeFlagIsNativeImpl       = 1 << 20
};


// forward declartions
#define YAL_TYPE_LIST_FUNC(CLASS) class CLASS;
#include "yal/types/typelist.h"
#undef YAL_TYPE_LIST_FUNC


class TypeVisitor
{
public:

    virtual ~TypeVisitor() {}

#define YAL_TYPE_LIST_FUNC(CLASS) virtual void visit(const CLASS& type) = 0;
#include "yal/types/typelist.h"
#undef YAL_TYPE_LIST_FUNC

};


union TypeId
{
    struct
    {
        yal_u32 moduleId;
        yal_u32 dataTypeId;
    } split;
    yal_u64 id;

    bool operator == (const TypeId& other) const
    {
        return id == other.id;
    }
};

yal_u32 LanguageModuleId();

yal_u32 LanguageDataTypeId();

TypeId LanguageTypeId();

class Type
{
public:

    typedef yal_u32 Id_t;

    typedef TypeId TypeId_t;

    virtual ~Type() {}

    bool isVoidType() const;

    bool isBuiltinType() const;

    bool isStringType() const;

    bool isFunctionType() const;

    bool isObjectType() const;

    bool isArrayType() const;

    bool isDictionaryType() const;

    bool isInteger() const;

    bool isSignedInteger() const;

    bool isUnsignedInteger() const;

    bool isDecimal() const;

    bool isBoolean() const;

    bool isCompoundType() const;

    bool is32BitSized() const;

    bool is64BitSized() const;

    bool isPointerSized() const;

    bool isPromotableToBoolean() const;

    bool isStringContant() const;

    bool isImmutable() const;

    bool isUndefined() const;

    bool isSignedType() const;

    bool isUnsignedType() const;

    bool isNativeImpl() const;

    bool acceptsOperator(const OperatorType op) const;

    VmType vmType() const
    {
        return _vmType;
    }

    const TypeId_t& typeId() const
    {
        return _typeId;
    }

    Id_t impId() const
    {
        return _impid;
    }

    virtual void accept(TypeVisitor& visitor) = 0;

    virtual const char* typeString() const
    {
        return typeStringImp();
    }

    virtual const char* nativeTypeString() const
    {
        return typeString();
    }

    virtual bool isPromotableTo(const Type* t) const = 0;

protected:

    Type(const TypeId_t& id,
         const VmType vmType,
         const Id_t impId);

private:
    virtual const char* typeStringImp() const = 0;

protected:
    const Id_t _impid;
    const TypeId_t _typeId;
    const VmType _vmType;
    yal_u32 _typeFlags;
    yal_u32 _operatorFlags;

};

#define YAL_TYPE_IMPL_HDR(type) \
    static Id_t ImpId;\
    inline virtual void accept(yal::TypeVisitor& visitor) override \
{ \
    visitor.visit(*this); \
} \
private:    \
    inline virtual const char* typeStringImp() const override\
{\
    return #type; \
} \
public:

#define YAL_TYPLE_IMPL_SRC(type, impid) \
Type::Id_t type::ImpId = impid;

template <class T, class U>
T* cast_type(U* type)
{
    return (type->impId() == T::ImpId) ? static_cast<T*>(type) : nullptr;
}


}
#endif
