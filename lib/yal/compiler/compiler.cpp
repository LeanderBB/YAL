#include "yal/compiler/compiler.h"

#include <iostream>

#include "yal/parser/flex_utils.h"
#include "yal/parser/bison_utils.h"
#include "yal/parser/parser_state.h"
#include "yal_bison_parser.hpp"
#include "yal_flex_lexer.h"
#include "yal/ast/astprinter.h"
#include "yal/symbols/symboltreebuilder.h"
#include "yal/bytecode/bytecodebuilder.h"
#include "yal/util/errorhandler.h"
#include "yal/bytecode/bytecodeprinter.h"
#include "yal/types/typeregistry.h"
namespace yal
{

Compiler::Compiler(InputSink& input,
                   OutputSink &codeOutput):
    _input(input),
    _codeOutput(codeOutput),
    _state()
{

}

bool
Compiler::compile(const uint32_t flags)
{
    // init flex
    yyscan_t scanner;
    struct FlexState flex_state;
    flex_state.lineOffsets.push_back(0);
    flex_state.sink = &_input;
    yylex_init_extra(&flex_state, &scanner);

    // parse with bison
    int parse_result = yyparse(scanner, &_state);

    // destroy flex state
    yylex_destroy(scanner);
    _lineOffsets = std::move(flex_state.lineOffsets);

    if (parse_result == 0)
    {
        _state.symbolTree.process(_state);

        if (flags & kDumpAst)
        {
            yal::AstPrinter printer(std::cout);
            printer.process(_state);
        }

        // remove unused globals and functions
        _state.module.removeUnusedAndAssignIndices();
       // _state.module.logInfo(_output);

        yal::ByteCodeBuilder bt_builder(_codeOutput);
        if (!bt_builder.process(_state)) {return false; }
        return true;
    }
    else
    {
        return false;
    }
}

}

