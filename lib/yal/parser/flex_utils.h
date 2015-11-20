#ifndef __YAL_FLEX_UTILS_H__
#define __YAL_FLEX_UTILS_H__

#include "yal/yal.h"
#include "yal/util/inputsink.h"
#include <vector>
union YYSTYPE;
struct FlexState
{
    FlexState(std::vector<size_t>& lnOfffsets):
        sink(nullptr),
        lineOffsets(lnOfffsets),
        curOffset(0)
    {
    }

    yal::InputSink* sink;
    std::vector<size_t>& lineOffsets;
    size_t curOffset = 0;
};

namespace yal
{



yal_u32
FlexReadInteger32(union YYSTYPE* token,
                  char* flexText);

yal_u32
FlexReadUnsignedInteger32(union YYSTYPE* token,
                          char* flexText);

yal_u32
FlexReadInteger64(union YYSTYPE* token,
                  char* flexText);

yal_u32
FlexReadUnsignedInteger64(union YYSTYPE* token,
                          char* flexText);

yal_u32
FlexReadFloat32(union YYSTYPE* token,
                char* flexText);

yal_u32
FlexReadFloat64(union YYSTYPE* token,
                char* flexText);

yal_u32
FlexReadText(union YYSTYPE* token,
             char* flexText);

yal_u32
FlexReadBool(union YYSTYPE* token,
             char* flexText);

yal_u32
FlexReadId(union YYSTYPE* token,
           char* flexText);

}
#endif
