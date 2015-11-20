#ifndef __YAL_ANYTYPE_H__
#define __YAL_ANYTYPE_H__

#include "yal/types/type.h"
namespace yal
{

/**
 * @brief Special type used to access VM functions that operate on any composite
 * types without any impac on the underlying type.
 */
class AnyType : public Type
{
public:

    static AnyType* GetType();

    AnyType();


    YAL_TYPE_IMPL_HDR(Antype)


    virtual bool isPromotableTo(const Type*) const override final
    {
        return true;
    }

    virtual const char* typeString() const override final;
};


}
#endif
