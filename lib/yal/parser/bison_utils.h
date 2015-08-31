#ifndef __YAL_BISON_UTILS_H__
#define __YAL_BISON_UTILS_H__

#include "yal/yal.h"
#include "yal/parser/parser_state.h"
#include "yal/enums.h"
#include "yal/util/errorhandler.h"


struct YYLTYPE;

namespace yal
{

SourceLocationInfo
BisonYyltypeToLocation(const YYLTYPE& loc);

void
BisonOnFunctionDecl(ParserState_t* state,
                    const char* funcName);

void
BisonOnFunctionArgs(ParserState_t* state);

void
BisonOnVarDecl(ParserState_t* state,
               const char* varName);

void
BisonOnIf(ParserState_t* state);

void
BisonOnElseIf(ParserState_t* state);

void
BisonOnElse(ParserState_t* state);

void
BisonOnEndIf(ParserState_t* state);

void
BisonOnConstantFloat32(ParserState_t* state,
                       const yal_f32 val);

void
BisonOnConstantFloat64(ParserState_t* state,
                       const yal_f64 val);

void
BisonOnConstantUInt32(ParserState_t* state,
                      const yal_u32 val);

void
BisonOnConstantUInt64(ParserState_t* state,
                      const yal_u64 val);

void
BisonOnConstantInt32(ParserState_t* state,
                     const yal_i32 val);

void
BisonOnConstantInt64(ParserState_t* state,
                     const yal_i64 val);

void
BisonOnConstantBool(ParserState_t* state,
                    const yal_bool val);

void
BisonOnConstantId(ParserState_t* state,
                  const char* id);

void
BisonOnExpAssign(ParserState_t* state,
                 const char* val);

void
BisonOnAssignOp(ParserState_t* state,
                const AssignOperatorType op);

void
BisonOnStatReturn(ParserState_t* state);

void
BisonOnFuncCall(ParserState_t* state,
                const char* val);

void
BisonOnCallArg(ParserState_t* state);

void
BisonOnDeclArg(ParserState_t* state);

void
BisonOnDeclArgCustom(ParserState_t* state,
                     const char* id);

void
BisonOnDeclArgType(ParserState_t* state,
                   const ConstantType type);

void
BisonOnDeclArgName(ParserState_t* state,
                   const char* name);

void
BisonOnExpSingleArit(ParserState_t* state,
                     const SingleOperatorType);

void
BisonOnExpDualArit(ParserState_t* state,
                   const DualOperatorType);

void
BisonOnExpCompare(ParserState_t* state,
                  const CompareOperatorType);

void
BisonOnScopeBegin(ParserState_t* state);

void
BisonOnScopeEnd(ParserState_t* state);

void
BisonOnDeclArgBegin(ParserState_t* state);

void
BisonOnDeclArgEnd(ParserState_t* state);

void
BisonOnCallArgBegin(ParserState_t* state);

void
BisonOnCallArgEnd(ParserState_t* state);

}

#endif
