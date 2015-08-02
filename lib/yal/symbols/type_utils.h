#ifndef __YAL_TYPE_UTILS_H__
#define __YAL_TYPE_UTILS_H__

#include "yal/enums.h"

namespace yal
{

bool
CanTypeBePromoted(const ConstantType from,
                  const ConstantType to);

bool
IsValidBoolean(const ConstantType from);

}

#endif
