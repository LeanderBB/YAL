#ifndef __YAL_TYPE_UTILS_H__
#define __YAL_TYPE_UTILS_H__

#include "yal/enums.h"
#include "yal/symbols/symbol.h"

namespace yal
{

bool
CanTypeBePromoted(const DataType from,
                  const DataType to);

bool
IsValidBoolean(const DataType from);

}

#endif
