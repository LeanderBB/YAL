/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is included that follows the "include" declaration
** in the input grammar file. */
#include <stdio.h>

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
#define YYMALLOCARGTYPE size_t
#include "parserimpl.h"
/* Next is all token values, in a form suitable for use by makeheaders.
** This section will be null unless lemon is run with the -m switch.
*/
/* 
** These constants (all generated automatically by the parser generator)
** specify the various kinds of tokens (terminals) that the parser
** understands. 
**
** Each symbol here is a terminal symbol in the grammar.
*/
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
/* Make sure the INTERFACE macro is defined.
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/* The next thing included is series of defines which control
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 terminals
**                       and nonterminals.  "int" is used otherwise.
**    YYNOCODE           is a number of type YYCODETYPE which corresponds
**                       to no legal terminal or nonterminal number.  This
**                       number is used to fill in empty slots of the hash 
**                       table.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       have fall-back values which should be used if the
**                       original value of the token will not parse.
**    YYACTIONTYPE       is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 rules and
**                       states combined.  "int" is used otherwise.
**    YALParserTOKENTYPE     is the data type used for minor tokens given 
**                       directly to the parser from the tokenizer.
**    YYMINORTYPE        is the data type used for all minor tokens.
**                       This is typically a union of many types, one of
**                       which is YALParserTOKENTYPE.  The entry in the union
**                       for base tokens is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    YALParserARG_SDECL     A static variable declaration for the %extra_argument
**    YALParserARG_PDECL     A parameter declaration for the %extra_argument
**    YALParserARG_STORE     Code to store %extra_argument into yypParser
**    YALParserARG_FETCH     Code to extract %extra_argument from yypParser
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
*/
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
  yal::StmtExpression* yy147;
  yal::DeclParamVarContainer* yy150;
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
#define YYNSTATE 120
#define YYNRULE 64
#define YY_NO_ACTION      (YYNSTATE+YYNRULE+2)
#define YY_ACCEPT_ACTION  (YYNSTATE+YYNRULE+1)
#define YY_ERROR_ACTION   (YYNSTATE+YYNRULE)

/* The yyzerominor constant is used to initialize instances of
** YYMINORTYPE objects to zero. */
static const YYMINORTYPE yyzerominor = { 0 };

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
**   0 <= N < YYNSTATE                  Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   YYNSTATE <= N < YYNSTATE+YYNRULE   Reduce by rule N-YYNSTATE.
**
**   N == YYNSTATE+YYNRULE              A syntax error has occurred.
**
**   N == YYNSTATE+YYNRULE+1            The parser accepts its input.
**
**   N == YYNSTATE+YYNRULE+2            No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as
**
**      yy_action[ yy_shift_ofst[S] + X ]
**
** If the index value yy_shift_ofst[S]+X is out of range or if the value
** yy_lookahead[yy_shift_ofst[S]+X] is not equal to X or if yy_shift_ofst[S]
** is equal to YY_SHIFT_USE_DFLT, it means that the action is not in the table
** and that yy_default[S] should be used instead.  
**
** The formula above is for computing the action when the lookahead is
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
*/
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    74,   75,   76,   77,   78,   79,   80,   81,   82,   83,
 /*    10 */    84,   85,   68,   98,   99,   97,   67,   74,   75,   76,
 /*    20 */    77,   78,   79,   80,   81,   82,   83,   84,   85,   24,
 /*    30 */   185,    9,   45,   98,   99,   97,    7,    5,   28,   89,
 /*    40 */   100,    1,   51,    9,  104,   98,   99,   97,    7,    5,
 /*    50 */    29,   32,  100,  102,   98,   99,   97,   50,   92,   46,
 /*    60 */   118,   11,   63,   20,    3,   48,   53,  106,  107,  108,
 /*    70 */    93,   34,   32,   11,   47,  118,    3,   48,   53,  106,
 /*    80 */   107,  108,   62,   75,   76,   77,   78,   79,   80,   81,
 /*    90 */    82,   83,   84,   85,    9,   13,   10,   12,   63,    7,
 /*   100 */     5,   70,   71,  100,   16,    8,   17,    6,    4,   16,
 /*   110 */     8,   17,    9,   55,   10,   12,   86,    7,    5,   69,
 /*   120 */    87,  100,  112,   22,   11,    6,    4,   16,    8,   17,
 /*   130 */   106,  107,  108,   30,   91,   66,  109,  105,   98,   99,
 /*   140 */    97,   57,   11,  119,   21,   10,   12,  116,  106,  107,
 /*   150 */   108,   10,   12,   54,   95,   73,    6,    4,   16,    8,
 /*   160 */    17,   31,    6,    4,   16,    8,   17,   10,   12,    2,
 /*   170 */    49,  114,   36,   98,   99,   97,   52,   35,    6,    4,
 /*   180 */    16,    8,   17,   61,   15,  111,   94,   36,   98,   99,
 /*   190 */    97,   52,   12,  120,   96,    6,    4,   16,    8,   17,
 /*   200 */   110,   33,    6,    4,   16,    8,   17,   42,   98,   99,
 /*   210 */    97,   14,   23,   90,   39,   98,   99,   97,   43,   98,
 /*   220 */    99,   97,   58,   38,   98,   99,   97,   41,   98,   99,
 /*   230 */    97,   40,   98,   99,   97,  101,   98,   99,   97,   59,
 /*   240 */   103,   98,   99,   97,   37,   98,   99,   97,   44,   98,
 /*   250 */    99,   97,   66,   66,   66,   60,   27,   66,   65,   56,
 /*   260 */   115,   66,   18,  117,   26,   19,   72,  113,   88,   64,
 /*   270 */   186,   25,   73,   73,   73,  186,  186,   73,  186,  186,
 /*   280 */   186,   73,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    23,   24,   25,   26,   27,   28,   29,   30,   31,   32,
 /*    10 */    33,   34,   68,   69,   70,   71,   39,   23,   24,   25,
 /*    20 */    26,   27,   28,   29,   30,   31,   32,   33,   34,   57,
 /*    30 */    58,   14,   68,   69,   70,   71,   19,   20,   75,   76,
 /*    40 */    23,   40,   22,   14,   68,   69,   70,   71,   19,   20,
 /*    50 */    45,   46,   23,   68,   69,   70,   71,   37,   41,   63,
 /*    60 */    64,   44,   23,   43,   47,   48,   49,   50,   51,   52,
 /*    70 */    41,   45,   46,   44,   63,   64,   47,   48,   49,   50,
 /*    80 */    51,   52,   23,   24,   25,   26,   27,   28,   29,   30,
 /*    90 */    31,   32,   33,   34,   14,    1,    2,    3,   23,   19,
 /*   100 */    20,   55,   56,   23,   15,   16,   17,   13,   14,   15,
 /*   110 */    16,   17,   14,   23,    2,    3,   41,   19,   20,   73,
 /*   120 */    76,   23,   42,   38,   44,   13,   14,   15,   16,   17,
 /*   130 */    50,   51,   52,   59,   66,   54,   42,   68,   69,   70,
 /*   140 */    71,   60,   44,   66,   38,    2,    3,   64,   50,   51,
 /*   150 */    52,    2,    3,   61,   42,   74,   13,   14,   15,   16,
 /*   160 */    17,   59,   13,   14,   15,   16,   17,    2,    3,   65,
 /*   170 */    61,   67,   68,   69,   70,   71,   72,   23,   13,   14,
 /*   180 */    15,   16,   17,   23,    1,   42,   67,   68,   69,   70,
 /*   190 */    71,   72,    3,    0,   45,   13,   14,   15,   16,   17,
 /*   200 */    42,   23,   13,   14,   15,   16,   17,   68,   69,   70,
 /*   210 */    71,    1,   38,   42,   68,   69,   70,   71,   68,   69,
 /*   220 */    70,   71,   38,   68,   69,   70,   71,   68,   69,   70,
 /*   230 */    71,   68,   69,   70,   71,   68,   69,   70,   71,   38,
 /*   240 */    68,   69,   70,   71,   68,   69,   70,   71,   68,   69,
 /*   250 */    70,   71,   54,   54,   54,   23,   44,   54,   60,   60,
 /*   260 */    60,   54,   38,   60,   44,   38,   36,   60,   42,   35,
 /*   270 */    77,   40,   74,   74,   74,   77,   77,   74,   77,   77,
 /*   280 */    77,   74,
};
#define YY_SHIFT_USE_DFLT (-24)
#define YY_SHIFT_MAX 67
static const short yy_shift_ofst[] = {
 /*     0 */   -24,   17,   29,   80,   98,   98,   98,   98,   98,   98,
 /*    10 */    98,   98,   98,   98,   98,   98,   98,   98,  -23,   -6,
 /*    20 */    59,   -6,   -6,   -6,   20,   39,   90,   90,   75,   85,
 /*    30 */     1,    1,   90,  106,   85,  106,   94,  143,  112,  149,
 /*    40 */   165,  165,  189,  182,   89,   89,    5,   26,  154,  183,
 /*    50 */   160,  193,  158,  178,  210,  174,  171,  184,  201,  232,
 /*    60 */   212,  224,  220,  227,  230,  226,  234,  231,
};
#define YY_REDUCE_USE_DFLT (-57)
#define YY_REDUCE_MAX 35
static const short yy_reduce_ofst[] = {
 /*     0 */   -28,  104,  119,  176,  180,  -56,  -36,  -24,  -15,   69,
 /*    10 */   139,  146,  150,  155,  159,  163,  167,  172,  199,  198,
 /*    20 */    81,  207,  200,  203,   46,  -37,   -4,   11,   44,   74,
 /*    30 */    68,   77,   83,   92,  102,  109,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   124,  184,  184,  184,  184,  184,  184,  184,  184,  184,
 /*    10 */   184,  184,  184,  184,  184,  184,  184,  184,  184,  184,
 /*    20 */   184,  184,  184,  184,  184,  184,  149,  149,  184,  152,
 /*    30 */   184,  184,  184,  165,  152,  165,  184,  184,  184,  184,
 /*    40 */   162,  163,  172,  171,  174,  173,  184,  184,  184,  184,
 /*    50 */   184,  184,  184,  184,  184,  184,  184,  184,  184,  184,
 /*    60 */   184,  184,  127,  184,  184,  184,  125,  184,  178,  121,
 /*    70 */   122,  123,  139,  126,  127,  128,  129,  130,  131,  132,
 /*    80 */   133,  134,  135,  136,  137,  138,  140,  142,  144,  143,
 /*    90 */   141,  145,  153,  154,  155,  157,  166,  167,  168,  169,
 /*   100 */   170,  175,  176,  177,  179,  180,  181,  182,  183,  159,
 /*   110 */   158,  160,  161,  164,  156,  151,  147,  150,  148,  146,
};
#define YY_SZ_ACTTAB (int)(sizeof(yy_action)/sizeof(yy_action[0]))

/* The next table maps tokens into fallback tokens.  If a construct
** like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammar, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
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
*/
struct yyStackEntry {
  YYACTIONTYPE stateno;  /* The state-number */
  YYCODETYPE major;      /* The major token value.  This is the code
                         ** number for the token at this stack level */
  YYMINORTYPE minor;     /* The user-supplied minor token value.  This
                         ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  int yyidx;                    /* Index of top element in stack */
#ifdef YYTRACKMAXSTACKDEPTH
  int yyidxMax;                 /* Maximum value of yyidx */
#endif
  int yyerrcnt;                 /* Shifts left before out of the error */
  YALParserARG_SDECL                /* A place to hold %extra_argument */
#if YYSTACKDEPTH<=0
  int yystksz;                  /* Current side of the stack */
  yyStackEntry *yystack;        /* The parser's stack */
#else
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
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
  "var_decl",      "type_decl",     "type_array",    "type_var_decls",
  "type_var_decl",
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "module ::= decls END",
 /*   1 */ "decls ::= decls type_decl",
 /*   2 */ "decls ::= decls decl_function",
 /*   3 */ "decls ::= decls type_function_decl",
 /*   4 */ "decls ::=",
 /*   5 */ "type_specifier ::= type_builtin",
 /*   6 */ "type_specifier ::= type_array",
 /*   7 */ "type_specifier ::= IDENTIFIER",
 /*   8 */ "type_builtin ::= TYPE_BOOL",
 /*   9 */ "type_builtin ::= TYPE_INT8",
 /*  10 */ "type_builtin ::= TYPE_UINT8",
 /*  11 */ "type_builtin ::= TYPE_INT16",
 /*  12 */ "type_builtin ::= TYPE_UINT16",
 /*  13 */ "type_builtin ::= TYPE_INT32",
 /*  14 */ "type_builtin ::= TYPE_UINT32",
 /*  15 */ "type_builtin ::= TYPE_INT64",
 /*  16 */ "type_builtin ::= TYPE_UINT64",
 /*  17 */ "type_builtin ::= TYPE_FLOAT",
 /*  18 */ "type_builtin ::= TYPE_DOUBLE",
 /*  19 */ "type_array ::= type_builtin ARRAY_BEGIN ARRAY_END",
 /*  20 */ "type_decl ::= TYPE IDENTIFIER COLON STRUCT SCOPE_BEGIN type_var_decls SCOPE_END",
 /*  21 */ "type_decl ::= TYPE IDENTIFIER COLON type_specifier SEMI_COLON",
 /*  22 */ "type_var_decls ::= type_var_decls type_var_decl",
 /*  23 */ "type_var_decls ::= type_var_decl",
 /*  24 */ "type_var_decl ::= IDENTIFIER COLON type_specifier SEMI_COLON",
 /*  25 */ "decl_function ::= FUNCTION IDENTIFIER PAR_BEGIN function_args_decl PAR_END function_return_decl function_scope",
 /*  26 */ "type_function_decl ::= FUNCTION type_specifier COLON COLON IDENTIFIER PAR_BEGIN function_args_decl PAR_END function_return_decl function_scope",
 /*  27 */ "function_args_decl ::= function_args_decl COMMA function_arg_decl",
 /*  28 */ "function_args_decl ::= function_arg_decl",
 /*  29 */ "function_args_decl ::=",
 /*  30 */ "function_arg_decl ::= IDENTIFIER COLON type_specifier",
 /*  31 */ "function_return_decl ::= COLON type_specifier",
 /*  32 */ "function_return_decl ::=",
 /*  33 */ "function_scope ::= SCOPE_BEGIN SCOPE_END",
 /*  34 */ "function_scope ::= SCOPE_BEGIN statement_list SCOPE_END",
 /*  35 */ "statement_list ::= statement_list statement",
 /*  36 */ "statement_list ::= statement",
 /*  37 */ "statement ::= expression ASSIGN expression SEMI_COLON",
 /*  38 */ "statement ::= var_decl SEMI_COLON",
 /*  39 */ "statement ::= expression SEMI_COLON",
 /*  40 */ "statement ::= RETURN expression SEMI_COLON",
 /*  41 */ "statement ::= RETURN SEMI_COLON",
 /*  42 */ "var_decl ::= VAR IDENTIFIER var_type_spec ASSIGN expression",
 /*  43 */ "var_decl ::= LET IDENTIFIER var_type_spec ASSIGN expression",
 /*  44 */ "var_type_spec ::= COLON type_specifier",
 /*  45 */ "var_type_spec ::=",
 /*  46 */ "expression ::= PAR_BEGIN expression PAR_END",
 /*  47 */ "expression ::= literal",
 /*  48 */ "expression ::= unaryexp",
 /*  49 */ "expression ::= binaryexp",
 /*  50 */ "expression ::= IDENTIFIER",
 /*  51 */ "binaryexp ::= expression AND expression",
 /*  52 */ "binaryexp ::= expression OR expression",
 /*  53 */ "binaryexp ::= expression PLUS expression",
 /*  54 */ "binaryexp ::= expression MINUS expression",
 /*  55 */ "binaryexp ::= expression MULT expression",
 /*  56 */ "binaryexp ::= expression DIV expression",
 /*  57 */ "binaryexp ::= expression MOD expression",
 /*  58 */ "unaryexp ::= NOT expression",
 /*  59 */ "unaryexp ::= BIT_NOT expression",
 /*  60 */ "unaryexp ::= MINUS expression",
 /*  61 */ "literal ::= INTEGER_LITERAL",
 /*  62 */ "literal ::= DECIMAL_LITERAL",
 /*  63 */ "literal ::= BOOL_LITERAL",
};
#endif /* NDEBUG */


#if YYSTACKDEPTH<=0
/*
** Try to increase the size of the parser stack.
*/
static void yyGrowStack(yyParser *p){
  int newSize;
  yyStackEntry *pNew;

  newSize = p->yystksz*2 + 100;
  pNew = realloc(p->yystack, newSize*sizeof(pNew[0]));
  if( pNew ){
    p->yystack = pNew;
    p->yystksz = newSize;
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sStack grows to %d entries!\n",
              yyTracePrompt, p->yystksz);
    }
#endif
  }
}
#endif

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
void *YALParserAlloc(void *(*mallocProc)(size_t)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (size_t)sizeof(yyParser) );
  if( pParser ){
    pParser->yyidx = -1;
#ifdef YYTRACKMAXSTACKDEPTH
    pParser->yyidxMax = 0;
#endif
#if YYSTACKDEPTH<=0
    pParser->yystack = NULL;
    pParser->yystksz = 0;
    yyGrowStack(pParser);
#endif
  }
  return pParser;
}

/* The following function deletes the value associated with a
** symbol.  The symbol can be either a terminal or nonterminal.
** "yymajor" is the symbol code, and "yypminor" is a pointer to
** the value.
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
    ** which appear on the RHS of the rule, but which are not used
    ** inside the C code.
    */
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
**
** Return the major token number for the symbol popped.
*/
static int yy_pop_parser_stack(yyParser *pParser){
  YYCODETYPE yymajor;
  yyStackEntry *yytos = &pParser->yystack[pParser->yyidx];

  if( pParser->yyidx<0 ) return 0;
#ifndef NDEBUG
  if( yyTraceFILE && pParser->yyidx>=0 ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yymajor = yytos->major;
  yy_destructor(pParser, yymajor, &yytos->minor);
  pParser->yyidx--;
  return yymajor;
}

/* 
** Deallocate and destroy a parser.  Destructors are all called for
** all stack elements before shutting the parser down.
**
** Inputs:
** <ul>
** <li>  A pointer to the parser.  This should be a pointer
**       obtained from YALParserAlloc.
** <li>  A pointer to a function used to reclaim memory obtained
**       from malloc.
** </ul>
*/
void YALParserFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
  yyParser *pParser = (yyParser*)p;
  if( pParser==0 ) return;
  while( pParser->yyidx>=0 ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  free(pParser->yystack);
#endif
  (*freeProc)((void*)pParser);
}

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int YALParserStackPeak(void *p){
  yyParser *pParser = (yyParser*)p;
  return pParser->yyidxMax;
}
#endif

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
  int stateno = pParser->yystack[pParser->yyidx].stateno;
 
  if( stateno>YY_SHIFT_MAX || (i = yy_shift_ofst[stateno])==YY_SHIFT_USE_DFLT ){
    return yy_default[stateno];
  }
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
  if( i<0 || i>=YY_SZ_ACTTAB || yy_lookahead[i]!=iLookAhead ){
    if( iLookAhead>0 ){
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
        return yy_find_shift_action(pParser, iFallback);
      }
#endif
#ifdef YYWILDCARD
      {
        int j = i - iLookAhead + YYWILDCARD;
        if( j>=0 && j<YY_SZ_ACTTAB && yy_lookahead[j]==YYWILDCARD ){
#ifndef NDEBUG
          if( yyTraceFILE ){
            fprintf(yyTraceFILE, "%sWILDCARD %s => %s\n",
               yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[YYWILDCARD]);
          }
#endif /* NDEBUG */
          return yy_action[j];
        }
      }
#endif /* YYWILDCARD */
    }
    return yy_default[stateno];
  }else{
    return yy_action[i];
  }
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_reduce_action(
  int stateno,              /* Current state number */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
#ifdef YYERRORSYMBOL
  if( stateno>YY_REDUCE_MAX ){
    return yy_default[stateno];
  }
#else
  assert( stateno<=YY_REDUCE_MAX );
#endif
  i = yy_reduce_ofst[stateno];
  assert( i!=YY_REDUCE_USE_DFLT );
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
#ifdef YYERRORSYMBOL
  if( i<0 || i>=YY_SZ_ACTTAB || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }
#else
  assert( i>=0 && i<YY_SZ_ACTTAB );
  assert( yy_lookahead[i]==iLookAhead );
#endif
  return yy_action[i];
}

/*
** The following routine is called if the stack overflows.
*/
static void yyStackOverflow(yyParser *yypParser, YYMINORTYPE *yypMinor){
   YALParserARG_FETCH;
   yypParser->yyidx--;
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
   YALParserARG_STORE; /* Suppress warning about unused %extra_argument var */
}

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  YYMINORTYPE *yypMinor         /* Pointer to the minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yyidx++;
#ifdef YYTRACKMAXSTACKDEPTH
  if( yypParser->yyidx>yypParser->yyidxMax ){
    yypParser->yyidxMax = yypParser->yyidx;
  }
#endif
#if YYSTACKDEPTH>0 
  if( yypParser->yyidx>=YYSTACKDEPTH ){
    yyStackOverflow(yypParser, yypMinor);
    return;
  }
#else
  if( yypParser->yyidx>=yypParser->yystksz ){
    yyGrowStack(yypParser);
    if( yypParser->yyidx>=yypParser->yystksz ){
      yyStackOverflow(yypParser, yypMinor);
      return;
    }
  }
#endif
  yytos = &yypParser->yystack[yypParser->yyidx];
  yytos->stateno = (YYACTIONTYPE)yyNewState;
  yytos->major = (YYCODETYPE)yyMajor;
  yytos->minor = *yypMinor;
#ifndef NDEBUG
  if( yyTraceFILE && yypParser->yyidx>0 ){
    int i;
    fprintf(yyTraceFILE,"%sShift %d\n",yyTracePrompt,yyNewState);
    fprintf(yyTraceFILE,"%sStack:",yyTracePrompt);
    for(i=1; i<=yypParser->yyidx; i++)
      fprintf(yyTraceFILE," %s",yyTokenName[yypParser->yystack[i].major]);
    fprintf(yyTraceFILE,"\n");
  }
#endif
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;         /* Symbol on the left-hand side of the rule */
  unsigned char nrhs;     /* Number of right-hand side symbols in the rule */
} yyRuleInfo[] = {
  { 58, 2 },
  { 57, 2 },
  { 57, 2 },
  { 57, 2 },
  { 57, 0 },
  { 60, 1 },
  { 60, 1 },
  { 60, 1 },
  { 54, 1 },
  { 54, 1 },
  { 54, 1 },
  { 54, 1 },
  { 54, 1 },
  { 54, 1 },
  { 54, 1 },
  { 54, 1 },
  { 54, 1 },
  { 54, 1 },
  { 54, 1 },
  { 74, 3 },
  { 73, 7 },
  { 73, 5 },
  { 75, 2 },
  { 75, 1 },
  { 76, 4 },
  { 55, 7 },
  { 56, 10 },
  { 63, 3 },
  { 63, 1 },
  { 63, 0 },
  { 64, 3 },
  { 59, 2 },
  { 59, 0 },
  { 66, 2 },
  { 66, 3 },
  { 65, 2 },
  { 65, 1 },
  { 67, 4 },
  { 67, 2 },
  { 67, 2 },
  { 67, 3 },
  { 67, 2 },
  { 72, 5 },
  { 72, 5 },
  { 61, 2 },
  { 61, 0 },
  { 68, 3 },
  { 68, 1 },
  { 68, 1 },
  { 68, 1 },
  { 68, 1 },
  { 70, 3 },
  { 70, 3 },
  { 70, 3 },
  { 70, 3 },
  { 70, 3 },
  { 70, 3 },
  { 70, 3 },
  { 69, 2 },
  { 69, 2 },
  { 69, 2 },
  { 71, 1 },
  { 71, 1 },
  { 71, 1 },
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  int yyruleno                 /* Number of the rule by which to reduce */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  YYMINORTYPE yygotominor;        /* The LHS of the rule reduced */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  YALParserARG_FETCH;
  yymsp = &yypParser->yystack[yypParser->yyidx];
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno>=0 
        && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    fprintf(yyTraceFILE, "%sReduce [%s].\n", yyTracePrompt,
      yyRuleName[yyruleno]);
  }
#endif /* NDEBUG */

  /* Silence complaints from purify about yygotominor being uninitialized
  ** in some cases when it is copied into the stack after the following
  ** switch.  yygotominor is uninitialized when a rule reduces that does
  ** not set the value of its left-hand side nonterminal.  Leaving the
  ** value of the nonterminal uninitialized is utterly harmless as long
  ** as the value is never used.  So really the only thing this code
  ** accomplishes is to quieten purify.  
  **
  ** 2007-01-16:  The wireshark project (www.wireshark.org) reports that
  ** without this code, their parser segfaults.  I'm not sure what there
  ** parser is doing to make this happen.  This is the second bug report
  ** from wireshark this week.  Clearly they are stressing Lemon in ways
  ** that it has not been previously stressed...  (SQLite ticket #2172)
  */
  /*memset(&yygotominor, 0, sizeof(yygotominor));*/
  yygotominor = yyzerominor;


  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
      case 2: /* decls ::= decls decl_function */
{
        yygotominor.yy63 =yymsp[-1].minor.yy63; yymsp[-1].minor.yy63->addDecl(yymsp[0].minor.yy115);
}
        break;
      case 3: /* decls ::= decls type_function_decl */
{
        yygotominor.yy63 =yymsp[-1].minor.yy63; yymsp[-1].minor.yy63->addDecl(yymsp[0].minor.yy113);
}
        break;
      case 4: /* decls ::= */
{
        yygotominor.yy63 =  pParser->newAstNode<yal::DeclModule>();
        pParser->onAstNodeCreate(yygotominor.yy63);
     }
        break;
      case 5: /* type_specifier ::= type_builtin */
{yygotominor.yy116=yymsp[0].minor.yy111;}
        break;
      case 6: /* type_specifier ::= type_array */
      case 32: /* function_return_decl ::= */ yytestcase(yyruleno==32);
      case 45: /* var_type_spec ::= */ yytestcase(yyruleno==45);
{yygotominor.yy116 = nullptr;}
        break;
      case 7: /* type_specifier ::= IDENTIFIER */
{yygotominor.yy116 = pParser->newAstNode<yal::RefTypeIdentifier>(yymsp[0].minor.yy0.tokenStr);}
        break;
      case 8: /* type_builtin ::= TYPE_BOOL */
{
        yygotominor.yy111 = pParser->newAstNode<yal::RefTypeBuiltin>(pParser->getModule().getTypeContext().getTypeBuiltinBool());
        }
        break;
      case 9: /* type_builtin ::= TYPE_INT8 */
{
        yygotominor.yy111 = pParser->newAstNode<yal::RefTypeBuiltin>(pParser->getModule().getTypeContext().getTypeBuiltinI8());
        }
        break;
      case 10: /* type_builtin ::= TYPE_UINT8 */
{
        yygotominor.yy111  = pParser->newAstNode<yal::RefTypeBuiltin>(pParser->getModule().getTypeContext().getTypeBuiltinU8());
        }
        break;
      case 11: /* type_builtin ::= TYPE_INT16 */
{
        yygotominor.yy111 = pParser->newAstNode<yal::RefTypeBuiltin>(pParser->getModule().getTypeContext().getTypeBuiltinI16());
        }
        break;
      case 12: /* type_builtin ::= TYPE_UINT16 */
{
        yygotominor.yy111  = pParser->newAstNode<yal::RefTypeBuiltin>(pParser->getModule().getTypeContext().getTypeBuiltinU16());
        }
        break;
      case 13: /* type_builtin ::= TYPE_INT32 */
{
        yygotominor.yy111 = pParser->newAstNode<yal::RefTypeBuiltin>(pParser->getModule().getTypeContext().getTypeBuiltinI32());
        }
        break;
      case 14: /* type_builtin ::= TYPE_UINT32 */
{
        yygotominor.yy111 = pParser->newAstNode<yal::RefTypeBuiltin>(pParser->getModule().getTypeContext().getTypeBuiltinU32());
        }
        break;
      case 15: /* type_builtin ::= TYPE_INT64 */
{
        yygotominor.yy111 = pParser->newAstNode<yal::RefTypeBuiltin>(pParser->getModule().getTypeContext().getTypeBuiltinI64());
        }
        break;
      case 16: /* type_builtin ::= TYPE_UINT64 */
{
        yygotominor.yy111 = pParser->newAstNode<yal::RefTypeBuiltin>(pParser->getModule().getTypeContext().getTypeBuiltinU64());
        }
        break;
      case 17: /* type_builtin ::= TYPE_FLOAT */
{
        yygotominor.yy111 = pParser->newAstNode<yal::RefTypeBuiltin>(pParser->getModule().getTypeContext().getTypeBuiltinFloat());
        }
        break;
      case 18: /* type_builtin ::= TYPE_DOUBLE */
{
        yygotominor.yy111 = pParser->newAstNode<yal::RefTypeBuiltin>(pParser->getModule().getTypeContext().getTypeBuiltinDouble());
        }
        break;
      case 25: /* decl_function ::= FUNCTION IDENTIFIER PAR_BEGIN function_args_decl PAR_END function_return_decl function_scope */
{
        yygotominor.yy115 = pParser->newAstNode<yal::DeclFunction>(yymsp[-5].minor.yy0.tokenStr, yymsp[-3].minor.yy150, yymsp[-1].minor.yy116, yymsp[0].minor.yy66);
        }
        break;
      case 26: /* type_function_decl ::= FUNCTION type_specifier COLON COLON IDENTIFIER PAR_BEGIN function_args_decl PAR_END function_return_decl function_scope */
{
            yygotominor.yy113 = pParser->newAstNode<yal::DeclTypeFunction>(yymsp[-5].minor.yy0.tokenStr, yymsp[-3].minor.yy150, yymsp[-8].minor.yy116, yymsp[-1].minor.yy116, yymsp[0].minor.yy66);
        }
        break;
      case 27: /* function_args_decl ::= function_args_decl COMMA function_arg_decl */
{
        yymsp[-2].minor.yy150->addDeclParam(yymsp[0].minor.yy87);
        yygotominor.yy150 = yymsp[-2].minor.yy150;
}
        break;
      case 28: /* function_args_decl ::= function_arg_decl */
{
        yygotominor.yy150 = pParser->newAstNode<yal::DeclParamVarContainer>();
        yygotominor.yy150->addDeclParam(yymsp[0].minor.yy87);
}
        break;
      case 29: /* function_args_decl ::= */
{
        yygotominor.yy150 = nullptr;
}
        break;
      case 30: /* function_arg_decl ::= IDENTIFIER COLON type_specifier */
{
    yygotominor.yy87 = pParser->newAstNode<yal::DeclParamVar>(yymsp[-2].minor.yy0.tokenStr, yal::Qualifier(),yymsp[0].minor.yy116);
}
        break;
      case 31: /* function_return_decl ::= COLON type_specifier */
{ yygotominor.yy116 = yymsp[0].minor.yy116;}
        break;
      case 33: /* function_scope ::= SCOPE_BEGIN SCOPE_END */
{ yygotominor.yy66 = nullptr;}
        break;
      case 34: /* function_scope ::= SCOPE_BEGIN statement_list SCOPE_END */
{yygotominor.yy66 = yymsp[-1].minor.yy66;}
        break;
      case 35: /* statement_list ::= statement_list statement */
{
    yymsp[-1].minor.yy66->addStatement(yymsp[0].minor.yy38);
    yygotominor.yy66=yymsp[-1].minor.yy66;
}
        break;
      case 36: /* statement_list ::= statement */
{
    yygotominor.yy66 = pParser->newAstNode<yal::StatementList>();
    yygotominor.yy66->addStatement(yymsp[0].minor.yy38);
}
        break;
      case 37: /* statement ::= expression ASSIGN expression SEMI_COLON */
{
   yygotominor.yy38 = pParser->newAstNode<yal::StmtAssign>(yymsp[-3].minor.yy147,yymsp[-1].minor.yy147);
}
        break;
      case 38: /* statement ::= var_decl SEMI_COLON */
{
    yygotominor.yy38 = yymsp[-1].minor.yy59;
}
        break;
      case 39: /* statement ::= expression SEMI_COLON */
{yygotominor.yy38 = yymsp[-1].minor.yy147;}
        break;
      case 40: /* statement ::= RETURN expression SEMI_COLON */
{
    yygotominor.yy38 = pParser->newAstNode<yal::StmtReturn>(yymsp[-1].minor.yy147);
}
        break;
      case 41: /* statement ::= RETURN SEMI_COLON */
{
    yygotominor.yy38 = pParser->newAstNode<yal::StmtReturn>();
}
        break;
      case 42: /* var_decl ::= VAR IDENTIFIER var_type_spec ASSIGN expression */
{
    auto varDecl = pParser->newAstNode<yal::DeclVar>(yymsp[-3].minor.yy0.tokenStr, yal::Qualifier(), yymsp[-2].minor.yy116, yymsp[0].minor.yy147);
    yygotominor.yy59= pParser->newAstNode<yal::StmtDecl>(varDecl);
}
        break;
      case 43: /* var_decl ::= LET IDENTIFIER var_type_spec ASSIGN expression */
{
    yal::Qualifier qualifier;
    qualifier.setImmutable();
    auto varDecl = pParser->newAstNode<yal::DeclVar>(yymsp[-3].minor.yy0.tokenStr, qualifier, yymsp[-2].minor.yy116, yymsp[0].minor.yy147);
    yygotominor.yy59= pParser->newAstNode<yal::StmtDecl>(varDecl);
}
        break;
      case 44: /* var_type_spec ::= COLON type_specifier */
{yygotominor.yy116 = yymsp[0].minor.yy116;}
        break;
      case 46: /* expression ::= PAR_BEGIN expression PAR_END */
{yygotominor.yy147 = yymsp[-1].minor.yy147;}
        break;
      case 47: /* expression ::= literal */
{yygotominor.yy147 = yymsp[0].minor.yy147;}
        break;
      case 48: /* expression ::= unaryexp */
{yygotominor.yy147 = yymsp[0].minor.yy25;}
        break;
      case 49: /* expression ::= binaryexp */
{yygotominor.yy147 = yymsp[0].minor.yy125;}
        break;
      case 50: /* expression ::= IDENTIFIER */
{
    yygotominor.yy147 = pParser->newAstNode<yal::ExprVarRef>(yymsp[0].minor.yy0.tokenStr);
}
        break;
      case 51: /* binaryexp ::= expression AND expression */
{
        yygotominor.yy125 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::And,
                                                        yymsp[-2].minor.yy147, yymsp[0].minor.yy147);
}
        break;
      case 52: /* binaryexp ::= expression OR expression */
{
        yygotominor.yy125 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Or,
                                                        yymsp[-2].minor.yy147, yymsp[0].minor.yy147);
}
        break;
      case 53: /* binaryexp ::= expression PLUS expression */
{
        yygotominor.yy125 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Plus,
                                                        yymsp[-2].minor.yy147, yymsp[0].minor.yy147);
}
        break;
      case 54: /* binaryexp ::= expression MINUS expression */
{
        yygotominor.yy125 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Minus,
                                                        yymsp[-2].minor.yy147, yymsp[0].minor.yy147);
}
        break;
      case 55: /* binaryexp ::= expression MULT expression */
{
        yygotominor.yy125 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Mult,
                                                        yymsp[-2].minor.yy147, yymsp[0].minor.yy147);
}
        break;
      case 56: /* binaryexp ::= expression DIV expression */
{
        yygotominor.yy125 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Div,
                                                        yymsp[-2].minor.yy147, yymsp[0].minor.yy147);
}
        break;
      case 57: /* binaryexp ::= expression MOD expression */
{
        yygotominor.yy125 = pParser->newAstNode<yal::ExprBinaryOperator>(yal::BinaryOperatorType::Mod,
                                                        yymsp[-2].minor.yy147, yymsp[0].minor.yy147);
}
        break;
      case 58: /* unaryexp ::= NOT expression */
{
    yygotominor.yy25 = pParser->newAstNode<yal::ExprUnaryOperator>(yal::UnaryOperatorType::Not,
                                               yymsp[0].minor.yy147);
}
        break;
      case 59: /* unaryexp ::= BIT_NOT expression */
{
    yygotominor.yy25 = pParser->newAstNode<yal::ExprUnaryOperator>(yal::UnaryOperatorType::BitNot,
                                               yymsp[0].minor.yy147);
}
        break;
      case 60: /* unaryexp ::= MINUS expression */
{
    yygotominor.yy25 = pParser->newAstNode<yal::ExprUnaryOperator>(yal::UnaryOperatorType::Negate,
                                               yymsp[0].minor.yy147);
}
        break;
      case 61: /* literal ::= INTEGER_LITERAL */
{
        yygotominor.yy147 = pParser->newIntegerLiteral(yymsp[0].minor.yy0);
   }
        break;
      case 63: /* literal ::= BOOL_LITERAL */
{
        yygotominor.yy147 = pParser->newAstNode<yal::ExprBoolLiteral>(yymsp[0].minor.yy0.tokenStr);
}
        break;
      default:
      /* (0) module ::= decls END */ yytestcase(yyruleno==0);
      /* (1) decls ::= decls type_decl */ yytestcase(yyruleno==1);
      /* (19) type_array ::= type_builtin ARRAY_BEGIN ARRAY_END */ yytestcase(yyruleno==19);
      /* (20) type_decl ::= TYPE IDENTIFIER COLON STRUCT SCOPE_BEGIN type_var_decls SCOPE_END */ yytestcase(yyruleno==20);
      /* (21) type_decl ::= TYPE IDENTIFIER COLON type_specifier SEMI_COLON */ yytestcase(yyruleno==21);
      /* (22) type_var_decls ::= type_var_decls type_var_decl */ yytestcase(yyruleno==22);
      /* (23) type_var_decls ::= type_var_decl */ yytestcase(yyruleno==23);
      /* (24) type_var_decl ::= IDENTIFIER COLON type_specifier SEMI_COLON */ yytestcase(yyruleno==24);
      /* (62) literal ::= DECIMAL_LITERAL */ yytestcase(yyruleno==62);
        break;
  };
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yypParser->yyidx -= yysize;
  yyact = yy_find_reduce_action(yymsp[-yysize].stateno,(YYCODETYPE)yygoto);
  if( yyact < YYNSTATE ){
#ifdef NDEBUG
    /* If we are not debugging and the reduce action popped at least
    ** one element off the stack, then we can push the new element back
    ** onto the stack here, and skip the stack overflow test in yy_shift().
    ** That gives a significant speed improvement. */
    if( yysize ){
      yypParser->yyidx++;
      yymsp -= yysize-1;
      yymsp->stateno = (YYACTIONTYPE)yyact;
      yymsp->major = (YYCODETYPE)yygoto;
      yymsp->minor = yygotominor;
    }else
#endif
    {
      yy_shift(yypParser,yyact,yygoto,&yygotominor);
    }
  }else{
    assert( yyact == YYNSTATE + YYNRULE + 1 );
    yy_accept(yypParser);
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
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
  YALParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  YYMINORTYPE yyminor            /* The minor type of the error token */
){
  YALParserARG_FETCH;
#define TOKEN (yyminor.yy0)


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
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
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
  int yyact;            /* The parser action. */
  int yyendofinput;     /* True if we are at the end of input */
#ifdef YYERRORSYMBOL
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
#endif
  yyParser *yypParser;  /* The parser */

  /* (re)initialize the parser, if necessary */
  yypParser = (yyParser*)yyp;
  if( yypParser->yyidx<0 ){
#if YYSTACKDEPTH<=0
    if( yypParser->yystksz <=0 ){
      /*memset(&yyminorunion, 0, sizeof(yyminorunion));*/
      yyminorunion = yyzerominor;
      yyStackOverflow(yypParser, &yyminorunion);
      return;
    }
#endif
    yypParser->yyidx = 0;
    yypParser->yyerrcnt = -1;
    yypParser->yystack[0].stateno = 0;
    yypParser->yystack[0].major = 0;
  }
  yyminorunion.yy0 = yyminor;
  yyendofinput = (yymajor==0);
  YALParserARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput %s\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,(YYCODETYPE)yymajor);
    if( yyact<YYNSTATE ){
      assert( !yyendofinput );  /* Impossible to shift the $ token */
      yy_shift(yypParser,yyact,yymajor,&yyminorunion);
      yypParser->yyerrcnt--;
      yymajor = YYNOCODE;
    }else if( yyact < YYNSTATE + YYNRULE ){
      yy_reduce(yypParser,yyact-YYNSTATE);
    }else{
      assert( yyact == YY_ERROR_ACTION );
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
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yymx = yypParser->yystack[yypParser->yyidx].major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yypParser, (YYCODETYPE)yymajor,&yyminorunion);
        yymajor = YYNOCODE;
      }else{
         while(
          yypParser->yyidx >= 0 &&
          yymx != YYERRORSYMBOL &&
          (yyact = yy_find_reduce_action(
                        yypParser->yystack[yypParser->yyidx].stateno,
                        YYERRORSYMBOL)) >= YYNSTATE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yyidx < 0 || yymajor==0 ){
          yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          YYMINORTYPE u2;
          u2.YYERRSYMDT = 0;
          yy_shift(yypParser,yyact,YYERRORSYMBOL,&u2);
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
      yy_syntax_error(yypParser,yymajor,yyminorunion);
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
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
      }
      yymajor = YYNOCODE;
#endif
    }
  }while( yymajor!=YYNOCODE && yypParser->yyidx>=0 );
  return;
}
