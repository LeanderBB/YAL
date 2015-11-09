#include "yal/parser/flex_utils.h"
#include <string.h>
#include <stdio.h>
#include "yal_bison_parser.hpp"
#include "yal/util/str_utils.h"
#include "yal/parser/parser_state.h"
#include "yal/util/inputsink.h"
#include "yal_flex_lexer.h"


int
yy_input_proc(char* buf, int size, yyscan_t yyscanner)
{
    FlexState* flx_state = yyget_extra(yyscanner);
    int rd = static_cast<int>(flx_state->sink->read(buf, size));
    return rd;
}

void
UpdateLineCounter(yyscan_t yyscanner)
{
    FlexState* flx_state = yyget_extra(yyscanner);
    flx_state->curOffset++;
    flx_state->lineOffsets.push_back(flx_state->curOffset);
}

void
UpdateOffset(yyscan_t yyscanner)
{
    FlexState* flx_state = yyget_extra(yyscanner);
    flx_state->curOffset++;
}

namespace yal
{


yal_u32
FlexReadInteger32(union YYSTYPE* token,
                  char* flexText)
{
    if (yal::StrToI32(flexText, &token->integer32))
    {
        return TK_INT32;
    }
    YAL_ASSERT(0 && "Should not be reached");
    return 0;
}

yal_u32
FlexReadUnsignedInteger32(union YYSTYPE* token,
                          char* flexText)
{
    /* remove 'u' from the text */
    size_t len = strlen(flexText);
    flexText[len-1] = '\0';

    if (yal::StrToU32(flexText, &token->uinteger32))
    {
        return TK_UINT32;
    }

    YAL_ASSERT(0 && "Should not be reached");
    return 0;
}

yal_u32
FlexReadInteger64(union YYSTYPE* token,
                  char* flexText)
{
    /* remove 'l' from the text */
    size_t len = strlen(flexText);
    flexText[len-1] = '\0';

    if (yal::StrToI64(flexText, &token->integer64))
    {
        return TK_INT64;
    }

    YAL_ASSERT(0 && "Should not be reached");
    return 0;
}

yal_u32
FlexReadUnsignedInteger64(union YYSTYPE* token,
                          char* flexText)
{
    /* remove 'ul' from the text */
    size_t len = strlen(flexText);
    flexText[len-2] = '\0';

    if (yal::StrToU64(flexText, &token->uinteger64))
    {
        return TK_UINT64;
    }

    YAL_ASSERT(0 && "Should not be reached");
    return 0;
}

yal_u32
FlexReadFloat32(union YYSTYPE* token,
                char* flexText)
{
    if (yal::StrToFloat(flexText, &token->float32))
    {
        return TK_FLT32;
    }

    YAL_ASSERT(0 && "Should not be reached");
    return 0;
}

yal_u32
FlexReadFloat64(union YYSTYPE* token,
                char* flexText)
{
    /* remove 'l' from the text */
    size_t len = strlen(flexText);
    flexText[len-1] = '\0';
    if (yal::StrToDouble(flexText, &token->float64))
    {
        return TK_FLT64;
    }

    YAL_ASSERT(0 && "Should not be reached");
    return 0;
}

yal_u32
FlexReadText(union YYSTYPE* token,
             char* flexText)
{
    size_t len = strlen(flexText);
    flexText[len-1] = '\0';
    token->text = yal::StrCopy(flexText + 1);
    return TK_TEXT;
}

yal_u32
FlexReadBool(union YYSTYPE* token,
             char* flexText)
{
    if (yal::StrToBool(flexText, &token->boolean))
    {
        return TK_BOOL;
    }

    YAL_ASSERT(0 && "Should not be reached");
    return 0;
}

yal_u32
FlexReadId(union YYSTYPE* token,
           char* flexText)
{
    token->text = yal::StrCopy(flexText);
    return TK_ID;
}
}
