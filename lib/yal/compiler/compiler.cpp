#include "yal/compiler/compiler.h"
#include "yal/parser/flex_utils.h"
#include "yal/parser/bison_utils.h"
#include "yal/parser/parser_state.h"
#include "yal_bison_parser.hpp"
#include "yal_flex_lexer.h"
#include "yal/ast/astprinter.h"
#include "yal/symbols/symboltreebuilder.h"
#include "yal/bytecode/bytecodebuilder.h"
#include "yal/util/errorhandler.h"
#include "yal/util/outputformater.h"
#include "yal/bytecode/bytecodeprinter.h"
#include "yal/types/typeregistry.h"
namespace yal
{

Compiler::Compiler(InputSink& input,
                   OutputSink& output,
                   OutputSink &codeOutput,
                   ErrorHandler& errHandler):
    _input(input),
    _output(output),
    _codeOutput(codeOutput),
    _errHandler(errHandler),
    _state(_errHandler)
{

}

bool
Compiler::compile(const uint32_t flags)
{
    // init flex
    yyscan_t scanner;
    struct FlexState flex_state;
    flex_state.sink = &_input;
    yylex_init_extra(&flex_state, &scanner);

    // parse with bison
    int parse_result = yyparse(scanner, &_state);

    // destroy flex state
    yylex_destroy(scanner);

    if (parse_result == 0)
    {
        if (flags & kDumpAst)
        {
            yal::AstPrinter printer(_output);
            printer.process(_state);
        }

        yal::SymbolTreeBuilder sym_builder(_errHandler, _state.registry);
        if(!sym_builder.process(_state))
        {
            return false;
        }

        // remove unused globals and functions
        _state.module.removeUnusedAndAssignIndices();
       // _state.module.logInfo(_output);

        yal::ByteCodeBuilder bt_builder(_codeOutput, _errHandler);
        if (!bt_builder.process(_state)) {return false; }
        return true;
    }
    else
    {
        return false;
    }
}

}

