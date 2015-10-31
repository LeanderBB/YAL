#ifndef __YAL_STRINGTYPE_H__
#define __YAL_STRINGTYPE_H__

#include "yal/types/type.h"

namespace yal
{

class StringType : public Type
{
public:

    YAL_TYPE_IMPL_HDR(StringType)

    static StringType* GetType();

    StringType();

    virtual ~StringType() {}

    bool isPromotableTo(const Type *t) const override;
};

}
#endif
