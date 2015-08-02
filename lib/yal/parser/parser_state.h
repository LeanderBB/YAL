#ifndef __YAL_PARSER_STATE_H__
#define __YAL_PARSER_STATE_H__

#include "yal/yal.h"
#include "yal/ast/astbasenode.h"
#include "yal/util/errorhandler.h"
#include "yal/module/module.h"
namespace yal
{

struct ParserState
{
    ParserState(yal::ErrorHandler& errHdl):
        program(),
        errHandler(errHdl)
    {}

    yal::AstBaseNodeVec_t program;
    yal::ErrorHandler& errHandler;
    yal::Module module;
};

typedef struct ParserState ParserState_t;

}
#endif
