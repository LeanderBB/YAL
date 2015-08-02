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
    Compiler(InputSink& input,
             OutputSink& output,
             OutputSink& codeOutput,
             ErrorHandler& errHandler);

    bool compile();

protected:
    InputSink& _input;
    OutputSink& _output;
    OutputSink& _codeOutput;
    ErrorHandler& _errHandler;
    ParserState _state;
};

}
#endif
