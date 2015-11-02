#ifndef __YAL_COMPILER_H__
#define __YAL_COMPILER_H__

#include "yal/yal.h"
#include "yal/util/inputsink.h"
#include "yal/util/outputsink.h"
#include "yal/util/errorhandler.h"
#include "yal/parser/parser_state.h"

namespace yal
{

class Compiler
{
public:

    enum
    {
        kDumpAst = 1 << 0
    };


    Compiler(InputSink& input,
             OutputSink& codeOutput);

    bool compile(const uint32_t flags = 0);

protected:
    InputSink& _input;
    OutputSink& _codeOutput;
    ParserState _state;
};

}
#endif
