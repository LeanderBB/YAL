/*
** 2000-05-29
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** Driver template for the LEMON parser generator.
**
** The "lemon" program processes an LALR(1) input grammar file, then uses
** this template to construct a parser.  The "lemon" program inserts text
** at each "%%" line.  Also, any "P-a-r-s-e" identifer prefix (without the
** interstitial "-" characters) contained in this template is changed into
** the value of the %name directive from the grammar.  Otherwise, the content
** of this template is copied straight through into the generate parser
** source file.
**
** The following is the concatenation of all %include directives from the
** input grammar file:
*/
#include <stdio.h>
/************ Begin %include sections from the grammar ************************/

#include <cassert>
#include <vector>
#include "yal/parser/parser.h"
#include "yal/util/log.h"
#include "yal/util/stringref.h"
#include "yal/ast/typebuiltin.h"
#include "yal/ast/declfunction.h"
#include "yal/ast/decltypefunction.h"
#include "yal/ast/declmodule.h"
#include "yal/ast/reftypebuiltin.h"
#include "yal/ast/reftypeidentifier.h"
#include "yal/ast/declvar.h"
#include "yal/ast/declparamvar.h"
#include "yal/ast/stmtexpression.h"
#include "yal/ast/stmtreturn.h"
#include "yal/ast/exprunaryoperator.h"
#include "yal/ast/exprbinaryoperator.h"
#include "yal/ast/statement.h"
#include "yal/ast/statementlist.h"
#include "yal/ast/exprintegerliteral.h"
#include "yal/ast/stmtreturn.h"
#include "yal/ast/exprboolliteral.h"
#include "yal/ast/stmtdecl.h"
#include "yal/lexer/lexer.h"
#include "yal/ast/stmtassign.h"
#include "yal/ast/exprvarref.h"
#include "yal/ast/declstruct.h"
#define YYMALLOCARGTYPE size_t
#include "parserimpl.h"
/**************** End of %include directives **********************************/
/* These constants specify the various numeric values for terminal symbols
** in a format understandable to "makeheaders".  This section is blank unless
** "lemon" is run with the "-m" command-line option.
***************** Begin makeheaders token definitions *************************/
#if INTERFACE
#define YAL_TOKEN_ASSIGN                          1
#define YAL_TOKEN_OR                              2
#define YAL_TOKEN_AND                             3
#define YAL_TOKEN_BIT_OR                          4
#define YAL_TOKEN_BIT_XOR                         5
#define YAL_TOKEN_BIT_AND                         6
#define YAL_TOKEN_EQ                              7
#define YAL_TOKEN_NE                              8
#define YAL_TOKEN_GT                              9
#define YAL_TOKEN_LT                             10
#define YAL_TOKEN_GE                             11
#define YAL_TOKEN_LE                             12
#define YAL_TOKEN_PLUS                           13
#define YAL_TOKEN_MINUS                          14
#define YAL_TOKEN_MULT                           15
#define YAL_TOKEN_DIV                            16
#define YAL_TOKEN_MOD                            17
#define YAL_TOKEN_NEGATE                         18
#define YAL_TOKEN_BIT_NOT                        19
#define YAL_TOKEN_NOT                            20
#define YAL_TOKEN_DOT                            21
#define YAL_TOKEN_END                            22
#define YAL_TOKEN_IDENTIFIER                     23
#define YAL_TOKEN_TYPE_BOOL                      24
#define YAL_TOKEN_TYPE_INT8                      25
#define YAL_TOKEN_TYPE_UINT8                     26
#define YAL_TOKEN_TYPE_INT16                     27
#define YAL_TOKEN_TYPE_UINT16                    28
#define YAL_TOKEN_TYPE_INT32                     29
#define YAL_TOKEN_TYPE_UINT32                    30
#define YAL_TOKEN_TYPE_INT64                     31
#define YAL_TOKEN_TYPE_UINT64                    32
#define YAL_TOKEN_TYPE_FLOAT                     33
#define YAL_TOKEN_TYPE_DOUBLE                    34
#define YAL_TOKEN_ARRAY_BEGIN                    35
#define YAL_TOKEN_ARRAY_END                      36
#define YAL_TOKEN_TYPE                           37
#define YAL_TOKEN_COLON                          38
#define YAL_TOKEN_STRUCT                         39
#define YAL_TOKEN_SCOPE_BEGIN                    40
#define YAL_TOKEN_SCOPE_END                      41
#define YAL_TOKEN_SEMI_COLON                     42
#define YAL_TOKEN_FUNCTION                       43
#define YAL_TOKEN_PAR_BEGIN                      44
#define YAL_TOKEN_PAR_END                        45
#define YAL_TOKEN_COMMA                          46
#define YAL_TOKEN_RETURN                         47
#define YAL_TOKEN_VAR                            48
#define YAL_TOKEN_LET                            49
#define YAL_TOKEN_INTEGER_LITERAL                50
#define YAL_TOKEN_DECIMAL_LITERAL                51
#define YAL_TOKEN_BOOL_LITERAL                   52
#endif
/**************** End makeheaders token definitions ***************************/

/* The next sections is a series of control #defines.
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used to store the integer codes
**                       that represent terminal and non-terminal symbols.
**                       "unsigned char" is used if there are fewer than
**                       256 symbols.  Larger types otherwise.
**    YYNOCODE           is a number of type YYCODETYPE that is not used for
**                       any terminal or nonterminal symbol.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       (also known as: "terminal symbols") have fall-back
**                       values which should be used if the original symbol
**                       would not parse.  This permits keywords to sometimes
**                       be used as identifiers, for example.
**    YYACTIONTYPE       is the data type used for "action codes" - numbers
**                       that indicate what to do in response to the next
**                       token.
**    YALParserTOKENTYPE     is the data type used for minor type for terminal
**                       symbols.  Background: A "minor type" is a semantic
**                       value associated with a terminal or non-terminal
**                       symbols.  For example, for an "ID" terminal symbol,
**                       the minor type might be the name of the identifier.
**                       Each non-terminal can have a different minor type.
**                       Terminal symbols all have the same minor type, though.
**                       This macros defines the minor type for terminal 
**                       symbols.
**    YYMINORTYPE        is the data type used for all minor types.
**                       This is typically a union of many types, one of
**                       which is YALParserTOKENTYPE.  The entry in the union
**                       for terminal symbols is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    YALParserARG_SDECL     A static variable declaration for the %extra_argument
**    YALParserARG_PDECL     A parameter declaration for the %extra_argument
**    YALParserARG_STORE     Code to store %extra_argument into yypParser
**    YALParserARG_FETCH     Code to extract %extra_argument from yypParser
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YY_MAX_SHIFT       Maximum value for shift actions
**    YY_MIN_SHIFTREDUCE Minimum value for shift-reduce actions
**    YY_MAX_SHIFTREDUCE Maximum value for shift-reduce actions
**    YY_MIN_REDUCE      Maximum value for reduce actions
**    YY_ERROR_ACTION    The yy_action[] code for syntax error
**    YY_ACCEPT_ACTION   The yy_action[] code for accept
**    YY_NO_ACTION       The yy_action[] code for no-op
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/************* Begin control #defines *****************************************/
#define YYCODETYPE unsigned char
#define YYNOCODE 78
#define YYACTIONTYPE unsigned char
#if INTERFACE
#define YALParserTOKENTYPE yal::TokenInfo
#endif
typedef union {
  int yyinit;
  YALParserTOKENTYPE yy0;
  yal::ExprUnaryOperator* yy25;
  yal::Statement* yy38;
  yal::DeclStruct* yy44;
  yal::StmtDecl* yy59;
  yal::DeclModule* yy63;
  yal::StatementList* yy66;
  yal::DeclParamVar* yy87;
  yal::RefTypeBuiltin* yy111;
  yal::DeclTypeFunction* yy113;
  yal::DeclFunction* yy115;
  yal::RefType* yy116;
  yal::ExprBinaryOperator* yy125;
  yal::StringRefPod yy142;
  yal::DeclVar* yy146;
  yal::StmtExpression* yy147;
  yal::DeclParamVarContainer* yy150;
  yal::DeclStructMembers* yy151;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#if INTERFACE
#define YALParserARG_SDECL  yal::Parser *pParser ;
#define YALParserARG_PDECL , yal::Parser *pParser 
#define YALParserARG_FETCH  yal::Parser *pParser  = yypParser->pParser 
#define YALParserARG_STORE yypParser->pParser  = pParser 
#endif
#define YYNSTATE             70
#define YYNRULE              64
#define YY_MAX_SHIFT         69
#define YY_MIN_SHIFTREDUCE   122
#define YY_MAX_SHIFTREDUCE   185
#define YY_MIN_REDUCE        186
#define YY_MAX_REDUCE        249
#define YY_ERROR_ACTION      250
#define YY_ACCEPT_ACTION     251
#define YY_NO_ACTION         252
/************* End control #defines *******************************************/

/* Define the yytestcase() macro to be a no-op if is not already defined
** otherwise.
**
** Applications can choose to define yytestcase() in the %include section
** to a macro that can assist in verifying code coverage.  For production
** code the yytestcase() macro should be turned off.  But it is useful
** for testing.
*/
#ifndef yytestcase
# define yytestcase(X)
#endif


/* Next are the tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N <= YY_MAX_SHIFT             Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   N between YY_MIN_SHIFTREDUCE       Shift to an arbitrary state then
**     and YY_MAX_SHIFTREDUCE           reduce by rule N-YY_MIN_SHIFTREDUCE.
**
**   N between YY_MIN_REDUCE            Reduce by rule N-YY_MIN_REDUCE
**     and YY_MAX_REDUCE
**
**   N == YY_ERROR_ACTION               A syntax error has occurred.
**
**   N == YY_ACCEPT_ACTION              The parser accepts its input.
**
**   N == YY_NO_ACTION                  No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as either:
**
**    (A)   N = yy_action[ yy_shift_ofst[S] + X ]
**    (B)   N = yy_default[S]
**
** The (A) formula is preferred.  The B formula is used instead if:
**    (1)  The yy_shift_ofst[S]+X value is out of range, or
**    (2)  yy_lookahead[yy_shift_ofst[S]+X] is not equal to X, or
**    (3)  yy_shift_ofst[S] equal YY_SHIFT_USE_DFLT.
** (Implementation note: YY_SHIFT_USE_DFLT is chosen so that
** YY_SHIFT_USE_DFLT+X will be out of range for all possible lookaheads X.
** Hence only tests (1) and (2) need to be evaluated.)
**
** The formulas above are for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array and YY_REDUCE_USE_DFLT is used in place of
** YY_SHIFT_USE_DFLT.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
**
*********** Begin parsing tables **********************************************/
#define YY_ACTTAB_COUNT (305)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */   128,  129,  130,  131,  132,  133,  134,  135,  136,  137,
 /*    10 */   138,  139,    7,  187,  188,  206,   64,    8,    9,   25,
 /*    20 */   251,  170,   63,   69,    7,   38,  232,  233,  231,    8,
 /*    30 */     9,  186,   54,  170,   42,  232,  233,  231,   68,  153,
 /*    40 */   140,   63,   16,    1,   23,    3,   58,   56,  181,  185,
 /*    50 */   182,  154,   48,  212,   16,   49,  212,    3,   58,   56,
 /*    60 */   181,  185,  182,  128,  129,  130,  131,  132,  133,  134,
 /*    70 */   135,  136,  137,  138,  139,   60,  129,  130,  131,  132,
 /*    80 */   133,  134,  135,  136,  137,  138,  139,    7,    6,   15,
 /*    90 */    17,   66,    8,    9,  207,   29,  170,   61,   31,   32,
 /*   100 */    14,   13,   12,   11,   10,    7,   21,   15,   17,  210,
 /*   110 */     8,    9,  211,  191,  170,  161,   30,   16,   14,   13,
 /*   120 */    12,   11,   10,  181,  185,  182,   12,   11,   10,  159,
 /*   130 */    43,  232,  233,  231,   22,   16,   55,   57,   15,   17,
 /*   140 */   209,  181,  185,  182,   15,   17,   26,  160,   66,   14,
 /*   150 */    13,   12,   11,   10,  214,   14,   13,   12,   11,   10,
 /*   160 */    15,   17,    2,   35,  220,   37,  232,  233,  231,   59,
 /*   170 */   191,   14,   13,   12,   11,   10,   36,   32,  157,   51,
 /*   180 */    50,   15,   17,   39,  232,  233,  231,  166,   52,   33,
 /*   190 */    20,   17,   14,   13,   12,   11,   10,   34,    4,   27,
 /*   200 */   144,   14,   13,   12,   11,   10,  219,   37,  232,  233,
 /*   210 */   231,   59,    5,  158,   14,   13,   12,   11,   10,  244,
 /*   220 */   232,  233,  231,  141,  243,  232,  233,  231,  242,  232,
 /*   230 */   233,  231,   18,  241,  232,  233,  231,  240,  232,  233,
 /*   240 */   231,  239,  232,  233,  231,   24,  184,   65,   28,  247,
 /*   250 */    46,  232,  233,  231,   47,  232,  233,  231,   19,   67,
 /*   260 */    44,  232,  233,  231,   40,  232,  233,  231,  188,  188,
 /*   270 */    45,  232,  233,  231,   41,  232,  233,  231,  188,   66,
 /*   280 */    66,   66,   66,  188,  188,  215,  228,   53,   62,  188,
 /*   290 */   188,  188,  188,  188,  188,  188,  188,  188,  188,  188,
 /*   300 */   188,  191,  191,  191,  191,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    23,   24,   25,   26,   27,   28,   29,   30,   31,   32,
 /*    10 */    33,   34,   14,   55,   56,   74,   39,   19,   20,   57,
 /*    20 */    58,   23,   23,   22,   14,   68,   69,   70,   71,   19,
 /*    30 */    20,   73,   23,   23,   68,   69,   70,   71,   37,   41,
 /*    40 */    41,   23,   44,   40,   43,   47,   48,   49,   50,   51,
 /*    50 */    52,   41,   63,   64,   44,   63,   64,   47,   48,   49,
 /*    60 */    50,   51,   52,   23,   24,   25,   26,   27,   28,   29,
 /*    70 */    30,   31,   32,   33,   34,   23,   24,   25,   26,   27,
 /*    80 */    28,   29,   30,   31,   32,   33,   34,   14,    1,    2,
 /*    90 */     3,   54,   19,   20,   74,   75,   23,   60,   45,   46,
 /*   100 */    13,   14,   15,   16,   17,   14,   38,    2,    3,   66,
 /*   110 */    19,   20,   64,   76,   23,   42,   59,   44,   13,   14,
 /*   120 */    15,   16,   17,   50,   51,   52,   15,   16,   17,   42,
 /*   130 */    68,   69,   70,   71,   38,   44,   61,   61,    2,    3,
 /*   140 */    66,   50,   51,   52,    2,    3,   44,   42,   54,   13,
 /*   150 */    14,   15,   16,   17,   60,   13,   14,   15,   16,   17,
 /*   160 */     2,    3,   65,   59,   67,   68,   69,   70,   71,   72,
 /*   170 */    76,   13,   14,   15,   16,   17,   45,   46,   42,   38,
 /*   180 */    23,    2,    3,   68,   69,   70,   71,   45,   38,   23,
 /*   190 */    38,    3,   13,   14,   15,   16,   17,   23,    1,   44,
 /*   200 */    42,   13,   14,   15,   16,   17,   67,   68,   69,   70,
 /*   210 */    71,   72,    1,   42,   13,   14,   15,   16,   17,   68,
 /*   220 */    69,   70,   71,   42,   68,   69,   70,   71,   68,   69,
 /*   230 */    70,   71,    1,   68,   69,   70,   71,   68,   69,   70,
 /*   240 */    71,   68,   69,   70,   71,   38,   36,   35,   40,    0,
 /*   250 */    68,   69,   70,   71,   68,   69,   70,   71,   38,   23,
 /*   260 */    68,   69,   70,   71,   68,   69,   70,   71,   77,   77,
 /*   270 */    68,   69,   70,   71,   68,   69,   70,   71,   77,   54,
 /*   280 */    54,   54,   54,   77,   77,   60,   60,   60,   60,   77,
 /*   290 */    77,   77,   77,   77,   77,   77,   77,   77,   77,   77,
 /*   300 */    77,   76,   76,   76,   76,
};
#define YY_SHIFT_USE_DFLT (305)
#define YY_SHIFT_COUNT    (69)
#define YY_SHIFT_MIN      (-23)
#define YY_SHIFT_MAX      (249)
static const short yy_shift_ofst[] = {
 /*     0 */   305,   -2,   10,   73,   91,   91,   91,   91,   91,   91,
 /*    10 */    91,   91,   91,   91,   91,   91,   91,   91,   91,  -23,
 /*    20 */    40,   40,   40,   52,   40,    1,    9,    9,   18,   -1,
 /*    30 */     3,   68,    9,   96,   96,    3,   68,   87,  105,  136,
 /*    40 */   142,  158,  179,  179,  188,  201,  111,  111,   53,  131,
 /*    50 */   102,  157,  141,  150,  152,  197,  166,  211,  174,  171,
 /*    60 */   155,  181,  231,  207,  208,  210,  212,  220,  236,  249,
};
#define YY_REDUCE_USE_DFLT (-60)
#define YY_REDUCE_COUNT (36)
#define YY_REDUCE_MIN   (-59)
#define YY_REDUCE_MAX   (228)
static const short yy_reduce_ofst[] = {
 /*     0 */   -38,   97,  139,  -43,  -34,   62,  115,  151,  156,  160,
 /*    10 */   165,  169,  173,  182,  186,  192,  196,  202,  206,   37,
 /*    20 */    94,  225,  226,  227,  228,  -42,  -11,   -8,   20,  -59,
 /*    30 */    43,   57,   48,   75,   76,   74,  104,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   189,  250,  250,  250,  250,  250,  250,  250,  250,  250,
 /*    10 */   250,  250,  250,  250,  250,  250,  250,  250,  250,  250,
 /*    20 */   250,  250,  250,  250,  250,  250,  213,  213,  250,  250,
 /*    30 */   250,  216,  250,  229,  229,  250,  216,  250,  250,  250,
 /*    40 */   250,  250,  227,  226,  236,  235,  238,  237,  250,  250,
 /*    50 */   250,  250,  250,  250,  250,  250,  250,  250,  250,  250,
 /*    60 */   192,  250,  250,  250,  250,  250,  190,  250,  250,  250,
};
/********** End of lemon-generated parsing tables *****************************/

/* The next table maps tokens (terminal symbols) into fallback tokens.  
** If a construct like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammar, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
**
** This feature can be used, for example, to cause some keywords in a language
** to revert to identifiers if they keyword does not apply in the context where
** it appears.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
**
** After the "shift" half of a SHIFTREDUCE action, the stateno field
** actually contains the reduce action for the second half of the
** SHIFTREDUCE.
*/
struct yyStackEntry {
  YYACTIONTYPE stateno;  /* The state-number, or reduce action in SHIFTREDUCE */
  YYCODETYPE major;      /* The major token value.  This is the code
                         ** number for the token at this stack level */
  YYMINORTYPE minor;     /* The user-supplied minor token value.  This
                         ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  yyStackEntry *yytos;          /* Pointer to top element of the stack */
#ifdef YYTRACKMAXSTACKDEPTH
  int yyhwm;                    /* High-water mark of the stack */
#endif
#ifndef YYNOERRORRECOVERY
  int yyerrcnt;                 /* Shifts left before out of the error */
#endif
  YALParserARG_SDECL                /* A place to hold %extra_argument */
#if YYSTACKDEPTH<=0
  int yystksz;                  /* Current side of the stack */
  yyStackEntry *yystack;        /* The parser's stack */
  yyStackEntry yystk0;          /* First stack entry */
#else
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
  yyStackEntry *yystackEnd;            /* Last entry in the stack */
#endif
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void YALParserTrace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *const yyTokenName[] = { 
  "$",             "ASSIGN",        "OR",            "AND",         
  "BIT_OR",        "BIT_XOR",       "BIT_AND",       "EQ",          
  "NE",            "GT",            "LT",            "GE",          
  "LE",            "PLUS",          "MINUS",         "MULT",        
  "DIV",           "MOD",           "NEGATE",        "BIT_NOT",     
  "NOT",           "DOT",           "END",           "IDENTIFIER",  
  "TYPE_BOOL",     "TYPE_INT8",     "TYPE_UINT8",    "TYPE_INT16",  
  "TYPE_UINT16",   "TYPE_INT32",    "TYPE_UINT32",   "TYPE_INT64",  
  "TYPE_UINT64",   "TYPE_FLOAT",    "TYPE_DOUBLE",   "ARRAY_BEGIN", 
  "ARRAY_END",     "TYPE",          "COLON",         "STRUCT",      
  "SCOPE_BEGIN",   "SCOPE_END",     "SEMI_COLON",    "FUNCTION",    
  "PAR_BEGIN",     "PAR_END",       "COMMA",         "RETURN",      
  "VAR",           "LET",           "INTEGER_LITERAL",  "DECIMAL_LITERAL",
  "BOOL_LITERAL",  "error",         "type_builtin",  "decl_function",
  "type_function_decl",  "decls",         "module",        "function_return_decl",
  "type_specifier",  "var_type_spec",  "named_decl",    "function_args_decl",
  "function_arg_decl",  "statement_list",  "function_scope",  "statement",   
  "expression",    "unaryexp",      "binaryexp",     "literal",     
  "var_decl",      "type_decl",     "type_var_decl",  "type_var_decls",
  "type_array",  
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "decls ::= decls type_decl",
 /*   1 */ "decls ::= decls decl_function",
 /*   2 */ "decls ::= decls type_function_decl",
 /*   3 */ "decls ::=",
 /*   4 */ "type_specifier ::= type_builtin",
 /*   5 */ "type_specifier ::= type_array",
 /*   6 */ "type_specifier ::= IDENTIFIER",
 /*   7 */ "type_builtin ::= TYPE_BOOL",
 /*   8 */ "type_builtin ::= TYPE_INT8",
 /*   9 */ "type_builtin ::= TYPE_UINT8",
 /*  10 */ "type_builtin ::= TYPE_INT16",
 /*  11 */ "type_builtin ::= TYPE_UINT16",
 /*  12 */ "type_builtin ::= TYPE_INT32",
 /*  13 */ "type_builtin ::= TYPE_UINT32",
 /*  14 */ "type_builtin ::= TYPE_INT64",
 /*  15 */ "type_builtin ::= TYPE_UINT64",
 /*  16 */ "type_builtin ::= TYPE_FLOAT",
 /*  17 */ "type_builtin ::= TYPE_DOUBLE",
 /*  18 */ "type_decl ::= TYPE IDENTIFIER COLON STRUCT SCOPE_BEGIN type_var_decls SCOPE_END",
 /*  19 */ "type_decl ::= TYPE IDENTIFIER COLON type_specifier SEMI_COLON",
 /*  20 */ "type_var_decls ::= type_var_decls type_var_decl",
 /*  21 */ "type_var_decls ::= type_var_decl",
 /*  22 */ "type_var_decl ::= IDENTIFIER COLON type_specifier ASSIGN expression SEMI_COLON",
 /*  23 */ "decl_function ::= FUNCTION IDENTIFIER PAR_BEGIN function_args_decl PAR_END function_return_decl function_scope",
 /*  24 */ "type_function_decl ::= FUNCTION type_specifier COLON COLON IDENTIFIER PAR_BEGIN function_args_decl PAR_END function_return_decl function_scope",
 /*  25 */ "function_args_decl ::= function_args_decl COMMA function_arg_decl",
 /*  26 */ "function_args_decl ::= function_arg_decl",
 /*  27 */ "function_args_decl ::=",
 /*  28 */ "function_arg_decl ::= IDENTIFIER COLON type_specifier",
 /*  29 */ "function_return_decl ::= COLON type_specifier",
 /*  30 */ "function_return_decl ::=",
 /*  31 */ "function_scope ::= SCOPE_BEGIN SCOPE_END",
 /*  32 */ "function_scope ::= SCOPE_BEGIN statement_list SCOPE_END",
 /*  33 */ "statement_list ::= statement_list statement",
 /*  34 */ "statement_list ::= statement",
 /*  35 */ "statement ::= expression ASSIGN expression SEMI_COLON",
 /*  36 */ "statement ::= var_decl SEMI_COLON",
 /*  37 */ "statement ::= expression SEMI_COLON",
 /*  38 */ "statement ::= RETURN expression SEMI_COLON",
 /*  39 */ "statement ::= RETURN SEMI_COLON",
 /*  40 */ "var_decl ::= VAR IDENTIFIER var_type_spec ASSIGN expression",
 /*  41 */ "var_decl ::= LET IDENTIFIER var_type_spec ASSIGN expression",
 /*  42 */ "var_type_spec ::= COLON type_specifier",
 /*  43 */ "var_type_spec ::=",
 /*  44 */ "expression ::= PAR_BEGIN expression PAR_END",
 /*  45 */ "expression ::= literal",
 /*  46 */ "expression ::= unaryexp",
 /*  47 */ "expression ::= binaryexp",
 /*  48 */ "expression ::= IDENTIFIER",
 /*  49 */ "binaryexp ::= expression AND expression",
 /*  50 */ "binaryexp ::= expression OR expression",
 /*  51 */ "binaryexp ::= expression PLUS expression",
 /*  52 */ "binaryexp ::= expression MINUS expression",
 /*  53 */ "binaryexp ::= expression MULT expression",
 /*  54 */ "binaryexp ::= expression DIV expression",
 /*  55 */ "binaryexp ::= expression MOD expression",
 /*  56 */ "unaryexp ::= NOT expression",
 /*  57 */ "unaryexp ::= BIT_NOT expression",
 /*  58 */ "unaryexp ::= MINUS expression",
 /*  59 */ "literal ::= INTEGER_LITERAL",
 /*  60 */ "literal ::= BOOL_LITERAL",
 /*  61 */ "module ::= decls END",
 /*  62 */ "type_array ::= type_builtin ARRAY_BEGIN ARRAY_END",
 /*  63 */ "literal ::= DECIMAL_LITERAL",
};
#endif /* NDEBUG */


#if YYSTACKDEPTH<=0
/*
** Try to increase the size of the parser stack.  Return the number
** of errors.  Return 0 on success.
*/
static int yyGrowStack(yyParser *p){
  int newSize;
  int idx;
  yyStackEntry *pNew;

  newSize = p->yystksz*2 + 100;
  idx = p->yytos ? (int)(p->yytos - p->yystack) : 0;
  if( p->yystack==&p->yystk0 ){
    pNew = malloc(newSize*sizeof(pNew[0]));
    if( pNew ) pNew[0] = p->yystk0;
  }else{
    pNew = realloc(p->yystack, newSize*sizeof(pNew[0]));
  }
  if( pNew ){
    p->yystack = pNew;
    p->yytos = &p->yystack[idx];
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sStack grows from %d to %d entries.\n",
              yyTracePrompt, p->yystksz, newSize);
    }
#endif
    p->yystksz = newSize;
  }
  return pNew==0; 
}
#endif

/* Datatype of the argument to the memory allocated passed as the
** second argument to YALParserAlloc() below.  This can be changed by
** putting an appropriate #define in the %include section of the input
** grammar.
*/
#ifndef YYMALLOCARGTYPE
# define YYMALLOCARGTYPE size_t
#endif

/* Initialize a new parser that has already been allocated.
*/
void YALParserInit(void *yypParser){
  yyParser *pParser = (yyParser*)yypParser;
#ifdef YYTRACKMAXSTACKDEPTH
  pParser->yyhwm = 0;
#endif
#if YYSTACKDEPTH<=0
  pParser->yytos = NULL;
  pParser->yystack = NULL;
  pParser->yystksz = 0;
  if( yyGrowStack(pParser) ){
    pParser->yystack = &pParser->yystk0;
    pParser->yystksz = 1;
  }
#endif
#ifndef YYNOERRORRECOVERY
  pParser->yyerrcnt = -1;
#endif
  pParser->yytos = pParser->yystack;
  pParser->yystack[0].stateno = 0;
  pParser->yystack[0].major = 0;
#if YYSTACKDEPTH>0
  pParser->yystackEnd = &pParser->yystack[YYSTACKDEPTH-1];
#endif
}

#ifndef YALParser_ENGINEALWAYSONSTACK
/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to YALParser and YALParserFree.
*/
void *YALParserAlloc(void *(*mallocProc)(YYMALLOCARGTYPE)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (YYMALLOCARGTYPE)sizeof(yyParser) );
  if( pParser ) YALParserInit(pParser);
  return pParser;
}
#endif /* YALParser_ENGINEALWAYSONSTACK */


/* The following function deletes the "minor type" or semantic value
** associated with a symbol.  The symbol can be either a terminal
** or nonterminal. "yymajor" is the symbol code, and "yypminor" is
** a pointer to the value to be deleted.  The code used to do the 
** deletions is derived from the %destructor and/or %token_destructor
** directives of the input grammar.
*/
static void yy_destructor(
  yyParser *yypParser,    /* The parser */
  YYCODETYPE yymajor,     /* Type code for object to destroy */
  YYMINORTYPE *yypminor   /* The object to be destroyed */
){
  YALParserARG_FETCH;
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are *not* used
    ** inside the C code.
    */
/********* Begin destructor definitions ***************************************/
/********* End destructor definitions *****************************************/
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
*/
static void yy_pop_parser_stack(yyParser *pParser){
  yyStackEntry *yytos;
  assert( pParser->yytos!=0 );
  assert( pParser->yytos > pParser->yystack );
  yytos = pParser->yytos--;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yy_destructor(pParser, yytos->major, &yytos->minor);
}

/*
** Clear all secondary memory allocations from the parser
*/
void YALParserFinalize(void *p){
  yyParser *pParser = (yyParser*)p;
  while( pParser->yytos>pParser->yystack ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  if( pParser->yystack!=&pParser->yystk0 ) free(pParser->yystack);
#endif
}

#ifndef YALParser_ENGINEALWAYSONSTACK
/* 
** Deallocate and destroy a parser.  Destructors are called for
** all stack elements before shutting the parser down.
**
** If the YYPARSEFREENEVERNULL macro exists (for example because it
** is defined in a %include section of the input grammar) then it is
** assumed that the input pointer is never NULL.
*/
void YALParserFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
#ifndef YYPARSEFREENEVERNULL
  if( p==0 ) return;
#endif
  YALParserFinalize(p);
  (*freeProc)(p);
}
#endif /* YALParser_ENGINEALWAYSONSTACK */

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int YALParserStackPeak(void *p){
  yyParser *pParser = (yyParser*)p;
  return pParser->yyhwm;
}
#endif

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
*/
static unsigned int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
  int stateno = pParser->yytos->stateno;
 
  if( stateno>=YY_MIN_REDUCE ) return stateno;
  assert( stateno <= YY_SHIFT_COUNT );
  do{
    i = yy_shift_ofst[stateno];
    assert( iLookAhead!=YYNOCODE );
    i += iLookAhead;
    if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
#ifdef YYFALLBACK
      YYCODETYPE iFallback;            /* Fallback token */
      if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
             && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
             yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
        }
#endif
        assert( yyFallback[iFallback]==0 ); /* Fallback loop must terminate */
        iLookAhead = iFallback;
        continue;
      }
#endif
#ifdef YYWILDCARD
      {
        int j = i - iLookAhead + YYWILDCARD;
        if( 
#if YY_SHIFT_MIN+YYWILDCARD<0
          j>=0 &&
#endif
#if YY_SHIFT_MAX+YYWILDCARD>=YY_ACTTAB_COUNT
          j<YY_ACTTAB_COUNT &&
#endif
          yy_lookahead[j]==YYWILDCARD && iLookAhead>0
        ){
#ifndef NDEBUG
          if( yyTraceFILE ){
            fprintf(yyTraceFILE, "%sWILDCARD %s => %s\n",
               yyTracePrompt, yyTokenName[iLookAhead],
               yyTokenName[YYWILDCARD]);
          }
#endif /* NDEBUG */
          return yy_action[j];
        }
      }
#endif /* YYWILDCARD */
      return yy_default[stateno];
    }else{
      return yy_action[i];
    }
  }while(1);
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
*/
static int yy_find_reduce_action(
  int stateno,              /* Current state number */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
#ifdef YYERRORSYMBOL
  if( stateno>YY_REDUCE_COUNT ){
    return yy_default[stateno];
  }
#else
  assert( stateno<=YY_REDUCE_COUNT );
#endif
  i = yy_reduce_ofst[stateno];
  assert( i!=YY_REDUCE_USE_DFLT );
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
#ifdef YYERRORSYMBOL
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }
#else
  assert( i>=0 && i<YY_ACTTAB_COUNT );
  assert( yy_lookahead[i]==iLookAhead );
#endif
  return yy_action[i];
}

/*
** The following routine is called if the stack overflows.
*/
static void yyStackOverflow(yyParser *yypParser){
   YALParserARG_FETCH;
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yytos>yypParser->yystack ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
/******** Begin %stack_overflow code ******************************************/
/******** End %stack_overflow code ********************************************/
   YALParserARG_STORE; /* Suppress warning about unused %extra_argument var */
}

/*
** Print tracing information for a SHIFT action
*/
#ifndef NDEBUG
static void yyTraceShift(yyParser *yypParser, int yyNewState){
  if( yyTraceFILE ){
    if( yyNewState<YYNSTATE ){
      fprintf(yyTraceFILE,"%sShift '%s', go to state %d\n",
         yyTracePrompt,yyTokenName[yypParser->yytos->major],
         yyNewState);
    }else{
      fprintf(yyTraceFILE,"%sShift '%s'\n",
         yyTracePrompt,yyTokenName[yypParser->yytos->major]);
    }
  }
}
#else
# define yyTraceShift(X,Y)
#endif

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  YALParserTOKENTYPE yyMinor        /* The minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yytos++;
#ifdef YYTRACKMAXSTACKDEPTH
  if( (int)(yypParser->yytos - yypParser->yystack)>yypParser->yyhwm ){
    yypParser->yyhwm++;
    assert( yypParser->yyhwm == (int)(yypParser->yytos - yypParser->yystack) );
  }
#endif
#if YYSTACKDEPTH>0 
  if( yypParser->yytos>yypParser->yystackEnd ){
    yypParser->yytos--;
    yyStackOverflow(yypParser);
    return;
  }
#else
  if( yypParser->yytos>=&yypParser->yystack[yypParser->yystksz] ){
    if( yyGrowStack(yypParser) ){
      yypParser->yytos--;
      yyStackOverflow(yypParser);
      return;
    }
  }
#endif
  if( yyNewState > YY_MAX_SHIFT ){
    yyNewState += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
  }
  yytos = yypParser->yytos;
  yytos->stateno = (YYACTIONTYPE)yyNewState;
  yytos->major = (YYCODETYPE)yyMajor;
  yytos->minor.yy0 = yyMinor;
  yyTraceShift(yypParser, yyNewState);
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;       /* Symbol on the left-hand side of the rule */
  signed char nrhs;     /* Negative of the number of RHS symbols in the rule */
} yyRuleInfo[] = {
  { 57, -2 },
  { 57, -2 },
  { 57, -2 },
  { 57, 0 },
  { 60, -1 },
  { 60, -1 },
  { 60, -1 },
  { 54, -1 },
  { 54, -1 },
  { 54, -1 },
  { 54, -1 },
  { 54, -1 },
  { 54, -1 },
  { 54, -1 },
  { 54, -1 },
  { 54, -1 },
  { 54, -1 },
  { 54, -1 },
  { 73, -7 },
  { 73, -5 },
  { 75, -2 },
  { 75, -1 },
  { 74, -6 },
  { 55, -7 },
  { 56, -10 },
  { 63, -3 },
  { 63, -1 },
  { 63, 0 },
  { 64, -3 },
  { 59, -2 },
  { 59, 0 },
  { 66, -2 },
  { 66, -3 },
  { 65, -2 },
  { 65, -1 },
  { 67, -4 },
  { 67, -2 },
  { 67, -2 },
  { 67, -3 },
  { 67, -2 },
  { 72, -5 },
  { 72, -5 },
  { 61, -2 },
  { 61, 0 },
  { 68, -3 },
  { 68, -1 },
  { 68, -1 },
  { 68, -1 },
  { 68, -1 },
  { 70, -3 },
  { 70, -3 },
  { 70, -3 },
  { 70, -3 },
  { 70, -3 },
  { 70, -3 },
  { 70, -3 },
  { 69, -2 },
  { 69, -2 },
  { 69, -2 },
  { 71, -1 },
  { 71, -1 },
  { 58, -2 },
  { 76, -3 },
  { 71, -1 },
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  unsigned int yyruleno        /* Number of the rule by which to reduce */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  YALParserARG_FETCH;
  yymsp = yypParser->yytos;
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    yysize = yyRuleInfo[yyruleno].nrhs;
    fprintf(yyTraceFILE, "%sReduce [%s], go to state %d.\n", yyTracePrompt,
      yyRuleName[yyruleno], yymsp[yysize].stateno);
  }
#endif /* NDEBUG */

  /* Check that the stack is large enough to grow by a single entry
  ** if the RHS of the rule is empty.  This ensures that there is room
  ** enough on the stack to push the LHS value */
  if( yyRuleInfo[yyruleno].nrhs==0 ){
#ifdef YYTRACKMAXSTACKDEPTH
    if( (int)(yypParser->yytos - yypParser->yystack)>yypParser->yyhwm ){
      yypParser->yyhwm++;
      assert( yypParser->yyhwm == (int)(yypParser->yytos - yypParser->yystack));
    }
#endif
#if YYSTACKDEPTH>0 
    if( yypParser->yytos>=yypParser->yystackEnd ){
      yyStackOverflow(yypParser);
      return;
    }
#else
    if( yypParser->yytos>=&yypParser->yystack[yypParser->yystksz-1] ){
      if( yyGrowStack(yypParser) ){
        yyStackOverflow(yypParser);
        return;
      }
      yymsp = yypParser->yytos;
    }
#endif
  }

  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
/********** Begin reduce actions **********************************************/
        YYMINORTYPE yylhsminor;
      case 0: /* decls ::= decls type_decl */
{
    yylhsminor.yy63 =yymsp[-1].minor.yy63; yymsp[-1].minor.yy63->addDecl(yymsp[0].minor.yy44);
}
  yymsp[-1].minor.yy63 = yylhsminor.yy63;
        break;
      case 1: /* decls ::= decls decl_function */
{
        yylhsminor.yy63 =yymsp[-1].minor.yy63; yymsp[-1].minor.yy63->addDecl(yymsp[0].minor.yy115);
}
  yymsp[-1].minor.yy63 = yylhsminor.yy63;
        break;
      case 2: /* decls ::= decls type_function_decl */
{
        yylhsminor.yy63 =yymsp[-1].minor.yy63; yymsp[-1].minor.yy63->addDecl(yymsp[0].minor.yy113);
}
  yymsp[-1].minor.yy63 = yylhsminor.yy63;
        break;
      case 3: /* decls ::= */
{
        yymsp[1].minor.yy63 =  pParser->newAstNode<yal::DeclModule>();
        pParser->onAstNodeCreate(yymsp[1].minor.yy63);
     }
        break;
      case 4: /* type_specifier ::= type_builtin */
{yylhsminor.yy116=yymsp[0].minor.yy111;}
  yymsp[0].minor.yy116 = yylhsminor.yy116;
        break;
      case 5: /* type_specifier ::= type_array */
{yymsp[0].minor.yy116 = nullptr;}
        break;
      case 6: /* type_specifier ::= IDENTIFIER */
{yylhsminor.yy116 = pParser->newAstNode<yal::RefTypeIdentifier>(yymsp[0].minor.yy0.tokenStr);}
  yymsp[0].minor.yy116 = yylhsminor.yy116;
        break;
      case 7: /* type_builtin ::= TYPE_BOOL */
{
        yymsp[0].minor.yy111 = pParser->newAstNode<yal::RefTypeBuiltin>(pParser->getModule().getTypeContext().getTypeBuiltinBool());
        }
        break;
      case 8: /* type_builtin ::= TYPE_INT8 */
{
        yymsp[0].minor.yy111 = pParser->newAstNode<yal::RefTypeBuiltin>(pParser->getModule().getTypeContext().getTypeBuiltinI8());
        }
        break;
      case 9: /* type_builtin ::= TYPE_UINT8 */
{
        yymsp[0].minor.yy111  = pParser->newAstNode<yal::RefTypeBuiltin>(pParser->getModule().getTypeContext().getTypeBuiltinU8());
        }
        break;
      case 10: /* type_builtin ::= TYPE_INT16 */
{
        yymsp[0].minor.yy111 = pParser->newAstNode<yal::RefTypeBuiltin>(pParser->getModule().getTypeContext().getTypeBuiltinI16());
        }
        break;
      case 11: /* type_builtin ::= TYPE_UINT16 */
{
        yymsp[0].minor.yy111  = pParser->newAstNode<yal::RefTypeBuiltin>(pParser->getModule().getTypeContext().getTypeBuiltinU16());
        }
        break;
      case 12: /* type_builtin ::= TYPE_INT32 */
{
        yymsp[0].minor.yy111 = pParser->newAstNode<yal::RefTypeBuiltin>(pParser->getModule().getTypeContext().getTypeBuiltinI32());
        }
        break;
      case 13: /* type_builtin ::= TYPE_UINT32 */
{
        yymsp[0].minor.yy111 = pParser->newAstNode<yal::RefTypeBuiltin>(pParser->getModule().getTypeContext().getTypeBuiltinU32());
        }
        break;
      case 14: /* type_builtin ::= TYPE_INT64 */
{
        yymsp[0].minor.yy111 = pParser->newAstNode<yal::RefTypeBuiltin>(pParser->getModule().getTypeContext().getTypeBuiltinI64());
        }
        break;
      case 15: /* type_builtin ::= TYPE_UINT64 */
{
        yymsp[0].minor.yy111 = pParser->newAstNode<yal::RefTypeBuiltin>(pParser->getModule().getTypeContext().getTypeBuiltinU64());
        }
        break;
      case 16: /* type_builtin ::= TYPE_FLOAT */
{
        yymsp[0].minor.yy111 = pParser->newAstNode<yal::RefTypeBuiltin>(pParser->getModule().getTypeContext().getTypeBuiltinFloat());
        }
        break;
      case 17: /* type_builtin ::= TYPE_DOUBLE */
{
        yymsp[0].minor.yy111 = pParser->newAstNode<yal::RefTypeBuiltin>(pParser->getModule().getTypeContext().getTypeBuiltinDouble());
        }
        break;
      case 18: /* type_decl ::= TYPE IDENTIFIER COLON STRUCT SCOPE_BEGIN type_var_decls SCOPE_END */
{
     yymsp[-6].minor.yy44 = pParser->newAstNode<yal::DeclStruct>(yymsp[-5].minor.yy0.tokenStr, yymsp[-1].minor.yy151);
}
        break;
      case 19: /* type_decl ::= TYPE IDENTIFIER COLON type_specifier SEMI_COLON */
{
    yymsp[-4].minor.yy44 = nullptr;
}
        break;
      case 20: /* type_var_decls ::= type_var_decls type_var_decl */
{
    yylhsminor.yy151 = yymsp[-1].minor.yy151;
    yylhsminor.yy151->addDeclVar(yymsp[0].minor.yy146);
}
  yymsp[-1].minor.yy151 = yylhsminor.yy151;
        break;
      case 21: /* type_var_decls ::= type_var_decl */
{
    yylhsminor.yy151 = pParser->newAstNode<yal::DeclStructMembers>();
    yylhsminor.yy151->addDeclVar(yymsp[0].minor.yy146);
}
  yymsp[0].minor.yy151 = yylhsminor.yy151;
        break;
      case 22: /* type_var_decl ::= IDENTIFIER COLON type_specifier ASSIGN expression SEMI_COLON */
{
    yal::Qualifier qualifier;
    yylhsminor.yy146 = pParser->newAstNode<yal::DeclVar>(yymsp[-5].minor.yy0.tokenStr, qualifier, yymsp[-3].minor.yy116, yymsp[-1].minor.yy147);
}
  yymsp[-5].minor.yy146 = yylhsminor.yy146;
        break;
      case 23: /* decl_function ::= FUNCTION IDENTIFIER PAR_BEGIN function_args_decl PAR_END function_return_decl function_scope */
{
        yymsp[-6].minor.yy115 = pParser->newAstNode<yal::DeclFunction>(yymsp[-5].minor.yy0.tokenStr, yymsp[-3].minor.yy150, yymsp[-1].minor.yy116, yymsp[0].minor.yy66);
        }
        break;
      case 24: /* type_function_decl ::= FUNCTION type_specifier COLON COLON IDENTIFIER PAR_BEGIN function_args_decl PAR_END function_return_decl function_scope */
{
            yymsp[-9].minor.yy113 = pParser->newAstNode<yal::DeclTypeFunction>(yymsp[-5].minor.yy0.tokenStr, yymsp[-3].minor.yy150, yymsp[-8].minor.yy116, yymsp[-1].minor.yy116, yymsp[0].minor.yy66);
        }
        break;
      case 25: /* function_args_decl ::= function_args_decl COMMA function_arg_decl */
{
        yymsp[-2].minor.yy150->addDeclParam(yymsp[0].minor.yy87);
        yylhsminor.yy150 = yymsp[-2].minor.yy150;
}
  yymsp[-2].minor.yy150 = yylhsminor.yy150;
        break;
      case 26: /* function_args_decl ::= function_arg_decl */
{
        yylhsminor.yy150 = pParser->newAstNode<yal::DeclParamVarContainer>();
        yylhsminor.yy150->addDeclParam(yymsp[0].minor.yy87);
}
  yymsp[0].minor.yy150 = yylhsminor.yy150;
        break;
      case 27: /* function_args_decl ::= */
{
        yymsp[1].minor.yy150 = nullptr;
}
        break;
      case 28: /* function_arg_decl ::= IDENTIFIER COLON type_specifier */
{
    yylhsminor.yy87 = pParser->newAstNode<yal::DeclParamVar>(yymsp[-2].minor.yy0.tokenStr, yal::Qualifier(),yymsp[0].minor.yy116);
}
  yymsp[-2].minor.yy87 = yylhsminor.yy87;
        break;
      case 29: /* function_return_decl ::= COLON type_specifier */
{ yymsp[-1].minor.yy116 = yymsp[0].minor.yy116;}
        break;
      case 30: /* function_return_decl ::= */
      case 43: /* var_type_spec ::= */ yytestcase(yyruleno==43);
{yymsp[1].minor.yy116 = nullptr;}
        break;
      case 31: /* function_scope ::= SCOPE_BEGIN SCOPE_END */
{ yymsp[-1].minor.yy66 = nullptr;}
        break;
      case 32: /* function_scope ::= SCOPE_BEGIN statement_list SCOPE_END */
{yymsp[-2].minor.yy66 = yymsp[-1].minor.yy66;}
        break;
      case 33: /* statement_list ::= statement_list statement */
{
    yymsp[-1].minor.yy66->addStatement(yymsp[0].minor.yy38);
    yylhsminor.yy66=yymsp[-1].minor.yy66;
}
  yymsp[-1].minor.yy66 = yylhsminor.yy66;
        break;
      case 34: /* statement_list ::= statement */
{
    yylhsminor.yy66 = pParser->newAstNode<yal::StatementList>();
    yylhsminor.yy66->addStatement(yymsp[0].minor.yy38);
}
  yymsp[0].minor.yy66 = yylhsminor.yy66;
        break;
      case 35: /* statement ::= expression ASSIGN expression SEMI_COLON */
{
   yylhsminor.yy38 = pParser->newAstNode<yal::StmtAssign>(yymsp[-3].minor.yy147,yymsp[-1].minor.yy147);
}
  yymsp[-3].minor.yy38 = yylhsminor.yy38;
        break;
      case 36: /* statement ::= var_decl SEMI_COLON */
{
    yylhsminor.yy38 = yymsp[-1].minor.yy59;
}
  yymsp[-1].minor.yy38 = yylhsminor.yy38;
        break;
      case 37: /* statement ::= expression SEMI_COLON */
{yylhsminor.yy38 = yymsp[-1].minor.yy147;}
  yymsp[-1].minor.yy38 = yylhsminor.yy38;
        break;
      case 38: /* statement ::= RETURN expression SEMI_COLON */
{
    yymsp[-2].minor.yy38 = pParser->newAstNode<yal::StmtReturn>(yymsp[-1].minor.yy147);
}
        break;
      case 39: /* statement ::= RETURN SEMI_COLON */
{
    yymsp[-1].minor.yy38 = pParser->newAstNode<yal::StmtReturn>();
}
        break;
      case 40: /* var_decl ::= VAR IDENTIFIER var_type_spec ASSIGN expression */
{
    auto varDecl = pParser->newAstNode<yal::DeclVar>(yymsp[-3].minor.yy0.tokenStr, yal::Qualifier(), yymsp[-2].minor.yy116, yymsp[0].minor.yy147);
    yymsp[-4].minor.yy59= pParser->newAstNode<yal::StmtDecl>(varDecl);
}
        break;
      case 41: /* var_decl ::= LET IDENTIFIER var_type_spec ASSIGN expression */
{
    yal::Qualifier qualifier;
    qualifier.setImmutable();
    auto varDecl = pParser->newAstNode<yal::DeclVar>(yymsp[-3].minor.yy0.tokenStr, qualifier, yymsp[-2].minor.yy116, yymsp[0].minor.yy147);
    yymsp[-4].minor.yy59= pParser->newAstNode<yal::StmtDecl>(varDecl);
}
        break;
      case 42: /* var_type_spec ::= COLON type_specifier */
{yymsp[-1].minor.yy116 = yymsp[0].minor.yy116;}
        break;
      case 44: /* expression ::= PAR_BEGIN expression PAR_END */
{yymsp[-2].minor.yy147 = yymsp[-1].minor.yy147;}
        break;
      case 45: /* expression ::= literal */
{yylhsminor.yy147 = yymsp[0].minor.yy147;}
  yymsp[0].minor.yy147 = yylhsminor.yy147;
        break;
      case 46: /* expression ::= unaryexp */
{yylhsminor.yy147 = yymsp[0].minor.yy25;}
  yymsp[0].minor.yy147 = yylhsminor.yy147;
        break;
      case 47: /* expression ::= binaryexp */
{yylhsminor.yy147 = yymsp[0].minor.yy125;}
  yymsp[0].minor.yy147 = yylhsminor.yy147;
        break;
      case 48: /* expression ::= IDENTIFIER */
{
    yylhsminor.yy147 = pParser->newAstNode<yal::ExprVarRef>(yymsp[0].minor.yy0.tokenStr);
}
  yymsp[0].minor.yy147 = yylhsminor.yy147;
        break;
      case 49: /* binaryexp ::= expression AND expression */
{
        yylhsminor.yy125 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::And,
                                                        yymsp[-2].minor.yy147, yymsp[0].minor.yy147);
}
  yymsp[-2].minor.yy125 = yylhsminor.yy125;
        break;
      case 50: /* binaryexp ::= expression OR expression */
{
        yylhsminor.yy125 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Or,
                                                        yymsp[-2].minor.yy147, yymsp[0].minor.yy147);
}
  yymsp[-2].minor.yy125 = yylhsminor.yy125;
        break;
      case 51: /* binaryexp ::= expression PLUS expression */
{
        yylhsminor.yy125 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Plus,
                                                        yymsp[-2].minor.yy147, yymsp[0].minor.yy147);
}
  yymsp[-2].minor.yy125 = yylhsminor.yy125;
        break;
      case 52: /* binaryexp ::= expression MINUS expression */
{
        yylhsminor.yy125 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Minus,
                                                        yymsp[-2].minor.yy147, yymsp[0].minor.yy147);
}
  yymsp[-2].minor.yy125 = yylhsminor.yy125;
        break;
      case 53: /* binaryexp ::= expression MULT expression */
{
        yylhsminor.yy125 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Mult,
                                                        yymsp[-2].minor.yy147, yymsp[0].minor.yy147);
}
  yymsp[-2].minor.yy125 = yylhsminor.yy125;
        break;
      case 54: /* binaryexp ::= expression DIV expression */
{
        yylhsminor.yy125 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Div,
                                                        yymsp[-2].minor.yy147, yymsp[0].minor.yy147);
}
  yymsp[-2].minor.yy125 = yylhsminor.yy125;
        break;
      case 55: /* binaryexp ::= expression MOD expression */
{
        yylhsminor.yy125 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Mod,
                                                        yymsp[-2].minor.yy147, yymsp[0].minor.yy147);
}
  yymsp[-2].minor.yy125 = yylhsminor.yy125;
        break;
      case 56: /* unaryexp ::= NOT expression */
{
    yymsp[-1].minor.yy25 = pParser->newAstNode<yal::ExprUnaryOperator>(yal::UnaryOperatorType::Not,
                                               yymsp[0].minor.yy147);
}
        break;
      case 57: /* unaryexp ::= BIT_NOT expression */
{
    yymsp[-1].minor.yy25 = pParser->newAstNode<yal::ExprUnaryOperator>(yal::UnaryOperatorType::BitNot,
                                               yymsp[0].minor.yy147);
}
        break;
      case 58: /* unaryexp ::= MINUS expression */
{
    yymsp[-1].minor.yy25 = pParser->newAstNode<yal::ExprUnaryOperator>(yal::UnaryOperatorType::Negate,
                                               yymsp[0].minor.yy147);
}
        break;
      case 59: /* literal ::= INTEGER_LITERAL */
{
        yylhsminor.yy147 = pParser->newIntegerLiteral(yymsp[0].minor.yy0);
   }
  yymsp[0].minor.yy147 = yylhsminor.yy147;
        break;
      case 60: /* literal ::= BOOL_LITERAL */
{
        yylhsminor.yy147 = pParser->newAstNode<yal::ExprBoolLiteral>(yymsp[0].minor.yy0.tokenStr);
}
  yymsp[0].minor.yy147 = yylhsminor.yy147;
        break;
      default:
      /* (61) module ::= decls END */ yytestcase(yyruleno==61);
      /* (62) type_array ::= type_builtin ARRAY_BEGIN ARRAY_END */ yytestcase(yyruleno==62);
      /* (63) literal ::= DECIMAL_LITERAL */ yytestcase(yyruleno==63);
        break;
/********** End reduce actions ************************************************/
  };
  assert( yyruleno<sizeof(yyRuleInfo)/sizeof(yyRuleInfo[0]) );
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yyact = yy_find_reduce_action(yymsp[yysize].stateno,(YYCODETYPE)yygoto);

  /* There are no SHIFTREDUCE actions on nonterminals because the table
  ** generator has simplified them to pure REDUCE actions. */
  assert( !(yyact>YY_MAX_SHIFT && yyact<=YY_MAX_SHIFTREDUCE) );

  /* It is not possible for a REDUCE to be followed by an error */
  assert( yyact!=YY_ERROR_ACTION );

  if( yyact==YY_ACCEPT_ACTION ){
    yypParser->yytos += yysize;
    yy_accept(yypParser);
  }else{
    yymsp += yysize+1;
    yypParser->yytos = yymsp;
    yymsp->stateno = (YYACTIONTYPE)yyact;
    yymsp->major = (YYCODETYPE)yygoto;
    yyTraceShift(yypParser, yyact);
  }
}

/*
** The following code executes when the parse fails
*/
#ifndef YYNOERRORRECOVERY
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  YALParserARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yytos>yypParser->yystack ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
/************ Begin %parse_failure code ***************************************/
/************ End %parse_failure code *****************************************/
  YALParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  YALParserTOKENTYPE yyminor         /* The minor type of the error token */
){
  YALParserARG_FETCH;
#define TOKEN yyminor
/************ Begin %syntax_error code ****************************************/


    pParser->logParseFailure();
    {
        yal::Log::MultiLineScope multiLineScope(pParser->getLog());
        pParser->getLog().error("Expected one of the following tokens:\n");
        int n = sizeof(yyTokenName) / sizeof(yyTokenName[0]);
        for (int i = 0; i < n; ++i) {
            int a = yy_find_shift_action(yypParser, (YYCODETYPE)i);
            if (a < YYNSTATE + YYNRULE) {
                pParser->getLog().error("%\n",yyTokenName[i]);
            }
        }
    }
/************ End %syntax_error code ******************************************/
  YALParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  YALParserARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
#ifndef YYNOERRORRECOVERY
  yypParser->yyerrcnt = -1;
#endif
  assert( yypParser->yytos==yypParser->yystack );
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
/*********** Begin %parse_accept code *****************************************/
/*********** End %parse_accept code *******************************************/
  YALParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "YALParserAlloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void YALParser(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  YALParserTOKENTYPE yyminor       /* The value for the token */
  YALParserARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  unsigned int yyact;   /* The parser action. */
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  int yyendofinput;     /* True if we are at the end of input */
#endif
#ifdef YYERRORSYMBOL
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
#endif
  yyParser *yypParser;  /* The parser */

  yypParser = (yyParser*)yyp;
  assert( yypParser->yytos!=0 );
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  yyendofinput = (yymajor==0);
#endif
  YALParserARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput '%s'\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,(YYCODETYPE)yymajor);
    if( yyact <= YY_MAX_SHIFTREDUCE ){
      yy_shift(yypParser,yyact,yymajor,yyminor);
#ifndef YYNOERRORRECOVERY
      yypParser->yyerrcnt--;
#endif
      yymajor = YYNOCODE;
    }else if( yyact <= YY_MAX_REDUCE ){
      yy_reduce(yypParser,yyact-YY_MIN_REDUCE);
    }else{
      assert( yyact == YY_ERROR_ACTION );
      yyminorunion.yy0 = yyminor;
#ifdef YYERRORSYMBOL
      int yymx;
#endif
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminor);
      }
      yymx = yypParser->yytos->major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yypParser, (YYCODETYPE)yymajor, &yyminorunion);
        yymajor = YYNOCODE;
      }else{
        while( yypParser->yytos >= yypParser->yystack
            && yymx != YYERRORSYMBOL
            && (yyact = yy_find_reduce_action(
                        yypParser->yytos->stateno,
                        YYERRORSYMBOL)) >= YY_MIN_REDUCE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yytos < yypParser->yystack || yymajor==0 ){
          yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
#ifndef YYNOERRORRECOVERY
          yypParser->yyerrcnt = -1;
#endif
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          yy_shift(yypParser,yyact,YYERRORSYMBOL,yyminor);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
#elif defined(YYNOERRORRECOVERY)
      /* If the YYNOERRORRECOVERY macro is defined, then do not attempt to
      ** do any kind of error recovery.  Instead, simply invoke the syntax
      ** error routine and continue going as if nothing had happened.
      **
      ** Applications can set this macro (for example inside %include) if
      ** they intend to abandon the parse upon the first syntax error seen.
      */
      yy_syntax_error(yypParser,yymajor, yyminor);
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      yymajor = YYNOCODE;
      
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor, yyminor);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
#ifndef YYNOERRORRECOVERY
        yypParser->yyerrcnt = -1;
#endif
      }
      yymajor = YYNOCODE;
#endif
    }
  }while( yymajor!=YYNOCODE && yypParser->yytos>yypParser->yystack );
#ifndef NDEBUG
  if( yyTraceFILE ){
    yyStackEntry *i;
    char cDiv = '[';
    fprintf(yyTraceFILE,"%sReturn. Stack=",yyTracePrompt);
    for(i=&yypParser->yystack[1]; i<=yypParser->yytos; i++){
      fprintf(yyTraceFILE,"%c%s", cDiv, yyTokenName[i->major]);
      cDiv = ' ';
    }
    fprintf(yyTraceFILE,"]\n");
  }
#endif
  return;
}
