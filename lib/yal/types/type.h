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
};


// forward declartions
#define YAL_TYPE_LIST_FUNC(CLASS) class CLASS;
#include "yal/types/typelist.h"
#undef YAL_TYPE_LIST_FUNC


class TypeVisitor
{
public:

    virtual ~TypeVisitor() {}

#define YAL_TYPE_LIST_FUNC(CLASS) virtual void visit(const CLASS& node) = 0;
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
};

bool operator == (const TypeId& id1, const TypeId& id2)
{
    return id1.id == id2.id;
}

yal_u32 LanguageModuleId();

yal_u32 LanguageDataTypeId();

TypeId LanguageTypeId();

class Type
{
public:

    typedef TypeId TypeId_t;

    virtual ~Type() {}

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

    bool isBooleanPromotable() const;

    bool acceptsOperator(const Operator op) const;

    VmType vmType() const
    {
        return _vmType;
    }

    const TypeId_t& typeId() const
    {
        return _typeId;
    }

    virtual void accept(TypeVisitor& visitor) = 0;

    virtual const char* typeString() const = 0;

protected:

    Type(const TypeId_t& id,
         const VmType vmType);

protected:
    const TypeId_t _typeId;
    const VmType _vmType;
    yal_u32 _typeFlags;
    yal_u32 _operatorFlags;

};

#define YAL_TYPE_IMPL_HDR(type) \
    inline virtual void accept(yal::TypeVisitor& visitor) override \
{ \
    visitor.visit(*this); \
} \
    inline virtual const char* typeString() const override\
{\
    return #type; \
}


}
#endif
