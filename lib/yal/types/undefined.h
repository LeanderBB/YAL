#ifndef __YAL_UNDEFINED_H__
#define __YAL_UNDEFINED_H__

#include "yal/types/type.h"
#include "yal/util/stl_utils.h"

namespace yal
{

class UndefinedType : public Type
{
public:

    YAL_TYPE_IMPL_HDR(UndefinedType)

    UndefinedType(const TypeId_t &id,
                  const char* typeString);

    bool isPromotableTo(const Type*) const override
    {
        return false;
    }

    const char* undefinedTypeString() const
    {
        return _typeString.c_str();
    }


private:
    const std::string _typeString;
};

}
#endif
