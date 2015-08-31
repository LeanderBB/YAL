#include "yal/parser/bison_utils.h"
#include "yal/parser/parser_state.h"
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "yal_bison_parser.hpp"
#include "yal/util/outputformater.h"


void yyerror(YYLTYPE* location,
             void* scanner,
             yal::ParserState_t* state,
             const char* s)
{
    (void) scanner;
    (void) state;
    yal::OutputFormater formater;

    yal::SourceLocationInfo loc_info = yal::BisonYyltypeToLocation(*location);
    formater.format("%s\n",s);
    state->errHandler.onCompileError(formater.str(), formater.strLen(), loc_info);
}


namespace yal
{

SourceLocationInfo
BisonYyltypeToLocation(const YYLTYPE& loc)
{
    SourceLocationInfo yalloc;
    yalloc.firstLine = loc.first_line;
    yalloc.lastLine = loc.last_line;
    yalloc.firstColumn = loc.first_column;
    yalloc.lastColumn = loc.last_column;
    return yalloc;
}

void
BisonOnFunctionDecl(ParserState_t* state,
                    const char* funcName)
{
    (void) state;
    printf("FuncDecl: %s\n", funcName);
}

void
BisonOnFunctionArgs(ParserState_t* state)
{
    (void) state;
    printf("FuncArgs\n");
}

void
BisonOnVarDecl(ParserState_t* state,
               const char* varName)
{
    (void) state;
    printf("VarDecl: %s\n", varName);
}

void
BisonOnIf(ParserState_t* state)
{
    (void) state;
    printf("If\n");
}

void
BisonOnElseIf(ParserState_t* state)
{
    (void) state;
    printf("Elif\n");
}

void
BisonOnElse(ParserState_t* state)
{
    (void) state;
    printf("Else\n");
}

void
BisonOnEndIf(ParserState_t* state)
{
    (void) state;
    printf("Endif\n");
}

void
BisonOnConstantFloat32(ParserState_t* state,
                       const yal_f32 val)
{
    (void) state;
    printf("Constant (f32): %f\n", val);
}

void
BisonOnConstantFloat64(ParserState_t* state,
                       const yal_f64 val)
{
    (void) state;
    printf("Constant (f64): %lf\n", val);
}

void
BisonOnConstantUInt32(ParserState_t* state,
                      const yal_u32 val)
{
    (void) state;
    printf("Constant (u32): %u\n", val);
}

void
BisonOnConstantUInt64(ParserState_t* state,
                      const yal_u64 val)
{
    (void) state;
    printf("Constant (u64): %" PRIu64 "\n", val);
}

void
BisonOnConstantInt32(ParserState_t* state,
                     const yal_i32 val)
{
    (void) state;
    printf("Constant (i32): %d\n", val);
}

void
BisonOnConstantInt64(ParserState_t* state,
                     const yal_i64 val)
{
    (void) state;
    printf("Constant (i64): %" PRIi64 "\n", val);
}

void
BisonOnConstantBool(ParserState_t* state,
                    const yal_bool val)
{
    (void) state;
    printf("Constant (bool): %d\n", val);
}

void
BisonOnConstantId(ParserState_t* state,
                  const char* val)
{
    (void) state;
    printf("Constant (id): %s\n", val);
}

void
BisonOnExpAssign(ParserState_t* state,
                 const char* val)
{
    (void) state;
    printf("ExpAssign: %s\n", val);
}

void
BisonOnAssignOp(ParserState_t* state,
                const AssignOperatorType op)
{
    (void) state;
    printf("AssignOp: %x(%s)\n", op, AssignOperatorTypeToSt(op));
}

void
BisonOnStatReturn(ParserState_t* state)
{
    (void) state;
    printf("return\n");
}

void
BisonOnFuncCall(ParserState_t* state,
                const char* val)
{
    (void) state;
    printf("Func call: %s\n", val);
}

void
BisonOnCallArg(ParserState_t* state)
{
    (void) state;
    printf("Call Arg\n");
}

void
BisonOnDeclArg(ParserState_t* state)
{
    (void) state;
    printf("Decl Arg\n");
}

void
BisonOnDeclArgCustom(ParserState_t* state,
                     const char* id)
{
    (void) state;
    printf("Decl Arg Custom: %s\n", id);
}

void
BisonOnDeclArgType(ParserState_t* state,
                   const ConstantType type)
{
    (void) state;
    printf("Decl Arg Type: %x (%s)\n", type, ConstantTypeToStr(type));
}

void
BisonOnDeclArgName(ParserState_t* state,
                   const char* name)
{
    (void) state;
    printf("Decl Arg Name: %s\n", name);
}

void
BisonOnExpSingleArit(ParserState_t* state,
                     const SingleOperatorType op)
{
    (void) state;
    printf("Exp Single Arit: %x(%s)\n", op, SingleOperatorTypeToStr(op));
}

void
BisonOnExpDualArit(ParserState_t* state,
                   const DualOperatorType op)
{
    (void) state;
    printf("Exp Dual Arit: %x(%s)\n", op, DualOperatorTypeToStr(op));
}

void
BisonOnExpCompare(ParserState_t* state,
                  const CompareOperatorType op)
{
    (void) state;
    printf("Exp Compare: %x(%s)\n", op, CompareOperatorToStr(op));
}

void
BisonOnScopeBegin(ParserState_t* state)
{
    (void) state;
    printf("> Scope Begin\n");
}

void
BisonOnScopeEnd(ParserState_t* state)
{
    (void) state;
    printf("> Scope End\n");
}

void
BisonOnDeclArgBegin(ParserState_t* state)
{
    (void) state;
    printf("> Decl Arg Begin\n");
}

void
BisonOnDeclArgEnd(ParserState_t* state)
{
    (void) state;
    printf("> Decl Arg End\n");
}

void
BisonOnCallArgBegin(ParserState_t* state)
{
    (void) state;
    printf("> Call Arg Begin\n");
}

void
BisonOnCallArgEnd(ParserState_t* state)
{
    (void) state;
    printf("> Call Arg End\n");
}

}
