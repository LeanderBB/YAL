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
#define YAL_TOKEN_NEG                            18
#define YAL_TOKEN_NOT                            19
#define YAL_TOKEN_DOT                            20
#define YAL_TOKEN_END                            21
#define YAL_TOKEN_IDENTIFIER                     22
#define YAL_TOKEN_TYPE_BOOL                      23
#define YAL_TOKEN_TYPE_INT8                      24
#define YAL_TOKEN_TYPE_UINT8                     25
#define YAL_TOKEN_TYPE_INT16                     26
#define YAL_TOKEN_TYPE_UINT16                    27
#define YAL_TOKEN_TYPE_INT32                     28
#define YAL_TOKEN_TYPE_UINT32                    29
#define YAL_TOKEN_TYPE_INT64                     30
#define YAL_TOKEN_TYPE_UINT64                    31
#define YAL_TOKEN_TYPE_FLOAT                     32
#define YAL_TOKEN_TYPE_DOUBLE                    33
#define YAL_TOKEN_ARRAY_BEGIN                    34
#define YAL_TOKEN_ARRAY_END                      35
#define YAL_TOKEN_TYPE                           36
#define YAL_TOKEN_COLON                          37
#define YAL_TOKEN_STRUCT                         38
#define YAL_TOKEN_SCOPE_BEGIN                    39
#define YAL_TOKEN_SCOPE_END                      40
#define YAL_TOKEN_SEMI_COLON                     41
#define YAL_TOKEN_FUNCTION                       42
#define YAL_TOKEN_PAR_BEGIN                      43
#define YAL_TOKEN_PAR_END                        44
#define YAL_TOKEN_COMMA                          45
#define YAL_TOKEN_VAR                            46
#define YAL_TOKEN_LET                            47
#define YAL_TOKEN_INTEGER_LITERAL                48
#define YAL_TOKEN_DECIMAL_LITERAL                49
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
#define YYNOCODE 74
#define YYACTIONTYPE unsigned char
#if INTERFACE
#define YALParserTOKENTYPE yal::StringRefPod
#endif
typedef union {
  int yyinit;
  YALParserTOKENTYPE yy0;
  yal::DeclFunction* yy25;
  const yal::RefTypeBuiltin* yy60;
  yal::DeclTypeFunction* yy65;
  const yal::RefType* yy107;
  yal::DeclModule* yy109;
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
#define YYNSTATE             69
#define YYNRULE              57
#define YY_MAX_SHIFT         68
#define YY_MIN_SHIFTREDUCE   112
#define YY_MAX_SHIFTREDUCE   168
#define YY_MIN_REDUCE        169
#define YY_MAX_REDUCE        225
#define YY_ERROR_ACTION      226
#define YY_ACCEPT_ACTION     227
#define YY_NO_ACTION         228
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
#define YY_ACTTAB_COUNT (198)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*    10 */   126,  127,    8,    8,   58,   57,   63,  116,  117,  118,
 /*    20 */   119,  120,  121,  122,  123,  124,  125,  126,  127,   59,
 /*    30 */   117,  118,  119,  120,  121,  122,  123,  124,  125,  126,
 /*    40 */   127,   13,   19,   21,   18,   17,   16,   15,   14,  204,
 /*    50 */    58,   57,   50,   18,   17,   16,   15,   14,   13,   19,
 /*    60 */    21,   56,  169,  191,   16,   15,   14,    9,  227,  190,
 /*    70 */    18,   17,   16,   15,   14,   13,   19,   21,   36,  145,
 /*    80 */    36,   37,   62,   37,  159,   54,   52,   18,   17,   16,
 /*    90 */    15,   14,   21,   56,   68,   55,   34,   38,   34,   38,
 /*   100 */   136,  156,   18,   17,   16,   15,   14,   65,  221,   67,
 /*   110 */   221,  146,  220,   60,  220,    6,  172,   54,   52,   20,
 /*   120 */   219,   62,  219,   65,  167,  168,   26,   26,   65,  201,
 /*   130 */   195,   42,  172,   42,  187,    1,   65,  172,   65,   43,
 /*   140 */     4,   43,  211,   65,   49,  172,  186,  172,   40,   61,
 /*   150 */    40,   35,  172,   35,   41,  198,   41,   44,   39,   44,
 /*   160 */    39,   45,   27,   45,   28,   29,    5,   33,   29,  185,
 /*   170 */    32,   51,   53,   23,   46,   47,   48,    3,   10,   30,
 /*   180 */    11,   31,   22,  150,   12,  149,   24,  137,  140,   25,
 /*   190 */     7,  135,   64,   66,  189,  171,  171,    2,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    22,   23,   24,   25,   26,   27,   28,   29,   30,   31,
 /*    10 */    32,   33,   66,   67,   68,   69,   38,   22,   23,   24,
 /*    20 */    25,   26,   27,   28,   29,   30,   31,   32,   33,   22,
 /*    30 */    23,   24,   25,   26,   27,   28,   29,   30,   31,   32,
 /*    40 */    33,    1,    2,    3,   13,   14,   15,   16,   17,   67,
 /*    50 */    68,   69,   22,   13,   14,   15,   16,   17,    1,    2,
 /*    60 */     3,   22,   52,   53,   15,   16,   17,   54,   55,   59,
 /*    70 */    13,   14,   15,   16,   17,    1,    2,    3,   70,   40,
 /*    80 */    72,   70,   22,   72,   44,   46,   47,   13,   14,   15,
 /*    90 */    16,   17,    3,   22,   21,   19,   70,   70,   72,   72,
 /*   100 */    40,   44,   13,   14,   15,   16,   17,   51,   70,   36,
 /*   110 */    72,   40,   70,   57,   72,   42,   60,   46,   47,   43,
 /*   120 */    70,   22,   72,   51,   48,   49,   61,   62,   51,   57,
 /*   130 */    62,   70,   60,   72,   57,   39,   51,   60,   51,   70,
 /*   140 */    37,   72,   57,   51,   57,   60,   64,   60,   70,   57,
 /*   150 */    72,   70,   60,   72,   70,   65,   72,   63,   70,   65,
 /*   160 */    72,   63,   56,   65,   44,   45,   37,   44,   45,   64,
 /*   170 */    56,   71,   71,   43,   22,   37,   37,   37,    1,   22,
 /*   180 */     1,   22,    1,   41,   43,   41,   43,   41,   41,   39,
 /*   190 */    37,   35,   34,   22,    0,   73,   73,   37,
};
#define YY_SHIFT_USE_DFLT (198)
#define YY_SHIFT_COUNT    (68)
#define YY_SHIFT_MIN      (-22)
#define YY_SHIFT_MAX      (194)
static const short yy_shift_ofst[] = {
 /*     0 */   198,   39,  -22,   -5,   -5,   -5,    7,   -5,   71,   73,
 /*    10 */    76,   76,   76,   76,   76,   76,   76,   76,   76,   76,
 /*    20 */    76,   76,   76,   30,   30,   99,   60,   96,  103,   30,
 /*    30 */   129,  129,   96,  103,   40,   57,   74,   74,   74,   74,
 /*    40 */    89,   31,   49,   49,  120,  123,  130,  152,  138,  139,
 /*    50 */   140,  177,  157,  179,  159,  141,  181,  142,  144,  143,
 /*    60 */   146,  147,  153,  150,  156,  158,  160,  171,  194,
};
#define YY_REDUCE_USE_DFLT (-55)
#define YY_REDUCE_COUNT (33)
#define YY_REDUCE_MIN   (-54)
#define YY_REDUCE_MAX   (114)
static const signed char yy_reduce_ofst[] = {
 /*     0 */    13,  -54,   56,   72,   77,   85,   87,   92,  -18,   10,
 /*    10 */     8,   11,   26,   27,   38,   42,   50,   61,   69,   78,
 /*    20 */    81,   84,   88,   94,   98,   65,   68,   82,  106,   90,
 /*    30 */   100,  101,  105,  114,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   170,  226,  226,  226,  226,  226,  226,  226,  226,  226,
 /*    10 */   226,  226,  226,  226,  226,  226,  226,  226,  226,  226,
 /*    20 */   226,  226,  226,  200,  200,  226,  226,  226,  188,  226,
 /*    30 */   212,  212,  226,  188,  226,  226,  210,  209,  222,  208,
 /*    40 */   215,  214,  218,  217,  226,  226,  226,  226,  226,  226,
 /*    50 */   226,  226,  226,  226,  226,  226,  226,  226,  226,  173,
 /*    60 */   226,  226,  226,  226,  226,  171,  226,  226,  226,
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
  "DIV",           "MOD",           "NEG",           "NOT",         
  "DOT",           "END",           "IDENTIFIER",    "TYPE_BOOL",   
  "TYPE_INT8",     "TYPE_UINT8",    "TYPE_INT16",    "TYPE_UINT16", 
  "TYPE_INT32",    "TYPE_UINT32",   "TYPE_INT64",    "TYPE_UINT64", 
  "TYPE_FLOAT",    "TYPE_DOUBLE",   "ARRAY_BEGIN",   "ARRAY_END",   
  "TYPE",          "COLON",         "STRUCT",        "SCOPE_BEGIN", 
  "SCOPE_END",     "SEMI_COLON",    "FUNCTION",      "PAR_BEGIN",   
  "PAR_END",       "COMMA",         "VAR",           "LET",         
  "INTEGER_LITERAL",  "DECIMAL_LITERAL",  "error",         "type_builtin",
  "decl_function",  "type_function_decl",  "decls",         "module",      
  "function_return_decl",  "type_specifier",  "named_decl",    "type_decl",   
  "type_array",    "type_var_decls",  "type_var_decl",  "function_args_decl",
  "function_scope",  "function_arg_decl",  "statement_list",  "statement",   
  "var_assignment",  "var_decl",      "expression",    "var_type_spec",
  "literal",     
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "decls ::= decls decl_function",
 /*   1 */ "decls ::=",
 /*   2 */ "type_specifier ::= type_builtin",
 /*   3 */ "type_specifier ::= type_array",
 /*   4 */ "type_specifier ::= IDENTIFIER",
 /*   5 */ "type_builtin ::= TYPE_BOOL",
 /*   6 */ "type_builtin ::= TYPE_INT8",
 /*   7 */ "type_builtin ::= TYPE_UINT8",
 /*   8 */ "type_builtin ::= TYPE_INT16",
 /*   9 */ "type_builtin ::= TYPE_UINT16",
 /*  10 */ "type_builtin ::= TYPE_INT32",
 /*  11 */ "type_builtin ::= TYPE_UINT32",
 /*  12 */ "type_builtin ::= TYPE_INT64",
 /*  13 */ "type_builtin ::= TYPE_UINT64",
 /*  14 */ "type_builtin ::= TYPE_FLOAT",
 /*  15 */ "type_builtin ::= TYPE_DOUBLE",
 /*  16 */ "decl_function ::= FUNCTION IDENTIFIER PAR_BEGIN function_args_decl PAR_END function_return_decl function_scope",
 /*  17 */ "type_function_decl ::= FUNCTION type_specifier COLON COLON IDENTIFIER PAR_BEGIN function_args_decl PAR_END function_return_decl function_scope",
 /*  18 */ "function_return_decl ::= COLON type_specifier",
 /*  19 */ "function_return_decl ::=",
 /*  20 */ "module ::= decls END",
 /*  21 */ "decls ::= decls type_decl",
 /*  22 */ "decls ::= decls type_function_decl",
 /*  23 */ "type_array ::= type_builtin ARRAY_BEGIN ARRAY_END",
 /*  24 */ "type_decl ::= TYPE IDENTIFIER COLON STRUCT SCOPE_BEGIN type_var_decls SCOPE_END",
 /*  25 */ "type_decl ::= TYPE IDENTIFIER COLON type_specifier SEMI_COLON",
 /*  26 */ "type_var_decls ::= type_var_decls type_var_decl",
 /*  27 */ "type_var_decls ::= type_var_decl",
 /*  28 */ "type_var_decl ::= IDENTIFIER COLON type_specifier SEMI_COLON",
 /*  29 */ "function_args_decl ::= function_args_decl COMMA function_arg_decl",
 /*  30 */ "function_args_decl ::= function_arg_decl",
 /*  31 */ "function_args_decl ::=",
 /*  32 */ "function_arg_decl ::= IDENTIFIER COLON type_specifier",
 /*  33 */ "function_scope ::= SCOPE_BEGIN SCOPE_END",
 /*  34 */ "function_scope ::= SCOPE_BEGIN statement_list SCOPE_END",
 /*  35 */ "statement_list ::= statement_list statement",
 /*  36 */ "statement_list ::= statement",
 /*  37 */ "statement ::= var_assignment SEMI_COLON",
 /*  38 */ "statement ::= var_decl SEMI_COLON",
 /*  39 */ "var_assignment ::= IDENTIFIER ASSIGN expression",
 /*  40 */ "var_decl ::= VAR IDENTIFIER var_type_spec ASSIGN expression",
 /*  41 */ "var_decl ::= LET IDENTIFIER var_type_spec ASSIGN expression",
 /*  42 */ "var_type_spec ::= COLON type_specifier",
 /*  43 */ "var_type_spec ::=",
 /*  44 */ "expression ::= PAR_BEGIN expression PAR_END",
 /*  45 */ "expression ::= expression AND expression",
 /*  46 */ "expression ::= expression OR expression",
 /*  47 */ "expression ::= NOT PAR_BEGIN expression PAR_END",
 /*  48 */ "expression ::= expression PLUS expression",
 /*  49 */ "expression ::= expression MINUS expression",
 /*  50 */ "expression ::= expression MULT expression",
 /*  51 */ "expression ::= expression DIV expression",
 /*  52 */ "expression ::= expression MOD expression",
 /*  53 */ "expression ::= expression ASSIGN expression",
 /*  54 */ "expression ::= literal",
 /*  55 */ "literal ::= INTEGER_LITERAL",
 /*  56 */ "literal ::= DECIMAL_LITERAL",
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
  { 54, -2 },
  { 54, 0 },
  { 57, -1 },
  { 57, -1 },
  { 57, -1 },
  { 51, -1 },
  { 51, -1 },
  { 51, -1 },
  { 51, -1 },
  { 51, -1 },
  { 51, -1 },
  { 51, -1 },
  { 51, -1 },
  { 51, -1 },
  { 51, -1 },
  { 51, -1 },
  { 52, -7 },
  { 53, -10 },
  { 56, -2 },
  { 56, 0 },
  { 55, -2 },
  { 54, -2 },
  { 54, -2 },
  { 60, -3 },
  { 59, -7 },
  { 59, -5 },
  { 61, -2 },
  { 61, -1 },
  { 62, -4 },
  { 63, -3 },
  { 63, -1 },
  { 63, 0 },
  { 65, -3 },
  { 64, -2 },
  { 64, -3 },
  { 66, -2 },
  { 66, -1 },
  { 67, -2 },
  { 67, -2 },
  { 68, -3 },
  { 69, -5 },
  { 69, -5 },
  { 71, -2 },
  { 71, 0 },
  { 70, -3 },
  { 70, -3 },
  { 70, -3 },
  { 70, -4 },
  { 70, -3 },
  { 70, -3 },
  { 70, -3 },
  { 70, -3 },
  { 70, -3 },
  { 70, -3 },
  { 70, -1 },
  { 72, -1 },
  { 72, -1 },
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
      case 0: /* decls ::= decls decl_function */
{
        yylhsminor.yy109 =yymsp[-1].minor.yy109; yymsp[-1].minor.yy109->addDecl(yymsp[0].minor.yy25);
}
  yymsp[-1].minor.yy109 = yylhsminor.yy109;
        break;
      case 1: /* decls ::= */
{
        yymsp[1].minor.yy109 =  pParser->newASTNode<yal::DeclModule>();
     }
        break;
      case 2: /* type_specifier ::= type_builtin */
{yylhsminor.yy107=yymsp[0].minor.yy60;}
  yymsp[0].minor.yy107 = yylhsminor.yy107;
        break;
      case 3: /* type_specifier ::= type_array */
{yymsp[0].minor.yy107 = nullptr;}
        break;
      case 4: /* type_specifier ::= IDENTIFIER */
{yylhsminor.yy107 = pParser->newASTNode<yal::RefTypeIdentifier>(yymsp[0].minor.yy0);}
  yymsp[0].minor.yy107 = yylhsminor.yy107;
        break;
      case 5: /* type_builtin ::= TYPE_BOOL */
{
        yymsp[0].minor.yy60 = pParser->newASTNode<yal::RefTypeBuiltin>(pParser->getModule().getTypeContext().getTypeBuiltinBool());
        }
        break;
      case 6: /* type_builtin ::= TYPE_INT8 */
{
        yymsp[0].minor.yy60 = pParser->newASTNode<yal::RefTypeBuiltin>(pParser->getModule().getTypeContext().getTypeBuiltinI8());
        }
        break;
      case 7: /* type_builtin ::= TYPE_UINT8 */
{
        yymsp[0].minor.yy60  = pParser->newASTNode<yal::RefTypeBuiltin>(pParser->getModule().getTypeContext().getTypeBuiltinU8());
        }
        break;
      case 8: /* type_builtin ::= TYPE_INT16 */
{
        yymsp[0].minor.yy60 = pParser->newASTNode<yal::RefTypeBuiltin>(pParser->getModule().getTypeContext().getTypeBuiltinI16());
        }
        break;
      case 9: /* type_builtin ::= TYPE_UINT16 */
{
        yymsp[0].minor.yy60  = pParser->newASTNode<yal::RefTypeBuiltin>(pParser->getModule().getTypeContext().getTypeBuiltinU16());
        }
        break;
      case 10: /* type_builtin ::= TYPE_INT32 */
{
        yymsp[0].minor.yy60 = pParser->newASTNode<yal::RefTypeBuiltin>(pParser->getModule().getTypeContext().getTypeBuiltinI32());
        }
        break;
      case 11: /* type_builtin ::= TYPE_UINT32 */
{
        yymsp[0].minor.yy60 = pParser->newASTNode<yal::RefTypeBuiltin>(pParser->getModule().getTypeContext().getTypeBuiltinU32());
        }
        break;
      case 12: /* type_builtin ::= TYPE_INT64 */
{
        yymsp[0].minor.yy60 = pParser->newASTNode<yal::RefTypeBuiltin>(pParser->getModule().getTypeContext().getTypeBuiltinI64());
        }
        break;
      case 13: /* type_builtin ::= TYPE_UINT64 */
{
        yymsp[0].minor.yy60 = pParser->newASTNode<yal::RefTypeBuiltin>(pParser->getModule().getTypeContext().getTypeBuiltinU64());
        }
        break;
      case 14: /* type_builtin ::= TYPE_FLOAT */
{
        yymsp[0].minor.yy60 = pParser->newASTNode<yal::RefTypeBuiltin>(pParser->getModule().getTypeContext().getTypeBuiltinFloat());
        }
        break;
      case 15: /* type_builtin ::= TYPE_DOUBLE */
{
        yymsp[0].minor.yy60 = pParser->newASTNode<yal::RefTypeBuiltin>(pParser->getModule().getTypeContext().getTypeBuiltinDouble());
        }
        break;
      case 16: /* decl_function ::= FUNCTION IDENTIFIER PAR_BEGIN function_args_decl PAR_END function_return_decl function_scope */
{
        yymsp[-6].minor.yy25 = pParser->newASTNode<yal::DeclFunction>(yymsp[-5].minor.yy0, yymsp[-1].minor.yy107);
        }
        break;
      case 17: /* type_function_decl ::= FUNCTION type_specifier COLON COLON IDENTIFIER PAR_BEGIN function_args_decl PAR_END function_return_decl function_scope */
{
            yymsp[-9].minor.yy65 = pParser->newASTNode<yal::DeclTypeFunction>(yymsp[-5].minor.yy0, yymsp[-8].minor.yy107, yymsp[-1].minor.yy107);
        }
        break;
      case 18: /* function_return_decl ::= COLON type_specifier */
{ yymsp[-1].minor.yy107 = yymsp[0].minor.yy107;}
        break;
      case 19: /* function_return_decl ::= */
{yymsp[1].minor.yy107 = nullptr;}
        break;
      default:
      /* (20) module ::= decls END */ yytestcase(yyruleno==20);
      /* (21) decls ::= decls type_decl */ yytestcase(yyruleno==21);
      /* (22) decls ::= decls type_function_decl */ yytestcase(yyruleno==22);
      /* (23) type_array ::= type_builtin ARRAY_BEGIN ARRAY_END */ yytestcase(yyruleno==23);
      /* (24) type_decl ::= TYPE IDENTIFIER COLON STRUCT SCOPE_BEGIN type_var_decls SCOPE_END */ yytestcase(yyruleno==24);
      /* (25) type_decl ::= TYPE IDENTIFIER COLON type_specifier SEMI_COLON */ yytestcase(yyruleno==25);
      /* (26) type_var_decls ::= type_var_decls type_var_decl */ yytestcase(yyruleno==26);
      /* (27) type_var_decls ::= type_var_decl (OPTIMIZED OUT) */ assert(yyruleno!=27);
      /* (28) type_var_decl ::= IDENTIFIER COLON type_specifier SEMI_COLON */ yytestcase(yyruleno==28);
      /* (29) function_args_decl ::= function_args_decl COMMA function_arg_decl */ yytestcase(yyruleno==29);
      /* (30) function_args_decl ::= function_arg_decl (OPTIMIZED OUT) */ assert(yyruleno!=30);
      /* (31) function_args_decl ::= */ yytestcase(yyruleno==31);
      /* (32) function_arg_decl ::= IDENTIFIER COLON type_specifier */ yytestcase(yyruleno==32);
      /* (33) function_scope ::= SCOPE_BEGIN SCOPE_END */ yytestcase(yyruleno==33);
      /* (34) function_scope ::= SCOPE_BEGIN statement_list SCOPE_END */ yytestcase(yyruleno==34);
      /* (35) statement_list ::= statement_list statement */ yytestcase(yyruleno==35);
      /* (36) statement_list ::= statement (OPTIMIZED OUT) */ assert(yyruleno!=36);
      /* (37) statement ::= var_assignment SEMI_COLON */ yytestcase(yyruleno==37);
      /* (38) statement ::= var_decl SEMI_COLON */ yytestcase(yyruleno==38);
      /* (39) var_assignment ::= IDENTIFIER ASSIGN expression */ yytestcase(yyruleno==39);
      /* (40) var_decl ::= VAR IDENTIFIER var_type_spec ASSIGN expression */ yytestcase(yyruleno==40);
      /* (41) var_decl ::= LET IDENTIFIER var_type_spec ASSIGN expression */ yytestcase(yyruleno==41);
      /* (42) var_type_spec ::= COLON type_specifier */ yytestcase(yyruleno==42);
      /* (43) var_type_spec ::= */ yytestcase(yyruleno==43);
      /* (44) expression ::= PAR_BEGIN expression PAR_END */ yytestcase(yyruleno==44);
      /* (45) expression ::= expression AND expression */ yytestcase(yyruleno==45);
      /* (46) expression ::= expression OR expression */ yytestcase(yyruleno==46);
      /* (47) expression ::= NOT PAR_BEGIN expression PAR_END */ yytestcase(yyruleno==47);
      /* (48) expression ::= expression PLUS expression */ yytestcase(yyruleno==48);
      /* (49) expression ::= expression MINUS expression */ yytestcase(yyruleno==49);
      /* (50) expression ::= expression MULT expression */ yytestcase(yyruleno==50);
      /* (51) expression ::= expression DIV expression */ yytestcase(yyruleno==51);
      /* (52) expression ::= expression MOD expression */ yytestcase(yyruleno==52);
      /* (53) expression ::= expression ASSIGN expression */ yytestcase(yyruleno==53);
      /* (54) expression ::= literal (OPTIMIZED OUT) */ assert(yyruleno!=54);
      /* (55) literal ::= INTEGER_LITERAL */ yytestcase(yyruleno==55);
      /* (56) literal ::= DECIMAL_LITERAL */ yytestcase(yyruleno==56);
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
