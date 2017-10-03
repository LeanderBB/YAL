/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is included that follows the "include" declaration
** in the input grammar file. */
#include <stdio.h>

#include "yal/parser/parser.h"
#include "yal/util/log.h"
#include <cassert>
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
#define YAL_TOKEN_COLON                          36
#define YAL_TOKEN_TYPE                           37
#define YAL_TOKEN_SCOPE_BEGIN                    38
#define YAL_TOKEN_SCOPE_END                      39
#define YAL_TOKEN_SEMI_COLON                     40
#define YAL_TOKEN_FUNCTION                       41
#define YAL_TOKEN_PAR_BEGIN                      42
#define YAL_TOKEN_PAR_END                        43
#define YAL_TOKEN_COMMA                          44
#define YAL_TOKEN_VAR                            45
#define YAL_TOKEN_LET                            46
#define YAL_TOKEN_INTEGER_LITERAL                47
#define YAL_TOKEN_DECIMAL_LITERAL                48
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
#define YYNOCODE 72
#define YYACTIONTYPE unsigned char
#if INTERFACE
#define YALParserTOKENTYPE void*
#endif
typedef union {
  int yyinit;
  YALParserTOKENTYPE yy0;
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
#define YYNSTATE 109
#define YYNRULE 56
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
 /*     0 */    75,   76,   77,   78,   79,   80,   81,   82,   83,   84,
 /*    10 */    85,   86,   53,   76,   77,   78,   79,   80,   81,   82,
 /*    20 */    83,   84,   85,   86,   10,   17,   15,   18,   19,   20,
 /*    30 */    21,    9,    7,  104,   66,   46,   18,   19,   20,   21,
 /*    40 */     9,   10,   17,   15,   48,    3,   57,   64,   74,  105,
 /*    50 */    64,   74,   51,   18,   19,   20,   21,    9,   10,   17,
 /*    60 */    15,   91,   92,   66,   46,    2,   99,   52,   55,   71,
 /*    70 */    18,   19,   20,   21,    9,   15,    1,   60,   48,  107,
 /*    80 */    64,   74,   67,   95,  106,   18,   19,   20,   21,    9,
 /*    90 */   103,   64,   74,  166,    8,   90,   20,   21,    9,   50,
 /*   100 */    16,   52,   55,   63,   62,  101,  102,   49,   64,   74,
 /*   110 */    13,   68,   69,   70,   56,   38,   36,  100,  100,   40,
 /*   120 */    47,  100,   34,    6,  100,   25,   88,   51,   39,   42,
 /*   130 */   100,  100,   41,   96,  100,  100,   97,   89,  100,   43,
 /*   140 */    44,   14,  108,  108,   31,   27,   29,   27,   72,   22,
 /*   150 */    94,   73,   98,   93,  100,   37,   33,  100,  100,   35,
 /*   160 */    87,  100,    5,   65,    4,   24,   32,  109,   59,   23,
 /*   170 */    12,   45,   30,   11,   58,   54,   26,   61,   28,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    22,   23,   24,   25,   26,   27,   28,   29,   30,   31,
 /*    10 */    32,   33,   22,   23,   24,   25,   26,   27,   28,   29,
 /*    20 */    30,   31,   32,   33,    1,    2,    3,   13,   14,   15,
 /*    30 */    16,   17,   64,   65,   66,   67,   13,   14,   15,   16,
 /*    40 */    17,    1,    2,    3,   22,   36,   55,   56,   57,   55,
 /*    50 */    56,   57,   22,   13,   14,   15,   16,   17,    1,    2,
 /*    60 */     3,   39,   65,   66,   67,   36,   43,   45,   46,   39,
 /*    70 */    13,   14,   15,   16,   17,    3,   38,   19,   22,   55,
 /*    80 */    56,   57,   62,   43,   63,   13,   14,   15,   16,   17,
 /*    90 */    55,   56,   57,   50,   51,   39,   15,   16,   17,   22,
 /*   100 */    42,   45,   46,   21,   22,   47,   48,   55,   56,   57,
 /*   110 */     1,   52,   53,   54,   36,   68,   68,   70,   70,   68,
 /*   120 */    37,   70,   68,   41,   70,   58,   59,   22,   68,   68,
 /*   130 */    70,   70,   68,   68,   70,   70,   68,   62,   70,   60,
 /*   140 */    60,    1,   63,   63,   43,   44,   43,   44,   59,   38,
 /*   150 */    40,   35,   68,   40,   70,   68,   68,   70,   70,   68,
 /*   160 */    40,   70,   36,   34,   36,   42,   22,    0,   22,   42,
 /*   170 */     1,   36,   22,   42,   36,   69,   61,   69,   61,
};
#define YY_SHIFT_USE_DFLT (-23)
#define YY_SHIFT_MAX 66
static const short yy_shift_ofst[] = {
 /*     0 */   -23,   56,  -22,  -22,  -22,  -22,  -10,   22,   82,   58,
 /*    10 */    58,   58,   58,   58,   58,   58,   58,   58,   58,   58,
 /*    20 */    58,   58,  105,   77,   77,   30,   38,   77,   38,   29,
 /*    30 */     9,   29,    9,   23,   40,   57,   57,   57,   57,   72,
 /*    40 */    14,   81,   81,  101,  103,   83,  110,  111,  140,  120,
 /*    50 */   126,  128,  144,  127,  169,  150,  138,   78,  146,  123,
 /*    60 */   131,  109,  135,  167,  129,  116,  113,
};
#define YY_REDUCE_USE_DFLT (-33)
#define YY_REDUCE_MAX 32
static const signed char yy_reduce_ofst[] = {
 /*     0 */    43,  -32,   -6,   35,   52,   24,   -9,   -3,   59,   84,
 /*    10 */    87,   88,   91,   47,   48,   51,   54,   60,   61,   64,
 /*    20 */    65,   68,   67,   79,   80,   89,   75,   21,   20,  117,
 /*    30 */   108,  115,  106,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   113,  165,  165,  165,  165,  165,  165,  165,  165,  165,
 /*    10 */   165,  165,  165,  165,  165,  165,  165,  165,  165,  165,
 /*    20 */   165,  165,  165,  137,  137,  165,  165,  165,  165,  140,
 /*    30 */   151,  140,  151,  165,  165,  148,  147,  161,  149,  154,
 /*    40 */   153,  157,  156,  165,  165,  165,  165,  165,  165,  165,
 /*    50 */   165,  165,  165,  116,  165,  165,  165,  165,  165,  165,
 /*    60 */   165,  165,  165,  165,  114,  165,  165,  134,  110,  111,
 /*    70 */   112,  129,  130,  128,  115,  116,  117,  118,  119,  120,
 /*    80 */   121,  122,  123,  124,  125,  126,  127,  132,  131,  133,
 /*    90 */   141,  142,  143,  145,  146,  152,  158,  159,  160,  155,
 /*   100 */   162,  163,  164,  150,  144,  139,  135,  138,  136,
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
  "DIV",           "MOD",           "NEG",           "NOT",         
  "DOT",           "END",           "IDENTIFIER",    "TYPE_BOOL",   
  "TYPE_INT8",     "TYPE_UINT8",    "TYPE_INT16",    "TYPE_UINT16", 
  "TYPE_INT32",    "TYPE_UINT32",   "TYPE_INT64",    "TYPE_UINT64", 
  "TYPE_FLOAT",    "TYPE_DOUBLE",   "ARRAY_BEGIN",   "ARRAY_END",   
  "COLON",         "TYPE",          "SCOPE_BEGIN",   "SCOPE_END",   
  "SEMI_COLON",    "FUNCTION",      "PAR_BEGIN",     "PAR_END",     
  "COMMA",         "VAR",           "LET",           "INTEGER_LITERAL",
  "DECIMAL_LITERAL",  "error",         "prog",          "translation_unit",
  "type_decl",     "function_decl",  "type_function_decl",  "type_specifier",
  "type_basic",    "type_array",    "type_var_decls",  "type_var_decl",
  "function_args_decl",  "function_return_decl",  "function_scope",  "function_arg_decl",
  "statement_list",  "statement",     "var_assignment",  "var_decl",    
  "expression",    "var_type_spec",  "literal",     
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "prog ::= translation_unit END",
 /*   1 */ "translation_unit ::= translation_unit type_decl",
 /*   2 */ "translation_unit ::= translation_unit function_decl",
 /*   3 */ "translation_unit ::= translation_unit type_function_decl",
 /*   4 */ "translation_unit ::=",
 /*   5 */ "type_specifier ::= type_basic",
 /*   6 */ "type_specifier ::= type_array",
 /*   7 */ "type_specifier ::= IDENTIFIER",
 /*   8 */ "type_basic ::= TYPE_BOOL",
 /*   9 */ "type_basic ::= TYPE_INT8",
 /*  10 */ "type_basic ::= TYPE_UINT8",
 /*  11 */ "type_basic ::= TYPE_INT16",
 /*  12 */ "type_basic ::= TYPE_UINT16",
 /*  13 */ "type_basic ::= TYPE_INT32",
 /*  14 */ "type_basic ::= TYPE_UINT32",
 /*  15 */ "type_basic ::= TYPE_INT64",
 /*  16 */ "type_basic ::= TYPE_UINT64",
 /*  17 */ "type_basic ::= TYPE_FLOAT",
 /*  18 */ "type_basic ::= TYPE_DOUBLE",
 /*  19 */ "type_array ::= type_basic ARRAY_BEGIN ARRAY_END",
 /*  20 */ "type_decl ::= IDENTIFIER COLON TYPE SCOPE_BEGIN type_var_decls SCOPE_END",
 /*  21 */ "type_var_decls ::= type_var_decls type_var_decl",
 /*  22 */ "type_var_decls ::= type_var_decl",
 /*  23 */ "type_var_decl ::= IDENTIFIER COLON type_specifier SEMI_COLON",
 /*  24 */ "function_decl ::= FUNCTION IDENTIFIER PAR_BEGIN function_args_decl PAR_END function_return_decl function_scope",
 /*  25 */ "type_function_decl ::= FUNCTION type_specifier COLON COLON IDENTIFIER PAR_BEGIN function_args_decl PAR_END function_return_decl function_scope",
 /*  26 */ "function_args_decl ::= function_args_decl COMMA function_arg_decl",
 /*  27 */ "function_args_decl ::= function_arg_decl",
 /*  28 */ "function_args_decl ::=",
 /*  29 */ "function_arg_decl ::= IDENTIFIER COLON type_specifier",
 /*  30 */ "function_return_decl ::= COLON type_specifier",
 /*  31 */ "function_return_decl ::=",
 /*  32 */ "function_scope ::= SCOPE_BEGIN SCOPE_END",
 /*  33 */ "function_scope ::= SCOPE_BEGIN statement_list SCOPE_END",
 /*  34 */ "statement_list ::= statement_list statement",
 /*  35 */ "statement_list ::= statement",
 /*  36 */ "statement ::= var_assignment SEMI_COLON",
 /*  37 */ "statement ::= var_decl SEMI_COLON",
 /*  38 */ "var_assignment ::= IDENTIFIER ASSIGN expression",
 /*  39 */ "var_decl ::= VAR IDENTIFIER var_type_spec ASSIGN expression",
 /*  40 */ "var_decl ::= LET IDENTIFIER var_type_spec ASSIGN expression",
 /*  41 */ "var_type_spec ::= COLON type_specifier",
 /*  42 */ "var_type_spec ::=",
 /*  43 */ "expression ::= PAR_BEGIN expression PAR_END",
 /*  44 */ "expression ::= expression AND expression",
 /*  45 */ "expression ::= expression OR expression",
 /*  46 */ "expression ::= NOT PAR_BEGIN expression PAR_END",
 /*  47 */ "expression ::= expression PLUS expression",
 /*  48 */ "expression ::= expression MINUS expression",
 /*  49 */ "expression ::= expression MULT expression",
 /*  50 */ "expression ::= expression DIV expression",
 /*  51 */ "expression ::= expression MOD expression",
 /*  52 */ "expression ::= expression ASSIGN expression",
 /*  53 */ "expression ::= literal",
 /*  54 */ "literal ::= INTEGER_LITERAL",
 /*  55 */ "literal ::= DECIMAL_LITERAL",
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
  { 50, 2 },
  { 51, 2 },
  { 51, 2 },
  { 51, 2 },
  { 51, 0 },
  { 55, 1 },
  { 55, 1 },
  { 55, 1 },
  { 56, 1 },
  { 56, 1 },
  { 56, 1 },
  { 56, 1 },
  { 56, 1 },
  { 56, 1 },
  { 56, 1 },
  { 56, 1 },
  { 56, 1 },
  { 56, 1 },
  { 56, 1 },
  { 57, 3 },
  { 52, 6 },
  { 58, 2 },
  { 58, 1 },
  { 59, 4 },
  { 53, 7 },
  { 54, 10 },
  { 60, 3 },
  { 60, 1 },
  { 60, 0 },
  { 63, 3 },
  { 61, 2 },
  { 61, 0 },
  { 62, 2 },
  { 62, 3 },
  { 64, 2 },
  { 64, 1 },
  { 65, 2 },
  { 65, 2 },
  { 66, 3 },
  { 67, 5 },
  { 67, 5 },
  { 69, 2 },
  { 69, 0 },
  { 68, 3 },
  { 68, 3 },
  { 68, 3 },
  { 68, 4 },
  { 68, 3 },
  { 68, 3 },
  { 68, 3 },
  { 68, 3 },
  { 68, 3 },
  { 68, 3 },
  { 68, 1 },
  { 70, 1 },
  { 70, 1 },
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
      default:
      /* (0) prog ::= translation_unit END */ yytestcase(yyruleno==0);
      /* (1) translation_unit ::= translation_unit type_decl */ yytestcase(yyruleno==1);
      /* (2) translation_unit ::= translation_unit function_decl */ yytestcase(yyruleno==2);
      /* (3) translation_unit ::= translation_unit type_function_decl */ yytestcase(yyruleno==3);
      /* (4) translation_unit ::= */ yytestcase(yyruleno==4);
      /* (5) type_specifier ::= type_basic */ yytestcase(yyruleno==5);
      /* (6) type_specifier ::= type_array */ yytestcase(yyruleno==6);
      /* (7) type_specifier ::= IDENTIFIER */ yytestcase(yyruleno==7);
      /* (8) type_basic ::= TYPE_BOOL */ yytestcase(yyruleno==8);
      /* (9) type_basic ::= TYPE_INT8 */ yytestcase(yyruleno==9);
      /* (10) type_basic ::= TYPE_UINT8 */ yytestcase(yyruleno==10);
      /* (11) type_basic ::= TYPE_INT16 */ yytestcase(yyruleno==11);
      /* (12) type_basic ::= TYPE_UINT16 */ yytestcase(yyruleno==12);
      /* (13) type_basic ::= TYPE_INT32 */ yytestcase(yyruleno==13);
      /* (14) type_basic ::= TYPE_UINT32 */ yytestcase(yyruleno==14);
      /* (15) type_basic ::= TYPE_INT64 */ yytestcase(yyruleno==15);
      /* (16) type_basic ::= TYPE_UINT64 */ yytestcase(yyruleno==16);
      /* (17) type_basic ::= TYPE_FLOAT */ yytestcase(yyruleno==17);
      /* (18) type_basic ::= TYPE_DOUBLE */ yytestcase(yyruleno==18);
      /* (19) type_array ::= type_basic ARRAY_BEGIN ARRAY_END */ yytestcase(yyruleno==19);
      /* (20) type_decl ::= IDENTIFIER COLON TYPE SCOPE_BEGIN type_var_decls SCOPE_END */ yytestcase(yyruleno==20);
      /* (21) type_var_decls ::= type_var_decls type_var_decl */ yytestcase(yyruleno==21);
      /* (22) type_var_decls ::= type_var_decl */ yytestcase(yyruleno==22);
      /* (23) type_var_decl ::= IDENTIFIER COLON type_specifier SEMI_COLON */ yytestcase(yyruleno==23);
      /* (24) function_decl ::= FUNCTION IDENTIFIER PAR_BEGIN function_args_decl PAR_END function_return_decl function_scope */ yytestcase(yyruleno==24);
      /* (25) type_function_decl ::= FUNCTION type_specifier COLON COLON IDENTIFIER PAR_BEGIN function_args_decl PAR_END function_return_decl function_scope */ yytestcase(yyruleno==25);
      /* (26) function_args_decl ::= function_args_decl COMMA function_arg_decl */ yytestcase(yyruleno==26);
      /* (27) function_args_decl ::= function_arg_decl */ yytestcase(yyruleno==27);
      /* (28) function_args_decl ::= */ yytestcase(yyruleno==28);
      /* (29) function_arg_decl ::= IDENTIFIER COLON type_specifier */ yytestcase(yyruleno==29);
      /* (30) function_return_decl ::= COLON type_specifier */ yytestcase(yyruleno==30);
      /* (31) function_return_decl ::= */ yytestcase(yyruleno==31);
      /* (32) function_scope ::= SCOPE_BEGIN SCOPE_END */ yytestcase(yyruleno==32);
      /* (33) function_scope ::= SCOPE_BEGIN statement_list SCOPE_END */ yytestcase(yyruleno==33);
      /* (34) statement_list ::= statement_list statement */ yytestcase(yyruleno==34);
      /* (35) statement_list ::= statement */ yytestcase(yyruleno==35);
      /* (36) statement ::= var_assignment SEMI_COLON */ yytestcase(yyruleno==36);
      /* (37) statement ::= var_decl SEMI_COLON */ yytestcase(yyruleno==37);
      /* (38) var_assignment ::= IDENTIFIER ASSIGN expression */ yytestcase(yyruleno==38);
      /* (39) var_decl ::= VAR IDENTIFIER var_type_spec ASSIGN expression */ yytestcase(yyruleno==39);
      /* (40) var_decl ::= LET IDENTIFIER var_type_spec ASSIGN expression */ yytestcase(yyruleno==40);
      /* (41) var_type_spec ::= COLON type_specifier */ yytestcase(yyruleno==41);
      /* (42) var_type_spec ::= */ yytestcase(yyruleno==42);
      /* (43) expression ::= PAR_BEGIN expression PAR_END */ yytestcase(yyruleno==43);
      /* (44) expression ::= expression AND expression */ yytestcase(yyruleno==44);
      /* (45) expression ::= expression OR expression */ yytestcase(yyruleno==45);
      /* (46) expression ::= NOT PAR_BEGIN expression PAR_END */ yytestcase(yyruleno==46);
      /* (47) expression ::= expression PLUS expression */ yytestcase(yyruleno==47);
      /* (48) expression ::= expression MINUS expression */ yytestcase(yyruleno==48);
      /* (49) expression ::= expression MULT expression */ yytestcase(yyruleno==49);
      /* (50) expression ::= expression DIV expression */ yytestcase(yyruleno==50);
      /* (51) expression ::= expression MOD expression */ yytestcase(yyruleno==51);
      /* (52) expression ::= expression ASSIGN expression */ yytestcase(yyruleno==52);
      /* (53) expression ::= literal */ yytestcase(yyruleno==53);
      /* (54) literal ::= INTEGER_LITERAL */ yytestcase(yyruleno==54);
      /* (55) literal ::= DECIMAL_LITERAL */ yytestcase(yyruleno==55);
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
    pParser->getLog().error("Expected one of the following tokens:\n");
    {
        yal::Log::MultiLineScope multiLineScope(pParser->getLog());
        int n = sizeof(yyTokenName) / sizeof(yyTokenName[0]);
        for (int i = 0; i < n; ++i) {
            int a = yy_find_shift_action(yypParser, (YYCODETYPE)i);
            if (a < YYNSTATE + YYNRULE) {
                pParser->getLog().error("%s \n",yyTokenName[i]);
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
