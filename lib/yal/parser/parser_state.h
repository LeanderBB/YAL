#ifndef __YAL_PARSER_STATE_H__
#define __YAL_PARSER_STATE_H__

#include "yal/yal.h"
#include "yal/ast/astbasenode.h"
#include "yal/module/module.h"
#include "yal/types/typeregistry.h"
#include "yal/symbols/symboltreebuilder.h"

namespace yal
{

class ParserState
{
public:
    ParserState():
        program(),
        module(),
        registry(module),
        symbolTree(registry)
    {}

    yal::StatementNodeVec_t program;
    yal::Module module;
    yal::TypeRegistry registry;
    yal::SymbolTreeBuilder symbolTree;
};

typedef class ParserState ParserState_t;

}
#endif
