#ifndef __YAL_PARSER_STATE_H__
#define __YAL_PARSER_STATE_H__

#include "yal/yal.h"
#include "yal/ast/astbasenode.h"
#include "yal/util/errorhandler.h"
#include "yal/module/module.h"
#include "yal/types/typeregistry.h"
#include "yal/symbols/symboltreebuilder.h"

namespace yal
{

struct ParserState
{
    ParserState(yal::ErrorHandler& errHdl):
        program(),
        errHandler(errHdl),
        module(),
        registry(module),
        symbolTree(errHandler, registry)
    {}

    yal::AstBaseNodeVec_t program;
    yal::ErrorHandler& errHandler;
    yal::Module module;
    yal::TypeRegistry registry;
    yal::SymbolTreeBuilder symbolTree;
};

typedef struct ParserState ParserState_t;

}
#endif
