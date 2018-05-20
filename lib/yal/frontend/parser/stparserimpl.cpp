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
#line 23 "stparserimpl.lemon"


#include "yal/frontend/lexer/lexer.h"
#include "yal/frontend/parser/stparser.h"
#include "yal/frontend/parser/syntaxtreenodes.h"
#include "yal/util/log.h"
#include "yal/util/stringref.h"


#include <cassert>
#include <vector>
#define YYMALLOCARGTYPE size_t
#line 41 "stparserimpl.c"
#include "stparserimpl.h"
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
#define YAL_TOKEN_SELF                           24
#define YAL_TOKEN_TYPE_BOOL                      25
#define YAL_TOKEN_TYPE_INT8                      26
#define YAL_TOKEN_TYPE_UINT8                     27
#define YAL_TOKEN_TYPE_INT16                     28
#define YAL_TOKEN_TYPE_UINT16                    29
#define YAL_TOKEN_TYPE_INT32                     30
#define YAL_TOKEN_TYPE_UINT32                    31
#define YAL_TOKEN_TYPE_INT64                     32
#define YAL_TOKEN_TYPE_UINT64                    33
#define YAL_TOKEN_TYPE_FLOAT                     34
#define YAL_TOKEN_TYPE_DOUBLE                    35
#define YAL_TOKEN_ARRAY_BEGIN                    36
#define YAL_TOKEN_ARRAY_END                      37
#define YAL_TOKEN_MUT                            38
#define YAL_TOKEN_REFERENCE                      39
#define YAL_TOKEN_TYPE                           40
#define YAL_TOKEN_COLON                          41
#define YAL_TOKEN_SEMI_COLON                     42
#define YAL_TOKEN_STRUCT                         43
#define YAL_TOKEN_SCOPE_BEGIN                    44
#define YAL_TOKEN_SCOPE_END                      45
#define YAL_TOKEN_COMMA                          46
#define YAL_TOKEN_PAR_BEGIN                      47
#define YAL_TOKEN_PAR_END                        48
#define YAL_TOKEN_FUNCTION                       49
#define YAL_TOKEN_RETURN                         50
#define YAL_TOKEN_VAR                            51
#define YAL_TOKEN_LET                            52
#define YAL_TOKEN_RANGE_CAST                     53
#define YAL_TOKEN_INTEGER_LITERAL                54
#define YAL_TOKEN_INTEGER_LITERAL_I8             55
#define YAL_TOKEN_INTEGER_LITERAL_I16            56
#define YAL_TOKEN_INTEGER_LITERAL_I32            57
#define YAL_TOKEN_INTEGER_LITERAL_I64            58
#define YAL_TOKEN_INTEGER_LITERAL_U8             59
#define YAL_TOKEN_INTEGER_LITERAL_U16            60
#define YAL_TOKEN_INTEGER_LITERAL_U32            61
#define YAL_TOKEN_INTEGER_LITERAL_U64            62
#define YAL_TOKEN_FLOAT_LITERAL                  63
#define YAL_TOKEN_FLOAT_LITERAL_32               64
#define YAL_TOKEN_FLOAT_LITERAL_64               65
#define YAL_TOKEN_BOOL_LITERAL                   66
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
**    YALSTParserTOKENTYPE     is the data type used for minor type for terminal
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
**                       which is YALSTParserTOKENTYPE.  The entry in the union
**                       for terminal symbols is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    YALSTParserARG_SDECL     A static variable declaration for the %extra_argument
**    YALSTParserARG_PDECL     A parameter declaration for the %extra_argument
**    YALSTParserARG_STORE     Code to store %extra_argument into yypParser
**    YALSTParserARG_FETCH     Code to extract %extra_argument from yypParser
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
#define YYNOCODE 103
#define YYACTIONTYPE unsigned short int
#if INTERFACE
#define YALSTParserTOKENTYPE yal::frontend::TokenInfo
#endif
typedef union {
  int yyinit;
  YALSTParserTOKENTYPE yy0;
  yal::frontend::STDeclVar* yy5;
  yal::frontend::STExprStructInit* yy24;
  yal::frontend::STStructMember* yy33;
  yal::frontend::STQualType* yy41;
  yal::frontend::STStatementList* yy42;
  yal::frontend::STExpression* yy44;
  yal::frontend::STDeclStruct::Members* yy68;
  yal::frontend::STStatement* yy69;
  yal::frontend::STDeclFunction* yy74;
  yal::frontend::STExprStructInit::MemberInitList* yy96;
  yal::frontend::STStructMemberInit* yy109;
  yal::frontend::STExprUnaryOperator* yy118;
  yal::frontend::STDeclStruct* yy145;
  yal::frontend::STIdentifier* yy147;
  yal::frontend::STDecl* yy152;
  yal::frontend::STDeclModule* yy154;
  yal::StringRefPod yy155;
  yal::frontend::STDeclFunction::Params* yy166;
  yal::frontend::STDeclParam* yy173;
  uint32_t yy177;
  yal::frontend::STExprFnCall::ParamList* yy192;
  yal::frontend::STType* yy198;
  yal::frontend::STExprBinaryOperator* yy200;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#if INTERFACE
#define YALSTParserARG_SDECL  yal::frontend::STParser *pParser ;
#define YALSTParserARG_PDECL , yal::frontend::STParser *pParser 
#define YALSTParserARG_FETCH  yal::frontend::STParser *pParser  = yypParser->pParser 
#define YALSTParserARG_STORE yypParser->pParser  = pParser 
#endif
#define YYNSTATE             121
#define YYNRULE              107
#define YY_MAX_SHIFT         120
#define YY_MIN_SHIFTREDUCE   192
#define YY_MAX_SHIFTREDUCE   298
#define YY_MIN_REDUCE        299
#define YY_MAX_REDUCE        405
#define YY_ERROR_ACTION      406
#define YY_ACCEPT_ACTION     407
#define YY_NO_ACTION         408
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
#define YY_ACTTAB_COUNT (1056)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    13,  118,   16,  116,  256,   14,   15,  196,  321,   86,
 /*    10 */   197,  203,  204,  205,  206,  207,  208,  209,  210,  211,
 /*    20 */   212,  213,  113,  216,  118,   12,   91,  196,   58,   25,
 /*    30 */    24,   23,  308,   31,   40,   55,    7,   53,   51,  108,
 /*    40 */   284,  285,  286,  287,  288,  289,  290,  291,  292,  293,
 /*    50 */   294,  295,  296,   13,   89,  308,   34,  407,   14,   15,
 /*    60 */   113,  216,   86,  197,  203,  204,  205,  206,  207,  208,
 /*    70 */   209,  210,  211,  212,  213,  299,  300,  301,   12,   57,
 /*    80 */   112,  243,  327,  118,   96,  325,   31,   33,   85,  336,
 /*    90 */   279,   45,  108,  284,  285,  286,  287,  288,  289,  290,
 /*   100 */   291,  292,  293,  294,  295,  296,   48,   13,   47,   82,
 /*   110 */   389,  120,   14,   15,  308,   41,   86,  197,  203,  204,
 /*   120 */   205,  206,  207,  208,  209,  210,  211,  212,  213,   59,
 /*   130 */   221,   46,   12,   57,  112,  340,   92,   36,   32,  353,
 /*   140 */    31,   36,  107,  335,   36,   93,  108,  284,  285,  286,
 /*   150 */   287,  288,  289,  290,  291,  292,  293,  294,  295,  296,
 /*   160 */    10,   28,   29,   16,   50,  257,   22,   21,   17,   19,
 /*   170 */    18,   20,   27,   26,   25,   24,   23,  196,   52,  114,
 /*   180 */    55,  101,  114,  118,   96,   84,  109,  110,  306,  361,
 /*   190 */    94,  329,  113,  216,  330,   87,  338,  331,   36,   36,
 /*   200 */   111,  241,  197,    3,   88,  345,   60,  357,  358,  356,
 /*   210 */   388,  359,  339,   16,  308,  258,   37,  118,  225,   84,
 /*   220 */    56,  110,  306,  361,   94,  102,  309,  119,   38,  309,
 /*   230 */    49,  227,    1,   90,    8,    9,  240,    3,   95,  345,
 /*   240 */    60,  357,  358,  356,  226,  359,   28,   29,  308,   11,
 /*   250 */    55,   22,   21,   17,   19,   18,   20,   27,   26,   25,
 /*   260 */    24,   23,   28,   29,    4,   55,   54,   22,   21,   17,
 /*   270 */    19,   18,   20,   27,   26,   25,   24,   23,   30,  106,
 /*   280 */    43,   55,   42,    5,   28,   29,  242,    6,  217,   22,
 /*   290 */    21,   17,   19,   18,   20,   27,   26,   25,   24,   23,
 /*   300 */    28,   29,  239,   55,   44,   22,   21,   17,   19,   18,
 /*   310 */    20,   27,   26,   25,   24,   23,  118,    2,   84,   55,
 /*   320 */   110,  306,  361,   94,   39,  219,  298,  117,   35,  404,
 /*   330 */   253,  301,  301,  301,  301,  301,  301,  301,  344,   60,
 /*   340 */   357,  358,  356,  301,  359,  301,  248,  308,  301,   28,
 /*   350 */    29,  301,  301,  301,   22,   21,   17,   19,   18,   20,
 /*   360 */    27,   26,   25,   24,   23,  301,  301,  301,   55,  301,
 /*   370 */   301,  202,  301,  203,  204,  205,  206,  207,  208,  209,
 /*   380 */   210,  211,  212,  213,  301,  301,  301,  301,  301,  301,
 /*   390 */   301,  115,  118,  301,   84,   29,  110,  306,  361,   22,
 /*   400 */    21,   17,   19,   18,   20,   27,   26,   25,   24,   23,
 /*   410 */   301,  301,  301,   55,  301,   69,  357,  358,  356,   83,
 /*   420 */   359,  301,  301,  308,  301,  118,  301,   84,  301,  110,
 /*   430 */   306,  361,  301,  301,  118,  301,   84,  301,  110,  306,
 /*   440 */   361,  301,  301,  301,  301,  301,  301,  301,   69,  357,
 /*   450 */   358,  356,   81,  359,  301,  301,  308,   69,  357,  358,
 /*   460 */   356,   80,  359,  301,   86,  308,  203,  204,  205,  206,
 /*   470 */   207,  208,  209,  210,  211,  212,  213,  202,  301,  203,
 /*   480 */   204,  205,  206,  207,  208,  209,  210,  211,  212,  213,
 /*   490 */   406,  406,  406,  406,  406,  406,   27,   26,   25,   24,
 /*   500 */    23,  301,  301,  301,   55,   22,   21,   17,   19,   18,
 /*   510 */    20,   27,   26,   25,   24,   23,  301,  301,  301,   55,
 /*   520 */   118,  301,   84,  301,  110,  306,  361,  301,  301,  118,
 /*   530 */   301,   84,  301,  110,  306,  361,  301,  301,  301,  301,
 /*   540 */   301,  301,  301,   61,  357,  358,  356,  301,  359,  301,
 /*   550 */   301,  308,   65,  357,  358,  356,  301,  359,  301,  118,
 /*   560 */   308,   84,  301,  110,  306,  361,  301,  301,  301,  118,
 /*   570 */   301,   84,  301,  110,  306,  361,  301,  301,  301,  301,
 /*   580 */   301,  301,   66,  357,  358,  356,  301,  359,  301,  301,
 /*   590 */   308,  301,   62,  357,  358,  356,  301,  359,  301,  301,
 /*   600 */   308,  301,  118,  301,   84,  301,  110,  306,  361,  301,
 /*   610 */   301,  301,  301,  301,  118,  301,   84,  301,  110,  306,
 /*   620 */   361,  301,  301,  301,  301,   67,  357,  358,  356,  301,
 /*   630 */   359,  301,  301,  308,  301,  301,  301,   97,  357,  358,
 /*   640 */   356,  301,  359,  301,  118,  308,   84,  301,  110,  306,
 /*   650 */   361,  301,  301,  118,  301,   84,  301,  110,  306,  361,
 /*   660 */   301,  301,  301,  301,  301,  301,  301,   98,  357,  358,
 /*   670 */   356,  301,  359,  301,  301,  308,   99,  357,  358,  356,
 /*   680 */   301,  359,  301,  301,  308,  118,  301,   84,  301,  110,
 /*   690 */   306,  361,  301,  301,  301,  301,  118,  301,   84,  301,
 /*   700 */   110,  306,  361,  301,  301,  301,  301,  301,  100,  357,
 /*   710 */   358,  356,  301,  359,  301,  301,  308,  301,  301,   68,
 /*   720 */   357,  358,  356,  301,  359,  301,  118,  308,   84,  301,
 /*   730 */   110,  306,  361,  301,  301,  301,  301,  301,  118,  301,
 /*   740 */    84,  301,  110,  306,  361,  301,  301,  301,  301,   71,
 /*   750 */   357,  358,  356,  301,  359,  301,  301,  308,  301,  301,
 /*   760 */   301,   72,  357,  358,  356,  301,  359,  301,  301,  308,
 /*   770 */   301,  118,  301,   84,  301,  110,  306,  361,  301,  301,
 /*   780 */   118,  301,   84,  301,  110,  306,  361,  301,  301,  301,
 /*   790 */   301,  301,  301,  301,   73,  357,  358,  356,  301,  359,
 /*   800 */   301,  301,  308,   74,  357,  358,  356,  301,  359,  301,
 /*   810 */   301,  308,  301,  118,  301,   84,  301,  110,  306,  361,
 /*   820 */   301,  301,  118,  301,   84,  301,  110,  306,  361,  301,
 /*   830 */   301,  301,  301,  301,  301,  301,   75,  357,  358,  356,
 /*   840 */   301,  359,  301,  301,  308,   76,  357,  358,  356,  301,
 /*   850 */   359,  301,  301,  308,  301,  118,  301,   84,  301,  110,
 /*   860 */   306,  361,  301,  301,  301,  118,  301,   84,  301,  110,
 /*   870 */   306,  361,  301,  301,  301,  301,  301,  301,  103,  357,
 /*   880 */   358,  356,  301,  359,  301,  301,  308,  301,  104,  357,
 /*   890 */   358,  356,  301,  359,  301,  118,  308,   84,  301,  110,
 /*   900 */   306,  361,  301,  301,  301,  301,  301,  118,  301,   84,
 /*   910 */   301,  110,  306,  361,  301,  301,  301,  301,  105,  357,
 /*   920 */   358,  356,  301,  359,  301,  301,  308,  301,  301,  301,
 /*   930 */    78,  357,  358,  356,  301,  359,  301,  301,  308,  301,
 /*   940 */   118,  301,   84,  301,  110,  306,  361,  301,  301,  118,
 /*   950 */   301,   84,  301,  110,  306,  361,  301,  301,  301,  301,
 /*   960 */   301,  301,  301,   79,  357,  358,  356,  301,  359,  301,
 /*   970 */   301,  308,   70,  357,  358,  356,  301,  359,  301,  301,
 /*   980 */   308,  301,  118,  301,   84,  301,  110,  306,  361,  301,
 /*   990 */   301,  118,  301,   84,  301,  110,  306,  361,  301,  301,
 /*  1000 */   301,  301,  301,  301,  301,   77,  357,  358,  356,  301,
 /*  1010 */   359,  301,  301,  308,   63,  357,  358,  356,  301,  359,
 /*  1020 */   301,  301,  308,  301,  118,  301,   84,  301,  110,  306,
 /*  1030 */   361,  301,  301,  301,  301,  301,  301,  301,  301,  301,
 /*  1040 */   301,  301,  301,  301,  301,  301,  301,   64,  357,  358,
 /*  1050 */   356,  301,  359,  301,  301,  308,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    14,   70,   46,   72,   48,   19,   20,   23,   24,   23,
 /*    10 */    24,   25,   26,   27,   28,   29,   30,   31,   32,   33,
 /*    20 */    34,   35,   38,   39,   70,   39,   72,   23,   74,   15,
 /*    30 */    16,   17,  101,   47,   41,   21,   50,   51,   52,   53,
 /*    40 */    54,   55,   56,   57,   58,   59,   60,   61,   62,   63,
 /*    50 */    64,   65,   66,   14,   81,  101,   68,   69,   19,   20,
 /*    60 */    38,   39,   23,   24,   25,   26,   27,   28,   29,   30,
 /*    70 */    31,   32,   33,   34,   35,   78,   79,   80,   39,   73,
 /*    80 */    74,   42,   85,   70,   74,   72,   47,   47,   82,   83,
 /*    90 */    45,   46,   53,   54,   55,   56,   57,   58,   59,   60,
 /*   100 */    61,   62,   63,   64,   65,   66,   74,   14,   84,   99,
 /*   110 */   100,   22,   19,   20,  101,   41,   23,   24,   25,   26,
 /*   120 */    27,   28,   29,   30,   31,   32,   33,   34,   35,   40,
 /*   130 */    45,   46,   39,   73,   74,   71,   88,   73,   49,   71,
 /*   140 */    47,   73,   71,   83,   73,   88,   53,   54,   55,   56,
 /*   150 */    57,   58,   59,   60,   61,   62,   63,   64,   65,   66,
 /*   160 */     1,    2,    3,   46,   74,   48,    7,    8,    9,   10,
 /*   170 */    11,   12,   13,   14,   15,   16,   17,   23,   74,   74,
 /*   180 */    21,   74,   74,   70,   74,   72,   74,   74,   75,   76,
 /*   190 */    77,   86,   38,   39,   86,   87,   71,   71,   73,   73,
 /*   200 */    81,   42,   24,   90,   91,   92,   93,   94,   95,   96,
 /*   210 */   100,   98,   75,   46,  101,   48,   46,   70,   48,   72,
 /*   220 */    84,   74,   75,   76,   77,   41,   41,   74,   44,   44,
 /*   230 */    41,   45,   44,   41,    1,    1,   42,   90,   91,   92,
 /*   240 */    93,   94,   95,   96,   45,   98,    2,    3,  101,   41,
 /*   250 */    21,    7,    8,    9,   10,   11,   12,   13,   14,   15,
 /*   260 */    16,   17,    2,    3,   47,   21,   41,    7,    8,    9,
 /*   270 */    10,   11,   12,   13,   14,   15,   16,   17,   47,    9,
 /*   280 */    41,   21,   10,   47,    2,    3,   42,   47,   39,    7,
 /*   290 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*   300 */     2,    3,   42,   21,   41,    7,    8,    9,   10,   11,
 /*   310 */    12,   13,   14,   15,   16,   17,   70,   44,   72,   21,
 /*   320 */    74,   75,   76,   77,   44,   42,   37,   36,   41,    0,
 /*   330 */    48,  102,  102,  102,  102,  102,  102,  102,   92,   93,
 /*   340 */    94,   95,   96,  102,   98,  102,   48,  101,  102,    2,
 /*   350 */     3,  102,  102,  102,    7,    8,    9,   10,   11,   12,
 /*   360 */    13,   14,   15,   16,   17,  102,  102,  102,   21,  102,
 /*   370 */   102,   23,  102,   25,   26,   27,   28,   29,   30,   31,
 /*   380 */    32,   33,   34,   35,  102,  102,  102,  102,  102,  102,
 /*   390 */   102,   43,   70,  102,   72,    3,   74,   75,   76,    7,
 /*   400 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*   410 */   102,  102,  102,   21,  102,   93,   94,   95,   96,   97,
 /*   420 */    98,  102,  102,  101,  102,   70,  102,   72,  102,   74,
 /*   430 */    75,   76,  102,  102,   70,  102,   72,  102,   74,   75,
 /*   440 */    76,  102,  102,  102,  102,  102,  102,  102,   93,   94,
 /*   450 */    95,   96,   97,   98,  102,  102,  101,   93,   94,   95,
 /*   460 */    96,   97,   98,  102,   23,  101,   25,   26,   27,   28,
 /*   470 */    29,   30,   31,   32,   33,   34,   35,   23,  102,   25,
 /*   480 */    26,   27,   28,   29,   30,   31,   32,   33,   34,   35,
 /*   490 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*   500 */    17,  102,  102,  102,   21,    7,    8,    9,   10,   11,
 /*   510 */    12,   13,   14,   15,   16,   17,  102,  102,  102,   21,
 /*   520 */    70,  102,   72,  102,   74,   75,   76,  102,  102,   70,
 /*   530 */   102,   72,  102,   74,   75,   76,  102,  102,  102,  102,
 /*   540 */   102,  102,  102,   93,   94,   95,   96,  102,   98,  102,
 /*   550 */   102,  101,   93,   94,   95,   96,  102,   98,  102,   70,
 /*   560 */   101,   72,  102,   74,   75,   76,  102,  102,  102,   70,
 /*   570 */   102,   72,  102,   74,   75,   76,  102,  102,  102,  102,
 /*   580 */   102,  102,   93,   94,   95,   96,  102,   98,  102,  102,
 /*   590 */   101,  102,   93,   94,   95,   96,  102,   98,  102,  102,
 /*   600 */   101,  102,   70,  102,   72,  102,   74,   75,   76,  102,
 /*   610 */   102,  102,  102,  102,   70,  102,   72,  102,   74,   75,
 /*   620 */    76,  102,  102,  102,  102,   93,   94,   95,   96,  102,
 /*   630 */    98,  102,  102,  101,  102,  102,  102,   93,   94,   95,
 /*   640 */    96,  102,   98,  102,   70,  101,   72,  102,   74,   75,
 /*   650 */    76,  102,  102,   70,  102,   72,  102,   74,   75,   76,
 /*   660 */   102,  102,  102,  102,  102,  102,  102,   93,   94,   95,
 /*   670 */    96,  102,   98,  102,  102,  101,   93,   94,   95,   96,
 /*   680 */   102,   98,  102,  102,  101,   70,  102,   72,  102,   74,
 /*   690 */    75,   76,  102,  102,  102,  102,   70,  102,   72,  102,
 /*   700 */    74,   75,   76,  102,  102,  102,  102,  102,   93,   94,
 /*   710 */    95,   96,  102,   98,  102,  102,  101,  102,  102,   93,
 /*   720 */    94,   95,   96,  102,   98,  102,   70,  101,   72,  102,
 /*   730 */    74,   75,   76,  102,  102,  102,  102,  102,   70,  102,
 /*   740 */    72,  102,   74,   75,   76,  102,  102,  102,  102,   93,
 /*   750 */    94,   95,   96,  102,   98,  102,  102,  101,  102,  102,
 /*   760 */   102,   93,   94,   95,   96,  102,   98,  102,  102,  101,
 /*   770 */   102,   70,  102,   72,  102,   74,   75,   76,  102,  102,
 /*   780 */    70,  102,   72,  102,   74,   75,   76,  102,  102,  102,
 /*   790 */   102,  102,  102,  102,   93,   94,   95,   96,  102,   98,
 /*   800 */   102,  102,  101,   93,   94,   95,   96,  102,   98,  102,
 /*   810 */   102,  101,  102,   70,  102,   72,  102,   74,   75,   76,
 /*   820 */   102,  102,   70,  102,   72,  102,   74,   75,   76,  102,
 /*   830 */   102,  102,  102,  102,  102,  102,   93,   94,   95,   96,
 /*   840 */   102,   98,  102,  102,  101,   93,   94,   95,   96,  102,
 /*   850 */    98,  102,  102,  101,  102,   70,  102,   72,  102,   74,
 /*   860 */    75,   76,  102,  102,  102,   70,  102,   72,  102,   74,
 /*   870 */    75,   76,  102,  102,  102,  102,  102,  102,   93,   94,
 /*   880 */    95,   96,  102,   98,  102,  102,  101,  102,   93,   94,
 /*   890 */    95,   96,  102,   98,  102,   70,  101,   72,  102,   74,
 /*   900 */    75,   76,  102,  102,  102,  102,  102,   70,  102,   72,
 /*   910 */   102,   74,   75,   76,  102,  102,  102,  102,   93,   94,
 /*   920 */    95,   96,  102,   98,  102,  102,  101,  102,  102,  102,
 /*   930 */    93,   94,   95,   96,  102,   98,  102,  102,  101,  102,
 /*   940 */    70,  102,   72,  102,   74,   75,   76,  102,  102,   70,
 /*   950 */   102,   72,  102,   74,   75,   76,  102,  102,  102,  102,
 /*   960 */   102,  102,  102,   93,   94,   95,   96,  102,   98,  102,
 /*   970 */   102,  101,   93,   94,   95,   96,  102,   98,  102,  102,
 /*   980 */   101,  102,   70,  102,   72,  102,   74,   75,   76,  102,
 /*   990 */   102,   70,  102,   72,  102,   74,   75,   76,  102,  102,
 /*  1000 */   102,  102,  102,  102,  102,   93,   94,   95,   96,  102,
 /*  1010 */    98,  102,  102,  101,   93,   94,   95,   96,  102,   98,
 /*  1020 */   102,  102,  101,  102,   70,  102,   72,  102,   74,   75,
 /*  1030 */    76,  102,  102,  102,  102,  102,  102,  102,  102,  102,
 /*  1040 */   102,  102,  102,  102,  102,  102,  102,   93,   94,   95,
 /*  1050 */    96,  102,   98,  102,  102,  101,
};
#define YY_SHIFT_USE_DFLT (1056)
#define YY_SHIFT_COUNT    (120)
#define YY_SHIFT_MIN      (-44)
#define YY_SHIFT_MAX      (498)
static const short yy_shift_ofst[] = {
 /*     0 */  1056,  -14,  -14,  -14,   93,   93,   93,   39,   93,   93,
 /*    10 */    93,   93,   93,   93,   93,   93,   93,   93,   93,   93,
 /*    20 */    93,   93,   93,   93,   93,   93,   93,   93,   93,   93,
 /*    30 */    93,   93,  441,  -16,   89,  348,  454,  154,    4,    4,
 /*    40 */    22,   22,   22,   22,   22,    4,    4,   -7,   40,    4,
 /*    50 */    74,    4,   74,    4,    4,    4,   -7,  178,   40,    4,
 /*    60 */   159,  244,  260,  282,  298,  347,  347,  347,  347,  347,
 /*    70 */   392,  483,  483,  483,  483,  483,  483,  498,   14,   14,
 /*    80 */   -44,  117,   45,  167,  184,  170,  185,   85,  186,  188,
 /*    90 */   189,  192,  233,  234,  194,  199,  208,  229,  229,  229,
 /*   100 */   229,  217,  225,  229,  229,  229,  231,  270,  272,  236,
 /*   110 */   240,  273,  239,  249,  263,  280,  283,  289,  291,  287,
 /*   120 */   329,
};
#define YY_REDUCE_USE_DFLT (-70)
#define YY_REDUCE_COUNT (59)
#define YY_REDUCE_MIN   (-69)
#define YY_REDUCE_MAX   (954)
static const short yy_reduce_ofst[] = {
 /*     0 */   -12,  113,  147,  246,  322,  355,  364,  450,  459,  489,
 /*    10 */   499,  532,  544,  574,  583,  615,  626,  656,  668,  701,
 /*    20 */   710,  743,  752,  785,  795,  825,  837,  870,  879,  912,
 /*    30 */   921,  954,  -46,    6,   -3,  -69,   13,   60,   10,  108,
 /*    40 */    64,   68,   71,  125,  126,  110,  105,  -27,   24,   32,
 /*    50 */    48,   90,   57,  104,  107,  112,  119,  137,  136,  153,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   302,  342,  342,  343,  385,  385,  385,  406,  406,  406,
 /*    10 */   406,  406,  406,  406,  406,  406,  406,  406,  406,  406,
 /*    20 */   406,  406,  406,  406,  406,  406,  406,  406,  406,  406,
 /*    30 */   406,  406,  406,  337,  406,  406,  406,  321,  387,  406,
 /*    40 */   321,  321,  321,  321,  321,  406,  406,  341,  406,  406,
 /*    50 */   354,  406,  354,  406,  406,  406,  341,  406,  406,  406,
 /*    60 */   406,  406,  406,  406,  406,  352,  351,  390,  383,  384,
 /*    70 */   367,  378,  377,  376,  375,  374,  373,  366,  369,  368,
 /*    80 */   406,  406,  406,  406,  406,  406,  303,  406,  406,  406,
 /*    90 */   406,  406,  406,  406,  406,  406,  406,  382,  381,  380,
 /*   100 */   379,  406,  406,  372,  371,  370,  406,  406,  406,  362,
 /*   110 */   305,  406,  406,  322,  406,  406,  406,  406,  307,  406,
 /*   120 */   406,
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
  YALSTParserARG_SDECL                /* A place to hold %extra_argument */
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
void YALSTParserTrace(FILE *TraceFILE, char *zTracePrompt){
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
  "SELF",          "TYPE_BOOL",     "TYPE_INT8",     "TYPE_UINT8",  
  "TYPE_INT16",    "TYPE_UINT16",   "TYPE_INT32",    "TYPE_UINT32", 
  "TYPE_INT64",    "TYPE_UINT64",   "TYPE_FLOAT",    "TYPE_DOUBLE", 
  "ARRAY_BEGIN",   "ARRAY_END",     "MUT",           "REFERENCE",   
  "TYPE",          "COLON",         "SEMI_COLON",    "STRUCT",      
  "SCOPE_BEGIN",   "SCOPE_END",     "COMMA",         "PAR_BEGIN",   
  "PAR_END",       "FUNCTION",      "RETURN",        "VAR",         
  "LET",           "RANGE_CAST",    "INTEGER_LITERAL",  "INTEGER_LITERAL_I8",
  "INTEGER_LITERAL_I16",  "INTEGER_LITERAL_I32",  "INTEGER_LITERAL_I64",  "INTEGER_LITERAL_U8",
  "INTEGER_LITERAL_U16",  "INTEGER_LITERAL_U32",  "INTEGER_LITERAL_U64",  "FLOAT_LITERAL",
  "FLOAT_LITERAL_32",  "FLOAT_LITERAL_64",  "BOOL_LITERAL",  "error",       
  "decls",         "module",        "type_builtin",  "type_qualified",
  "type_specifier",  "qualifier",     "identifier",    "self",        
  "identifier_or_self",  "decl_var",      "decl_type",     "decl_function",
  "decl_type_function",  "function_return_decl",  "function_args_decl",  "function_arg_decl",
  "function_param_list",  "decl_struct",   "struct_decl_var",  "struct_decl_vars",
  "var_type_spec",  "named_decl",    "statement_list",  "statement_list_or_empty",
  "statement",     "expression",    "unaryexp",      "binaryexp",   
  "literal",       "function_call_args",  "struct_init",   "struct_member_init_list",
  "struct_member_init",  "type_array",  
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "decls ::= decls decl_type",
 /*   1 */ "decls ::= decls decl_function",
 /*   2 */ "decls ::= decls decl_type_function",
 /*   3 */ "decls ::=",
 /*   4 */ "identifier ::= IDENTIFIER",
 /*   5 */ "self ::= SELF",
 /*   6 */ "identifier_or_self ::= identifier",
 /*   7 */ "identifier_or_self ::= self",
 /*   8 */ "type_specifier ::= type_builtin",
 /*   9 */ "type_specifier ::= type_array",
 /*  10 */ "type_specifier ::= IDENTIFIER",
 /*  11 */ "type_builtin ::= TYPE_BOOL",
 /*  12 */ "type_builtin ::= TYPE_INT8",
 /*  13 */ "type_builtin ::= TYPE_UINT8",
 /*  14 */ "type_builtin ::= TYPE_INT16",
 /*  15 */ "type_builtin ::= TYPE_UINT16",
 /*  16 */ "type_builtin ::= TYPE_INT32",
 /*  17 */ "type_builtin ::= TYPE_UINT32",
 /*  18 */ "type_builtin ::= TYPE_INT64",
 /*  19 */ "type_builtin ::= TYPE_UINT64",
 /*  20 */ "type_builtin ::= TYPE_FLOAT",
 /*  21 */ "type_builtin ::= TYPE_DOUBLE",
 /*  22 */ "qualifier ::=",
 /*  23 */ "qualifier ::= MUT",
 /*  24 */ "qualifier ::= REFERENCE",
 /*  25 */ "qualifier ::= MUT REFERENCE",
 /*  26 */ "type_qualified ::= qualifier type_specifier",
 /*  27 */ "decl_type ::= TYPE identifier COLON type_specifier SEMI_COLON",
 /*  28 */ "decl_type ::= decl_struct",
 /*  29 */ "decl_struct ::= TYPE identifier COLON STRUCT SCOPE_BEGIN struct_decl_vars SCOPE_END",
 /*  30 */ "struct_decl_vars ::= struct_decl_vars COMMA struct_decl_var",
 /*  31 */ "struct_decl_vars ::= struct_decl_var",
 /*  32 */ "struct_decl_var ::= identifier COLON type_qualified",
 /*  33 */ "function_param_list ::= PAR_BEGIN function_args_decl PAR_END",
 /*  34 */ "decl_function ::= FUNCTION identifier function_param_list function_return_decl SCOPE_BEGIN statement_list_or_empty SCOPE_END",
 /*  35 */ "decl_type_function ::= FUNCTION type_specifier COLON COLON identifier function_param_list function_return_decl SCOPE_BEGIN statement_list_or_empty SCOPE_END",
 /*  36 */ "function_args_decl ::= function_args_decl COMMA function_arg_decl",
 /*  37 */ "function_args_decl ::= function_arg_decl",
 /*  38 */ "function_args_decl ::=",
 /*  39 */ "function_arg_decl ::= identifier COLON type_qualified",
 /*  40 */ "function_arg_decl ::= qualifier self",
 /*  41 */ "function_return_decl ::= COLON type_qualified",
 /*  42 */ "function_return_decl ::=",
 /*  43 */ "statement_list_or_empty ::=",
 /*  44 */ "statement_list_or_empty ::= statement_list",
 /*  45 */ "statement_list ::= statement_list statement",
 /*  46 */ "statement_list ::= statement",
 /*  47 */ "statement ::= expression ASSIGN expression SEMI_COLON",
 /*  48 */ "statement ::= decl_var SEMI_COLON",
 /*  49 */ "statement ::= expression SEMI_COLON",
 /*  50 */ "statement ::= RETURN expression SEMI_COLON",
 /*  51 */ "statement ::= RETURN SEMI_COLON",
 /*  52 */ "decl_var ::= VAR identifier var_type_spec ASSIGN expression",
 /*  53 */ "decl_var ::= LET identifier var_type_spec ASSIGN expression",
 /*  54 */ "var_type_spec ::= COLON type_qualified",
 /*  55 */ "var_type_spec ::=",
 /*  56 */ "expression ::= PAR_BEGIN expression PAR_END",
 /*  57 */ "expression ::= literal",
 /*  58 */ "expression ::= unaryexp",
 /*  59 */ "expression ::= binaryexp",
 /*  60 */ "expression ::= struct_init",
 /*  61 */ "expression ::= RANGE_CAST LT type_qualified GT PAR_BEGIN expression PAR_END",
 /*  62 */ "expression ::= identifier_or_self",
 /*  63 */ "expression ::= expression DOT identifier",
 /*  64 */ "expression ::= identifier PAR_BEGIN function_call_args PAR_END",
 /*  65 */ "expression ::= expression DOT identifier PAR_BEGIN function_call_args PAR_END",
 /*  66 */ "expression ::= type_specifier COLON COLON identifier PAR_BEGIN function_call_args PAR_END",
 /*  67 */ "binaryexp ::= expression AND expression",
 /*  68 */ "binaryexp ::= expression OR expression",
 /*  69 */ "binaryexp ::= expression PLUS expression",
 /*  70 */ "binaryexp ::= expression MINUS expression",
 /*  71 */ "binaryexp ::= expression MULT expression",
 /*  72 */ "binaryexp ::= expression DIV expression",
 /*  73 */ "binaryexp ::= expression MOD expression",
 /*  74 */ "binaryexp ::= expression EQ expression",
 /*  75 */ "binaryexp ::= expression NE expression",
 /*  76 */ "binaryexp ::= expression LE expression",
 /*  77 */ "binaryexp ::= expression LT expression",
 /*  78 */ "binaryexp ::= expression GE expression",
 /*  79 */ "binaryexp ::= expression GT expression",
 /*  80 */ "unaryexp ::= NOT expression",
 /*  81 */ "unaryexp ::= BIT_NOT expression",
 /*  82 */ "unaryexp ::= MINUS expression",
 /*  83 */ "unaryexp ::= REFERENCE expression",
 /*  84 */ "function_call_args ::= function_call_args COMMA expression",
 /*  85 */ "function_call_args ::= expression",
 /*  86 */ "function_call_args ::=",
 /*  87 */ "struct_init ::= type_specifier SCOPE_BEGIN struct_member_init_list SCOPE_END",
 /*  88 */ "struct_member_init_list ::=",
 /*  89 */ "struct_member_init_list ::= struct_member_init_list COMMA struct_member_init",
 /*  90 */ "struct_member_init_list ::= struct_member_init",
 /*  91 */ "struct_member_init ::= identifier COLON expression",
 /*  92 */ "literal ::= INTEGER_LITERAL",
 /*  93 */ "literal ::= INTEGER_LITERAL_I8",
 /*  94 */ "literal ::= INTEGER_LITERAL_I16",
 /*  95 */ "literal ::= INTEGER_LITERAL_I32",
 /*  96 */ "literal ::= INTEGER_LITERAL_I64",
 /*  97 */ "literal ::= INTEGER_LITERAL_U8",
 /*  98 */ "literal ::= INTEGER_LITERAL_U16",
 /*  99 */ "literal ::= INTEGER_LITERAL_U32",
 /* 100 */ "literal ::= INTEGER_LITERAL_U64",
 /* 101 */ "literal ::= FLOAT_LITERAL",
 /* 102 */ "literal ::= FLOAT_LITERAL_32",
 /* 103 */ "literal ::= FLOAT_LITERAL_64",
 /* 104 */ "literal ::= BOOL_LITERAL",
 /* 105 */ "module ::= decls END",
 /* 106 */ "type_array ::= type_builtin ARRAY_BEGIN ARRAY_END",
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
** second argument to YALSTParserAlloc() below.  This can be changed by
** putting an appropriate #define in the %include section of the input
** grammar.
*/
#ifndef YYMALLOCARGTYPE
# define YYMALLOCARGTYPE size_t
#endif

/* Initialize a new parser that has already been allocated.
*/
void YALSTParserInit(void *yypParser){
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

#ifndef YALSTParser_ENGINEALWAYSONSTACK
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
** to YALSTParser and YALSTParserFree.
*/
void *YALSTParserAlloc(void *(*mallocProc)(YYMALLOCARGTYPE)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (YYMALLOCARGTYPE)sizeof(yyParser) );
  if( pParser ) YALSTParserInit(pParser);
  return pParser;
}
#endif /* YALSTParser_ENGINEALWAYSONSTACK */


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
  YALSTParserARG_FETCH;
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
void YALSTParserFinalize(void *p){
  yyParser *pParser = (yyParser*)p;
  while( pParser->yytos>pParser->yystack ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  if( pParser->yystack!=&pParser->yystk0 ) free(pParser->yystack);
#endif
}

#ifndef YALSTParser_ENGINEALWAYSONSTACK
/* 
** Deallocate and destroy a parser.  Destructors are called for
** all stack elements before shutting the parser down.
**
** If the YYPARSEFREENEVERNULL macro exists (for example because it
** is defined in a %include section of the input grammar) then it is
** assumed that the input pointer is never NULL.
*/
void YALSTParserFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
#ifndef YYPARSEFREENEVERNULL
  if( p==0 ) return;
#endif
  YALSTParserFinalize(p);
  (*freeProc)(p);
}
#endif /* YALSTParser_ENGINEALWAYSONSTACK */

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int YALSTParserStackPeak(void *p){
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
   YALSTParserARG_FETCH;
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
   YALSTParserARG_STORE; /* Suppress warning about unused %extra_argument var */
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
  YALSTParserTOKENTYPE yyMinor        /* The minor token to shift in */
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
  { 68, -2 },
  { 68, -2 },
  { 68, -2 },
  { 68, 0 },
  { 74, -1 },
  { 75, -1 },
  { 76, -1 },
  { 76, -1 },
  { 72, -1 },
  { 72, -1 },
  { 72, -1 },
  { 70, -1 },
  { 70, -1 },
  { 70, -1 },
  { 70, -1 },
  { 70, -1 },
  { 70, -1 },
  { 70, -1 },
  { 70, -1 },
  { 70, -1 },
  { 70, -1 },
  { 70, -1 },
  { 73, 0 },
  { 73, -1 },
  { 73, -1 },
  { 73, -2 },
  { 71, -2 },
  { 78, -5 },
  { 78, -1 },
  { 85, -7 },
  { 87, -3 },
  { 87, -1 },
  { 86, -3 },
  { 84, -3 },
  { 79, -7 },
  { 80, -10 },
  { 82, -3 },
  { 82, -1 },
  { 82, 0 },
  { 83, -3 },
  { 83, -2 },
  { 81, -2 },
  { 81, 0 },
  { 91, 0 },
  { 91, -1 },
  { 90, -2 },
  { 90, -1 },
  { 92, -4 },
  { 92, -2 },
  { 92, -2 },
  { 92, -3 },
  { 92, -2 },
  { 77, -5 },
  { 77, -5 },
  { 88, -2 },
  { 88, 0 },
  { 93, -3 },
  { 93, -1 },
  { 93, -1 },
  { 93, -1 },
  { 93, -1 },
  { 93, -7 },
  { 93, -1 },
  { 93, -3 },
  { 93, -4 },
  { 93, -6 },
  { 93, -7 },
  { 95, -3 },
  { 95, -3 },
  { 95, -3 },
  { 95, -3 },
  { 95, -3 },
  { 95, -3 },
  { 95, -3 },
  { 95, -3 },
  { 95, -3 },
  { 95, -3 },
  { 95, -3 },
  { 95, -3 },
  { 95, -3 },
  { 94, -2 },
  { 94, -2 },
  { 94, -2 },
  { 94, -2 },
  { 97, -3 },
  { 97, -1 },
  { 97, 0 },
  { 98, -4 },
  { 99, 0 },
  { 99, -3 },
  { 99, -1 },
  { 100, -3 },
  { 96, -1 },
  { 96, -1 },
  { 96, -1 },
  { 96, -1 },
  { 96, -1 },
  { 96, -1 },
  { 96, -1 },
  { 96, -1 },
  { 96, -1 },
  { 96, -1 },
  { 96, -1 },
  { 96, -1 },
  { 96, -1 },
  { 69, -2 },
  { 101, -3 },
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
  YALSTParserARG_FETCH;
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
      case 0: /* decls ::= decls decl_type */
#line 125 "stparserimpl.lemon"
{
    yylhsminor.yy154 =yymsp[-1].minor.yy154; yymsp[-1].minor.yy154->addDecl(yymsp[0].minor.yy152);
}
#line 1343 "stparserimpl.c"
  yymsp[-1].minor.yy154 = yylhsminor.yy154;
        break;
      case 1: /* decls ::= decls decl_function */
      case 2: /* decls ::= decls decl_type_function */ yytestcase(yyruleno==2);
#line 128 "stparserimpl.lemon"
{
        yylhsminor.yy154 =yymsp[-1].minor.yy154; yymsp[-1].minor.yy154->addDecl(yymsp[0].minor.yy74);
}
#line 1352 "stparserimpl.c"
  yymsp[-1].minor.yy154 = yylhsminor.yy154;
        break;
      case 3: /* decls ::= */
#line 134 "stparserimpl.lemon"
{
        yymsp[1].minor.yy154 =  pParser->getDeclModule();
     }
#line 1360 "stparserimpl.c"
        break;
      case 4: /* identifier ::= IDENTIFIER */
      case 5: /* self ::= SELF */ yytestcase(yyruleno==5);
#line 141 "stparserimpl.lemon"
{
        yylhsminor.yy147 = pParser->createNode<yal::frontend::STIdentifier>(yymsp[0].minor.yy0.tokenStr);
        auto srcInfo = CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy147->setSourceInfo(srcInfo);
}
#line 1370 "stparserimpl.c"
  yymsp[0].minor.yy147 = yylhsminor.yy147;
        break;
      case 6: /* identifier_or_self ::= identifier */
      case 7: /* identifier_or_self ::= self */ yytestcase(yyruleno==7);
#line 153 "stparserimpl.lemon"
{yylhsminor.yy147=yymsp[0].minor.yy147;}
#line 1377 "stparserimpl.c"
  yymsp[0].minor.yy147 = yylhsminor.yy147;
        break;
      case 8: /* type_specifier ::= type_builtin */
#line 156 "stparserimpl.lemon"
{
        yylhsminor.yy198=yymsp[0].minor.yy198;
}
#line 1385 "stparserimpl.c"
  yymsp[0].minor.yy198 = yylhsminor.yy198;
        break;
      case 9: /* type_specifier ::= type_array */
#line 160 "stparserimpl.lemon"
{yymsp[0].minor.yy198 = nullptr;}
#line 1391 "stparserimpl.c"
        break;
      case 10: /* type_specifier ::= IDENTIFIER */
#line 161 "stparserimpl.lemon"
{
    yylhsminor.yy198 = pParser->createNode<yal::frontend::STType>(yymsp[0].minor.yy0.tokenStr);
    yylhsminor.yy198->setSourceInfo(yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0,yymsp[0].minor.yy0, pParser->getSourceHandle()));
}
#line 1399 "stparserimpl.c"
  yymsp[0].minor.yy198 = yylhsminor.yy198;
        break;
      case 11: /* type_builtin ::= TYPE_BOOL */
#line 166 "stparserimpl.lemon"
{
    yylhsminor.yy198 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Bool);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy198->setSourceInfo(srcInfo);
    }
#line 1409 "stparserimpl.c"
  yymsp[0].minor.yy198 = yylhsminor.yy198;
        break;
      case 12: /* type_builtin ::= TYPE_INT8 */
#line 171 "stparserimpl.lemon"
{
        yylhsminor.yy198 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int8);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy198->setSourceInfo(srcInfo);
    }
#line 1419 "stparserimpl.c"
  yymsp[0].minor.yy198 = yylhsminor.yy198;
        break;
      case 13: /* type_builtin ::= TYPE_UINT8 */
#line 176 "stparserimpl.lemon"
{
        yylhsminor.yy198 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt8);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy198->setSourceInfo(srcInfo);
    }
#line 1429 "stparserimpl.c"
  yymsp[0].minor.yy198 = yylhsminor.yy198;
        break;
      case 14: /* type_builtin ::= TYPE_INT16 */
#line 181 "stparserimpl.lemon"
{
        yylhsminor.yy198 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int16);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy198->setSourceInfo(srcInfo);
    }
#line 1439 "stparserimpl.c"
  yymsp[0].minor.yy198 = yylhsminor.yy198;
        break;
      case 15: /* type_builtin ::= TYPE_UINT16 */
#line 186 "stparserimpl.lemon"
{
        yylhsminor.yy198 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt16);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy198->setSourceInfo(srcInfo);
    }
#line 1449 "stparserimpl.c"
  yymsp[0].minor.yy198 = yylhsminor.yy198;
        break;
      case 16: /* type_builtin ::= TYPE_INT32 */
#line 191 "stparserimpl.lemon"
{
        yylhsminor.yy198 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int32);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy198->setSourceInfo(srcInfo);
    }
#line 1459 "stparserimpl.c"
  yymsp[0].minor.yy198 = yylhsminor.yy198;
        break;
      case 17: /* type_builtin ::= TYPE_UINT32 */
#line 196 "stparserimpl.lemon"
{
        yylhsminor.yy198 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt32);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy198->setSourceInfo(srcInfo);
    }
#line 1469 "stparserimpl.c"
  yymsp[0].minor.yy198 = yylhsminor.yy198;
        break;
      case 18: /* type_builtin ::= TYPE_INT64 */
#line 201 "stparserimpl.lemon"
{
        yylhsminor.yy198 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int64);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy198->setSourceInfo(srcInfo);
    }
#line 1479 "stparserimpl.c"
  yymsp[0].minor.yy198 = yylhsminor.yy198;
        break;
      case 19: /* type_builtin ::= TYPE_UINT64 */
#line 206 "stparserimpl.lemon"
{
        yylhsminor.yy198 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt64);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy198->setSourceInfo(srcInfo);
    }
#line 1489 "stparserimpl.c"
  yymsp[0].minor.yy198 = yylhsminor.yy198;
        break;
      case 20: /* type_builtin ::= TYPE_FLOAT */
#line 211 "stparserimpl.lemon"
{
        yylhsminor.yy198 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Float32);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy198->setSourceInfo(srcInfo);
    }
#line 1499 "stparserimpl.c"
  yymsp[0].minor.yy198 = yylhsminor.yy198;
        break;
      case 21: /* type_builtin ::= TYPE_DOUBLE */
#line 216 "stparserimpl.lemon"
{
        yylhsminor.yy198 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Float64);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy198->setSourceInfo(srcInfo);
    }
#line 1509 "stparserimpl.c"
  yymsp[0].minor.yy198 = yylhsminor.yy198;
        break;
      case 22: /* qualifier ::= */
#line 224 "stparserimpl.lemon"
{yymsp[1].minor.yy177 = 0;}
#line 1515 "stparserimpl.c"
        break;
      case 23: /* qualifier ::= MUT */
#line 225 "stparserimpl.lemon"
{yymsp[0].minor.yy177 = yal::frontend::STQualType::Qualifiers::kQualMutable;}
#line 1520 "stparserimpl.c"
        break;
      case 24: /* qualifier ::= REFERENCE */
#line 226 "stparserimpl.lemon"
{yymsp[0].minor.yy177 = yal::frontend::STQualType::Qualifiers::kQualReference;}
#line 1525 "stparserimpl.c"
        break;
      case 25: /* qualifier ::= MUT REFERENCE */
#line 227 "stparserimpl.lemon"
{
        yymsp[-1].minor.yy177 = yal::frontend::STQualType::Qualifiers::kQualReference | yal::frontend::STQualType::Qualifiers::kQualMutable;
 }
#line 1532 "stparserimpl.c"
        break;
      case 26: /* type_qualified ::= qualifier type_specifier */
#line 230 "stparserimpl.lemon"
{
    yylhsminor.yy41 = pParser->createNode<yal::frontend::STQualType>(yymsp[0].minor.yy198, yymsp[-1].minor.yy177);
}
#line 1539 "stparserimpl.c"
  yymsp[-1].minor.yy41 = yylhsminor.yy41;
        break;
      case 27: /* decl_type ::= TYPE identifier COLON type_specifier SEMI_COLON */
#line 236 "stparserimpl.lemon"
{
    yymsp[-4].minor.yy152 = nullptr;
}
#line 1547 "stparserimpl.c"
        break;
      case 28: /* decl_type ::= decl_struct */
#line 240 "stparserimpl.lemon"
{ yylhsminor.yy152 = yymsp[0].minor.yy145;}
#line 1552 "stparserimpl.c"
  yymsp[0].minor.yy152 = yylhsminor.yy152;
        break;
      case 29: /* decl_struct ::= TYPE identifier COLON STRUCT SCOPE_BEGIN struct_decl_vars SCOPE_END */
#line 245 "stparserimpl.lemon"
{
    yylhsminor.yy145 = pParser->createNode<yal::frontend::STDeclStruct>(yymsp[-5].minor.yy147, yymsp[-1].minor.yy68);
    auto srcInfo = CreateSourceInfo(yymsp[-6].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy145->setSourceInfo(srcInfo);
}
#line 1562 "stparserimpl.c"
  yymsp[-6].minor.yy145 = yylhsminor.yy145;
        break;
      case 30: /* struct_decl_vars ::= struct_decl_vars COMMA struct_decl_var */
#line 251 "stparserimpl.lemon"
{
    yylhsminor.yy68 = yymsp[-2].minor.yy68;
    yylhsminor.yy68->push_back(yymsp[0].minor.yy33);
}
#line 1571 "stparserimpl.c"
  yymsp[-2].minor.yy68 = yylhsminor.yy68;
        break;
      case 31: /* struct_decl_vars ::= struct_decl_var */
#line 255 "stparserimpl.lemon"
{
    yylhsminor.yy68 = pParser->createVector<const yal::frontend::STStructMember*>();
    yylhsminor.yy68->push_back(yymsp[0].minor.yy33);
}
#line 1580 "stparserimpl.c"
  yymsp[0].minor.yy68 = yylhsminor.yy68;
        break;
      case 32: /* struct_decl_var ::= identifier COLON type_qualified */
#line 260 "stparserimpl.lemon"
{
    yylhsminor.yy33 = pParser->createNode<yal::frontend::STStructMember>(yymsp[-2].minor.yy147, yymsp[0].minor.yy41);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy147->getSourceInfo(), yymsp[0].minor.yy41->getSourceInfo(),pParser->getSourceHandle());
    yylhsminor.yy33->setSourceInfo(srcInfo);
}
#line 1590 "stparserimpl.c"
  yymsp[-2].minor.yy33 = yylhsminor.yy33;
        break;
      case 33: /* function_param_list ::= PAR_BEGIN function_args_decl PAR_END */
#line 269 "stparserimpl.lemon"
{
    yymsp[-2].minor.yy166 = yymsp[-1].minor.yy166;
}
#line 1598 "stparserimpl.c"
        break;
      case 34: /* decl_function ::= FUNCTION identifier function_param_list function_return_decl SCOPE_BEGIN statement_list_or_empty SCOPE_END */
#line 274 "stparserimpl.lemon"
{
    yylhsminor.yy74 = pParser->createNode<yal::frontend::STDeclFunction>(yymsp[-5].minor.yy147, nullptr, yymsp[-3].minor.yy41, yymsp[-4].minor.yy166, yymsp[-1].minor.yy42);
    yylhsminor.yy74->setSourceInfo(CreateSourceInfo(yymsp[-6].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle()));
}
#line 1606 "stparserimpl.c"
  yymsp[-6].minor.yy74 = yylhsminor.yy74;
        break;
      case 35: /* decl_type_function ::= FUNCTION type_specifier COLON COLON identifier function_param_list function_return_decl SCOPE_BEGIN statement_list_or_empty SCOPE_END */
#line 282 "stparserimpl.lemon"
{
    yylhsminor.yy74 = pParser->createNode<yal::frontend::STDeclFunction>(yymsp[-5].minor.yy147, yymsp[-8].minor.yy198, yymsp[-3].minor.yy41, yymsp[-4].minor.yy166, yymsp[-1].minor.yy42);
    yylhsminor.yy74->setSourceInfo(CreateSourceInfo(yymsp[-9].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle()));
}
#line 1615 "stparserimpl.c"
  yymsp[-9].minor.yy74 = yylhsminor.yy74;
        break;
      case 36: /* function_args_decl ::= function_args_decl COMMA function_arg_decl */
#line 289 "stparserimpl.lemon"
{
        yymsp[-2].minor.yy166->push_back(yymsp[0].minor.yy173);
        yylhsminor.yy166 = yymsp[-2].minor.yy166;
}
#line 1624 "stparserimpl.c"
  yymsp[-2].minor.yy166 = yylhsminor.yy166;
        break;
      case 37: /* function_args_decl ::= function_arg_decl */
#line 294 "stparserimpl.lemon"
{
    yylhsminor.yy166 = pParser->createVector<const yal::frontend::STDeclParam*>();
    yylhsminor.yy166->push_back(yymsp[0].minor.yy173);
}
#line 1633 "stparserimpl.c"
  yymsp[0].minor.yy166 = yylhsminor.yy166;
        break;
      case 38: /* function_args_decl ::= */
#line 299 "stparserimpl.lemon"
{
        yymsp[1].minor.yy166 = nullptr;
}
#line 1641 "stparserimpl.c"
        break;
      case 39: /* function_arg_decl ::= identifier COLON type_qualified */
#line 303 "stparserimpl.lemon"
{
        yylhsminor.yy173 = pParser->createNode<yal::frontend::STDeclParam>(yymsp[-2].minor.yy147, yymsp[0].minor.yy41);
        yylhsminor.yy173->setSourceInfo(yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy147->getSourceInfo(),yymsp[0].minor.yy41->getSourceInfo(), pParser->getSourceHandle()));
}
#line 1649 "stparserimpl.c"
  yymsp[-2].minor.yy173 = yylhsminor.yy173;
        break;
      case 40: /* function_arg_decl ::= qualifier self */
#line 308 "stparserimpl.lemon"
{
        auto qualType = pParser->createNode<yal::frontend::STQualType>(nullptr, yymsp[-1].minor.yy177);
        yylhsminor.yy173 = pParser->createNode<yal::frontend::STDeclParam>(yymsp[0].minor.yy147, qualType);
        yylhsminor.yy173->setSourceInfo(yymsp[0].minor.yy147->getSourceInfo());
}
#line 1659 "stparserimpl.c"
  yymsp[-1].minor.yy173 = yylhsminor.yy173;
        break;
      case 41: /* function_return_decl ::= COLON type_qualified */
#line 314 "stparserimpl.lemon"
{ yymsp[-1].minor.yy41 = yymsp[0].minor.yy41;}
#line 1665 "stparserimpl.c"
        break;
      case 42: /* function_return_decl ::= */
      case 55: /* var_type_spec ::= */ yytestcase(yyruleno==55);
#line 315 "stparserimpl.lemon"
{yymsp[1].minor.yy41 = nullptr;}
#line 1671 "stparserimpl.c"
        break;
      case 43: /* statement_list_or_empty ::= */
#line 319 "stparserimpl.lemon"
{yymsp[1].minor.yy42 = nullptr;}
#line 1676 "stparserimpl.c"
        break;
      case 44: /* statement_list_or_empty ::= statement_list */
#line 320 "stparserimpl.lemon"
{yylhsminor.yy42 = yymsp[0].minor.yy42;}
#line 1681 "stparserimpl.c"
  yymsp[0].minor.yy42 = yylhsminor.yy42;
        break;
      case 45: /* statement_list ::= statement_list statement */
#line 323 "stparserimpl.lemon"
{
    yymsp[-1].minor.yy42->push_back(yymsp[0].minor.yy69);
    yylhsminor.yy42=yymsp[-1].minor.yy42;
}
#line 1690 "stparserimpl.c"
  yymsp[-1].minor.yy42 = yylhsminor.yy42;
        break;
      case 46: /* statement_list ::= statement */
#line 327 "stparserimpl.lemon"
{
    yylhsminor.yy42 = pParser->createVector<const yal::frontend::STStatement*>();
    yylhsminor.yy42->push_back(yymsp[0].minor.yy69);
}
#line 1699 "stparserimpl.c"
  yymsp[0].minor.yy42 = yylhsminor.yy42;
        break;
      case 47: /* statement ::= expression ASSIGN expression SEMI_COLON */
#line 333 "stparserimpl.lemon"
{
   yylhsminor.yy69 = pParser->createNode<yal::frontend::STStmtAssign>(yymsp[-3].minor.yy44,yymsp[-1].minor.yy44);
   yylhsminor.yy69->setSourceInfo(yal::frontend::CreateSourceInfo(yymsp[-3].minor.yy44->getSourceInfo(), yymsp[-1].minor.yy44->getSourceInfo(), pParser->getSourceHandle()));
}
#line 1708 "stparserimpl.c"
  yymsp[-3].minor.yy69 = yylhsminor.yy69;
        break;
      case 48: /* statement ::= decl_var SEMI_COLON */
#line 337 "stparserimpl.lemon"
{
    yylhsminor.yy69 = pParser->createNode<yal::frontend::STStmtDecl>(yymsp[-1].minor.yy5);
    yylhsminor.yy69->setSourceInfo(yymsp[-1].minor.yy5->getSourceInfo());
}
#line 1717 "stparserimpl.c"
  yymsp[-1].minor.yy69 = yylhsminor.yy69;
        break;
      case 49: /* statement ::= expression SEMI_COLON */
#line 342 "stparserimpl.lemon"
{
    yylhsminor.yy69 = pParser->createNode<yal::frontend::STStmtExpression>(yymsp[-1].minor.yy44);
    yylhsminor.yy69->setSourceInfo(yymsp[-1].minor.yy44->getSourceInfo());
}
#line 1726 "stparserimpl.c"
  yymsp[-1].minor.yy69 = yylhsminor.yy69;
        break;
      case 50: /* statement ::= RETURN expression SEMI_COLON */
#line 347 "stparserimpl.lemon"
{
    yylhsminor.yy69 = pParser->createNode<yal::frontend::STStmtReturn>(yymsp[-1].minor.yy44);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy0, yymsp[-1].minor.yy44->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy69->setSourceInfo(srcInfo);
}
#line 1736 "stparserimpl.c"
  yymsp[-2].minor.yy69 = yylhsminor.yy69;
        break;
      case 51: /* statement ::= RETURN SEMI_COLON */
#line 353 "stparserimpl.lemon"
{
    yylhsminor.yy69 = pParser->createNode<yal::frontend::STStmtReturn>(nullptr);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[-1].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy69->setSourceInfo(srcInfo);
}
#line 1746 "stparserimpl.c"
  yymsp[-1].minor.yy69 = yylhsminor.yy69;
        break;
      case 52: /* decl_var ::= VAR identifier var_type_spec ASSIGN expression */
      case 53: /* decl_var ::= LET identifier var_type_spec ASSIGN expression */ yytestcase(yyruleno==53);
#line 361 "stparserimpl.lemon"
{
    yylhsminor.yy5 = pParser->createNode<yal::frontend::STDeclVar>(yymsp[-3].minor.yy147, yymsp[-2].minor.yy41, yymsp[0].minor.yy44);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy44->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy5->setSourceInfo(srcInfo);
}
#line 1757 "stparserimpl.c"
  yymsp[-4].minor.yy5 = yylhsminor.yy5;
        break;
      case 54: /* var_type_spec ::= COLON type_qualified */
#line 373 "stparserimpl.lemon"
{yymsp[-1].minor.yy41 = yymsp[0].minor.yy41;}
#line 1763 "stparserimpl.c"
        break;
      case 56: /* expression ::= PAR_BEGIN expression PAR_END */
#line 379 "stparserimpl.lemon"
{yymsp[-2].minor.yy44 = yymsp[-1].minor.yy44;}
#line 1768 "stparserimpl.c"
        break;
      case 57: /* expression ::= literal */
#line 380 "stparserimpl.lemon"
{yylhsminor.yy44 = yymsp[0].minor.yy44;}
#line 1773 "stparserimpl.c"
  yymsp[0].minor.yy44 = yylhsminor.yy44;
        break;
      case 58: /* expression ::= unaryexp */
#line 381 "stparserimpl.lemon"
{yylhsminor.yy44 = yymsp[0].minor.yy118;}
#line 1779 "stparserimpl.c"
  yymsp[0].minor.yy44 = yylhsminor.yy44;
        break;
      case 59: /* expression ::= binaryexp */
#line 382 "stparserimpl.lemon"
{yylhsminor.yy44 = yymsp[0].minor.yy200;}
#line 1785 "stparserimpl.c"
  yymsp[0].minor.yy44 = yylhsminor.yy44;
        break;
      case 60: /* expression ::= struct_init */
#line 383 "stparserimpl.lemon"
{ yylhsminor.yy44 = yymsp[0].minor.yy24;}
#line 1791 "stparserimpl.c"
  yymsp[0].minor.yy44 = yylhsminor.yy44;
        break;
      case 61: /* expression ::= RANGE_CAST LT type_qualified GT PAR_BEGIN expression PAR_END */
#line 385 "stparserimpl.lemon"
{
    yylhsminor.yy44 = pParser->createNode<yal::frontend::STExprRangeCast>(yymsp[-4].minor.yy41, yymsp[-1].minor.yy44);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-6].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy44->setSourceInfo(srcInfo);
}
#line 1801 "stparserimpl.c"
  yymsp[-6].minor.yy44 = yylhsminor.yy44;
        break;
      case 62: /* expression ::= identifier_or_self */
#line 391 "stparserimpl.lemon"
{
    yylhsminor.yy44 = pParser->createNode<yal::frontend::STExprVarRef>(yymsp[0].minor.yy147);
    yylhsminor.yy44->setSourceInfo(yymsp[0].minor.yy147->getSourceInfo());
}
#line 1810 "stparserimpl.c"
  yymsp[0].minor.yy44 = yylhsminor.yy44;
        break;
      case 63: /* expression ::= expression DOT identifier */
#line 396 "stparserimpl.lemon"
{
    yylhsminor.yy44 = pParser->createNode<yal::frontend::STExprStructVarRef>(yymsp[-2].minor.yy44, yymsp[0].minor.yy147);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy44->getSourceInfo(), yymsp[0].minor.yy147->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy44->setSourceInfo(srcInfo);
}
#line 1820 "stparserimpl.c"
  yymsp[-2].minor.yy44 = yylhsminor.yy44;
        break;
      case 64: /* expression ::= identifier PAR_BEGIN function_call_args PAR_END */
#line 402 "stparserimpl.lemon"
{
    yylhsminor.yy44 = pParser->createNode<yal::frontend::STExprFnCall>(yymsp[-3].minor.yy147,yymsp[-1].minor.yy192);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-3].minor.yy147->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy44->setSourceInfo(srcInfo);
}
#line 1830 "stparserimpl.c"
  yymsp[-3].minor.yy44 = yylhsminor.yy44;
        break;
      case 65: /* expression ::= expression DOT identifier PAR_BEGIN function_call_args PAR_END */
#line 408 "stparserimpl.lemon"
{
    yylhsminor.yy44 = pParser->createNode<yal::frontend::STExprFnCall>(yymsp[-3].minor.yy147,yymsp[-5].minor.yy44,yymsp[-1].minor.yy192);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-5].minor.yy44->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy44->setSourceInfo(srcInfo);
}
#line 1840 "stparserimpl.c"
  yymsp[-5].minor.yy44 = yylhsminor.yy44;
        break;
      case 66: /* expression ::= type_specifier COLON COLON identifier PAR_BEGIN function_call_args PAR_END */
#line 414 "stparserimpl.lemon"
{
    yylhsminor.yy44 = pParser->createNode<yal::frontend::STExprFnCall>(yymsp[-3].minor.yy147,yymsp[-6].minor.yy198,yymsp[-1].minor.yy192);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-6].minor.yy198->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy44->setSourceInfo(srcInfo);
}
#line 1850 "stparserimpl.c"
  yymsp[-6].minor.yy44 = yylhsminor.yy44;
        break;
      case 67: /* binaryexp ::= expression AND expression */
#line 422 "stparserimpl.lemon"
{
        yylhsminor.yy200 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::And,
                                                        yymsp[-2].minor.yy44, yymsp[0].minor.yy44);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy44->getSourceInfo(), yymsp[0].minor.yy44->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy200->setSourceInfo(srcInfo);
}
#line 1861 "stparserimpl.c"
  yymsp[-2].minor.yy200 = yylhsminor.yy200;
        break;
      case 68: /* binaryexp ::= expression OR expression */
#line 428 "stparserimpl.lemon"
{
        yylhsminor.yy200 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Or,
                                                        yymsp[-2].minor.yy44, yymsp[0].minor.yy44);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy44->getSourceInfo(), yymsp[0].minor.yy44->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy200->setSourceInfo(srcInfo);
}
#line 1872 "stparserimpl.c"
  yymsp[-2].minor.yy200 = yylhsminor.yy200;
        break;
      case 69: /* binaryexp ::= expression PLUS expression */
#line 434 "stparserimpl.lemon"
{
        yylhsminor.yy200 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Plus,
                                                        yymsp[-2].minor.yy44, yymsp[0].minor.yy44);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy44->getSourceInfo(), yymsp[0].minor.yy44->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy200->setSourceInfo(srcInfo);
}
#line 1883 "stparserimpl.c"
  yymsp[-2].minor.yy200 = yylhsminor.yy200;
        break;
      case 70: /* binaryexp ::= expression MINUS expression */
#line 440 "stparserimpl.lemon"
{
        yylhsminor.yy200 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Minus,
                                                        yymsp[-2].minor.yy44, yymsp[0].minor.yy44);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy44->getSourceInfo(), yymsp[0].minor.yy44->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy200->setSourceInfo(srcInfo);
}
#line 1894 "stparserimpl.c"
  yymsp[-2].minor.yy200 = yylhsminor.yy200;
        break;
      case 71: /* binaryexp ::= expression MULT expression */
#line 446 "stparserimpl.lemon"
{
        yylhsminor.yy200 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Mult,
                                                        yymsp[-2].minor.yy44, yymsp[0].minor.yy44);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy44->getSourceInfo(), yymsp[0].minor.yy44->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy200->setSourceInfo(srcInfo);
}
#line 1905 "stparserimpl.c"
  yymsp[-2].minor.yy200 = yylhsminor.yy200;
        break;
      case 72: /* binaryexp ::= expression DIV expression */
#line 452 "stparserimpl.lemon"
{
        yylhsminor.yy200 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Div,
                                                        yymsp[-2].minor.yy44, yymsp[0].minor.yy44);
}
#line 1914 "stparserimpl.c"
  yymsp[-2].minor.yy200 = yylhsminor.yy200;
        break;
      case 73: /* binaryexp ::= expression MOD expression */
#line 456 "stparserimpl.lemon"
{
        yylhsminor.yy200 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Mod,
                                                        yymsp[-2].minor.yy44, yymsp[0].minor.yy44);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy44->getSourceInfo(), yymsp[0].minor.yy44->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy200->setSourceInfo(srcInfo);
}
#line 1925 "stparserimpl.c"
  yymsp[-2].minor.yy200 = yylhsminor.yy200;
        break;
      case 74: /* binaryexp ::= expression EQ expression */
#line 463 "stparserimpl.lemon"
{
        yylhsminor.yy200 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Eq,
                                                        yymsp[-2].minor.yy44, yymsp[0].minor.yy44);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy44->getSourceInfo(), yymsp[0].minor.yy44->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy200->setSourceInfo(srcInfo);
}
#line 1936 "stparserimpl.c"
  yymsp[-2].minor.yy200 = yylhsminor.yy200;
        break;
      case 75: /* binaryexp ::= expression NE expression */
#line 470 "stparserimpl.lemon"
{
        yylhsminor.yy200 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Ne,
                                                        yymsp[-2].minor.yy44, yymsp[0].minor.yy44);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy44->getSourceInfo(), yymsp[0].minor.yy44->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy200->setSourceInfo(srcInfo);
}
#line 1947 "stparserimpl.c"
  yymsp[-2].minor.yy200 = yylhsminor.yy200;
        break;
      case 76: /* binaryexp ::= expression LE expression */
#line 477 "stparserimpl.lemon"
{
        yylhsminor.yy200 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Le,
                                                        yymsp[-2].minor.yy44, yymsp[0].minor.yy44);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy44->getSourceInfo(), yymsp[0].minor.yy44->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy200->setSourceInfo(srcInfo);
}
#line 1958 "stparserimpl.c"
  yymsp[-2].minor.yy200 = yylhsminor.yy200;
        break;
      case 77: /* binaryexp ::= expression LT expression */
#line 484 "stparserimpl.lemon"
{
        yylhsminor.yy200 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Lt,
                                                        yymsp[-2].minor.yy44, yymsp[0].minor.yy44);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy44->getSourceInfo(), yymsp[0].minor.yy44->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy200->setSourceInfo(srcInfo);
}
#line 1969 "stparserimpl.c"
  yymsp[-2].minor.yy200 = yylhsminor.yy200;
        break;
      case 78: /* binaryexp ::= expression GE expression */
#line 491 "stparserimpl.lemon"
{
        yylhsminor.yy200 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Ge,
                                                        yymsp[-2].minor.yy44, yymsp[0].minor.yy44);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy44->getSourceInfo(), yymsp[0].minor.yy44->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy200->setSourceInfo(srcInfo);
}
#line 1980 "stparserimpl.c"
  yymsp[-2].minor.yy200 = yylhsminor.yy200;
        break;
      case 79: /* binaryexp ::= expression GT expression */
#line 498 "stparserimpl.lemon"
{
        yylhsminor.yy200 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Gt,
                                                        yymsp[-2].minor.yy44, yymsp[0].minor.yy44);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy44->getSourceInfo(), yymsp[0].minor.yy44->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy200->setSourceInfo(srcInfo);
}
#line 1991 "stparserimpl.c"
  yymsp[-2].minor.yy200 = yylhsminor.yy200;
        break;
      case 80: /* unaryexp ::= NOT expression */
#line 507 "stparserimpl.lemon"
{
    yylhsminor.yy118 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Not,
                                               yymsp[0].minor.yy44);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy44->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy118->setSourceInfo(srcInfo);
}
#line 2002 "stparserimpl.c"
  yymsp[-1].minor.yy118 = yylhsminor.yy118;
        break;
      case 81: /* unaryexp ::= BIT_NOT expression */
#line 513 "stparserimpl.lemon"
{
    yylhsminor.yy118 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::BitNot,
                                               yymsp[0].minor.yy44);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy44->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy118->setSourceInfo(srcInfo);
}
#line 2013 "stparserimpl.c"
  yymsp[-1].minor.yy118 = yylhsminor.yy118;
        break;
      case 82: /* unaryexp ::= MINUS expression */
#line 519 "stparserimpl.lemon"
{
    yylhsminor.yy118 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Negate,
                                               yymsp[0].minor.yy44);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy44->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy118->setSourceInfo(srcInfo);
}
#line 2024 "stparserimpl.c"
  yymsp[-1].minor.yy118 = yylhsminor.yy118;
        break;
      case 83: /* unaryexp ::= REFERENCE expression */
#line 526 "stparserimpl.lemon"
{
    yylhsminor.yy118 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Reference,
                                               yymsp[0].minor.yy44);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy44->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy118->setSourceInfo(srcInfo);
}
#line 2035 "stparserimpl.c"
  yymsp[-1].minor.yy118 = yylhsminor.yy118;
        break;
      case 84: /* function_call_args ::= function_call_args COMMA expression */
#line 535 "stparserimpl.lemon"
{
    yylhsminor.yy192 = yymsp[-2].minor.yy192;
    yylhsminor.yy192->push_back(yymsp[0].minor.yy44);
}
#line 2044 "stparserimpl.c"
  yymsp[-2].minor.yy192 = yylhsminor.yy192;
        break;
      case 85: /* function_call_args ::= expression */
#line 539 "stparserimpl.lemon"
{
    yylhsminor.yy192 = pParser->createVector<const yal::frontend::STExpression*>();
    yylhsminor.yy192->push_back(yymsp[0].minor.yy44);
}
#line 2053 "stparserimpl.c"
  yymsp[0].minor.yy192 = yylhsminor.yy192;
        break;
      case 86: /* function_call_args ::= */
#line 543 "stparserimpl.lemon"
{
    yymsp[1].minor.yy192= nullptr;
}
#line 2061 "stparserimpl.c"
        break;
      case 87: /* struct_init ::= type_specifier SCOPE_BEGIN struct_member_init_list SCOPE_END */
#line 550 "stparserimpl.lemon"
{
    yylhsminor.yy24 = pParser->createNode<yal::frontend::STExprStructInit>(yymsp[-3].minor.yy198, yymsp[-1].minor.yy96);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-3].minor.yy198->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy24->setSourceInfo(srcInfo);
}
#line 2070 "stparserimpl.c"
  yymsp[-3].minor.yy24 = yylhsminor.yy24;
        break;
      case 88: /* struct_member_init_list ::= */
#line 556 "stparserimpl.lemon"
{yymsp[1].minor.yy96 = nullptr;}
#line 2076 "stparserimpl.c"
        break;
      case 89: /* struct_member_init_list ::= struct_member_init_list COMMA struct_member_init */
#line 557 "stparserimpl.lemon"
{
     yymsp[-2].minor.yy96->push_back(yymsp[0].minor.yy109);
     yylhsminor.yy96 = yymsp[-2].minor.yy96;
}
#line 2084 "stparserimpl.c"
  yymsp[-2].minor.yy96 = yylhsminor.yy96;
        break;
      case 90: /* struct_member_init_list ::= struct_member_init */
#line 561 "stparserimpl.lemon"
{
        yylhsminor.yy96 = pParser->createVector<const yal::frontend::STStructMemberInit*>();
        yylhsminor.yy96->push_back(yymsp[0].minor.yy109);
}
#line 2093 "stparserimpl.c"
  yymsp[0].minor.yy96 = yylhsminor.yy96;
        break;
      case 91: /* struct_member_init ::= identifier COLON expression */
#line 566 "stparserimpl.lemon"
{
    yylhsminor.yy109 = pParser->createNode<yal::frontend::STStructMemberInit>(yymsp[-2].minor.yy147,yymsp[0].minor.yy44);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy147->getSourceInfo(), yymsp[0].minor.yy44->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy109->setSourceInfo(srcInfo);
}
#line 2103 "stparserimpl.c"
  yymsp[-2].minor.yy109 = yylhsminor.yy109;
        break;
      case 92: /* literal ::= INTEGER_LITERAL */
      case 93: /* literal ::= INTEGER_LITERAL_I8 */ yytestcase(yyruleno==93);
      case 94: /* literal ::= INTEGER_LITERAL_I16 */ yytestcase(yyruleno==94);
      case 95: /* literal ::= INTEGER_LITERAL_I32 */ yytestcase(yyruleno==95);
      case 96: /* literal ::= INTEGER_LITERAL_I64 */ yytestcase(yyruleno==96);
      case 97: /* literal ::= INTEGER_LITERAL_U8 */ yytestcase(yyruleno==97);
      case 98: /* literal ::= INTEGER_LITERAL_U16 */ yytestcase(yyruleno==98);
      case 99: /* literal ::= INTEGER_LITERAL_U32 */ yytestcase(yyruleno==99);
      case 100: /* literal ::= INTEGER_LITERAL_U64 */ yytestcase(yyruleno==100);
#line 574 "stparserimpl.lemon"
{
        yylhsminor.yy44 = pParser->createNode<yal::frontend::STExprIntegerLiteral>(yymsp[0].minor.yy0);
        auto srcInfo =yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy44->setSourceInfo(srcInfo);
   }
#line 2121 "stparserimpl.c"
  yymsp[0].minor.yy44 = yylhsminor.yy44;
        break;
      case 101: /* literal ::= FLOAT_LITERAL */
      case 102: /* literal ::= FLOAT_LITERAL_32 */ yytestcase(yyruleno==102);
      case 103: /* literal ::= FLOAT_LITERAL_64 */ yytestcase(yyruleno==103);
#line 619 "stparserimpl.lemon"
{
        yylhsminor.yy44 = pParser->createNode<yal::frontend::STExprFloatLiteral>(yymsp[0].minor.yy0);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy44->setSourceInfo(srcInfo);
}
#line 2133 "stparserimpl.c"
  yymsp[0].minor.yy44 = yylhsminor.yy44;
        break;
      case 104: /* literal ::= BOOL_LITERAL */
#line 636 "stparserimpl.lemon"
{
        yylhsminor.yy44 = pParser->createNode<yal::frontend::STExprBoolLiteral>(yymsp[0].minor.yy0.tokenStr);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy44->setSourceInfo(srcInfo);
}
#line 2143 "stparserimpl.c"
  yymsp[0].minor.yy44 = yylhsminor.yy44;
        break;
      default:
      /* (105) module ::= decls END */ yytestcase(yyruleno==105);
      /* (106) type_array ::= type_builtin ARRAY_BEGIN ARRAY_END */ yytestcase(yyruleno==106);
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
  YALSTParserARG_FETCH;
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
  YALSTParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  YALSTParserTOKENTYPE yyminor         /* The minor type of the error token */
){
  YALSTParserARG_FETCH;
#define TOKEN yyminor
/************ Begin %syntax_error code ****************************************/
#line 40 "stparserimpl.lemon"


    yal::frontend::STParser::OnParseError(*pParser);
    {
#if defined(YAL_DEBUG)
        /*
        yal::frontend::Log::MultiLineScope multiLineScope(pParser->getLog());
        pParser->getLog().error("Expected one of the following tokens:\n");
        int n = sizeof(yyTokenName) / sizeof(yyTokenName[0]);
        for (int i = 0; i < n; ++i) {
            int a = yy_find_shift_action(yypParser, (YYCODETYPE)i);
            if (a < YYNSTATE + YYNRULE) {
                pParser->getLog().error("%\n",yyTokenName[i]);
            }
        }*/
#endif
    }
#line 2227 "stparserimpl.c"
/************ End %syntax_error code ******************************************/
  YALSTParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  YALSTParserARG_FETCH;
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
  YALSTParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "YALSTParserAlloc" which describes the current state of the parser.
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
void YALSTParser(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  YALSTParserTOKENTYPE yyminor       /* The value for the token */
  YALSTParserARG_PDECL               /* Optional %extra_argument parameter */
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
  YALSTParserARG_STORE;

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
