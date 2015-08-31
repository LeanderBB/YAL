#ifndef __YAL_BYTECODEBUILDER_H__
#define __YAL_BYTECODEBUILDER_H__

#include "yal/yal.h"
#include "yal/ast/astbasenode.h"
#include "yal/util/outputsink.h"
#include "yal/util/errorhandler.h"
#include "yal/parser/parser_state.h"

namespace yal
{

class SymbolTreeBuilder;
class ByteCodeBuilder
{
public:
    ByteCodeBuilder(OutputSink &codeOutput,
                    ErrorHandler& errorHandler);

    ~ByteCodeBuilder();

    bool process(ParserState_t& state);


protected:

    bool writeModuleInfo(ParserState& state);

private:
    OutputSink& _codeOutput;
    ErrorHandler& _errorHandler;
};

}

#endif
