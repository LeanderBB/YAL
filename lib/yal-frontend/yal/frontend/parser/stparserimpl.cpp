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
#define YAL_TOKEN_AS                              2
#define YAL_TOKEN_OR                              3
#define YAL_TOKEN_AND                             4
#define YAL_TOKEN_BIT_OR                          5
#define YAL_TOKEN_BIT_XOR                         6
#define YAL_TOKEN_BIT_AND                         7
#define YAL_TOKEN_EQ                              8
#define YAL_TOKEN_NE                              9
#define YAL_TOKEN_GT                             10
#define YAL_TOKEN_LT                             11
#define YAL_TOKEN_GE                             12
#define YAL_TOKEN_LE                             13
#define YAL_TOKEN_PLUS                           14
#define YAL_TOKEN_MINUS                          15
#define YAL_TOKEN_MULT                           16
#define YAL_TOKEN_DIV                            17
#define YAL_TOKEN_MOD                            18
#define YAL_TOKEN_NEGATE                         19
#define YAL_TOKEN_BIT_NOT                        20
#define YAL_TOKEN_NOT                            21
#define YAL_TOKEN_DOT                            22
#define YAL_TOKEN_END                            23
#define YAL_TOKEN_IDENTIFIER                     24
#define YAL_TOKEN_SELF                           25
#define YAL_TOKEN_TYPE_BOOL                      26
#define YAL_TOKEN_TYPE_INT8                      27
#define YAL_TOKEN_TYPE_UINT8                     28
#define YAL_TOKEN_TYPE_INT16                     29
#define YAL_TOKEN_TYPE_UINT16                    30
#define YAL_TOKEN_TYPE_INT32                     31
#define YAL_TOKEN_TYPE_UINT32                    32
#define YAL_TOKEN_TYPE_INT64                     33
#define YAL_TOKEN_TYPE_UINT64                    34
#define YAL_TOKEN_TYPE_FLOAT                     35
#define YAL_TOKEN_TYPE_DOUBLE                    36
#define YAL_TOKEN_ARRAY_BEGIN                    37
#define YAL_TOKEN_ARRAY_END                      38
#define YAL_TOKEN_MUT                            39
#define YAL_TOKEN_REFERENCE                      40
#define YAL_TOKEN_TYPE                           41
#define YAL_TOKEN_COLON                          42
#define YAL_TOKEN_SEMI_COLON                     43
#define YAL_TOKEN_STRUCT                         44
#define YAL_TOKEN_SCOPE_BEGIN                    45
#define YAL_TOKEN_SCOPE_END                      46
#define YAL_TOKEN_COMMA                          47
#define YAL_TOKEN_PAR_BEGIN                      48
#define YAL_TOKEN_PAR_END                        49
#define YAL_TOKEN_FUNCTION                       50
#define YAL_TOKEN_RETURN                         51
#define YAL_TOKEN_VAR                            52
#define YAL_TOKEN_LET                            53
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
#define YYNOCODE 102
#define YYACTIONTYPE unsigned short int
#if INTERFACE
#define YALSTParserTOKENTYPE yal::frontend::TokenInfo
#endif
typedef union {
  int yyinit;
  YALSTParserTOKENTYPE yy0;
  yal::frontend::STExprUnaryOperator* yy2;
  yal::frontend::STDeclParam* yy3;
  yal::frontend::STExprBinaryOperator* yy4;
  yal::frontend::STStructMember* yy25;
  yal::frontend::ParseListStructInit::Range yy32;
  yal::frontend::STStructMemberInit* yy33;
  yal::frontend::ParseListStructMember::Range yy34;
  yal::frontend::ParseListStmt::Range yy47;
  yal::frontend::STIdentifier* yy57;
  yal::frontend::STDeclModule* yy66;
  yal::frontend::STDeclFunction* yy68;
  yal::frontend::ParseListDeclParam::Range yy69;
  yal::frontend::STQualType* yy71;
  uint32_t yy72;
  yal::frontend::STType* yy74;
  yal::frontend::STDeclStruct* yy77;
  yal::frontend::STExprStructInit* yy100;
  yal::frontend::STDecl* yy108;
  yal::frontend::STStatement* yy129;
  yal::frontend::STExpression* yy162;
  yal::frontend::STDeclVar* yy167;
  yal::frontend::ParseListExpr::Range yy200;
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
#define YYNSTATE             118
#define YYNRULE              108
#define YY_MAX_SHIFT         117
#define YY_MIN_SHIFTREDUCE   190
#define YY_MAX_SHIFTREDUCE   297
#define YY_MIN_REDUCE        298
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
#define YY_ACTTAB_COUNT (1002)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    14,  115,  115,  113,  324,   15,   16,  194,  320,   85,
 /*    10 */   195,  201,  202,  203,  204,  205,  206,  207,  208,  209,
 /*    20 */   210,  211,  110,  214,  115,   13,   90,  339,   58,   36,
 /*    30 */     2,  307,  307,   31,  194,   96,    8,   53,   51,  283,
 /*    40 */   284,  285,  286,  287,  288,  289,  290,  291,  292,  293,
 /*    50 */   294,  295,   14,   40,  307,   34,  407,   15,   16,   81,
 /*    60 */   389,   85,  195,  201,  202,  203,  204,  205,  206,  207,
 /*    70 */   208,  209,  210,  211,   57,  109,   88,   13,   57,  109,
 /*    80 */   241,  110,  214,   84,  335,   31,   33,  353,  334,   36,
 /*    90 */    47,  283,  284,  285,  286,  287,  288,  289,  290,  291,
 /*   100 */   292,  293,  294,  295,  298,  299,  300,   14,   96,   48,
 /*   110 */    91,  326,   15,   16,  278,   45,   85,  195,  201,  202,
 /*   120 */   203,  204,  205,  206,  207,  208,  209,  210,  211,   26,
 /*   130 */    25,   24,   13,  388,  360,   55,   36,  337,   41,   36,
 /*   140 */    31,  330,   17,   36,  255,   50,  283,  284,  285,  286,
 /*   150 */   287,  288,  289,  290,  291,  292,  293,  294,  295,   11,
 /*   160 */    42,   29,   30,   17,   92,  257,   23,   22,   18,   20,
 /*   170 */    19,   21,   28,   27,   26,   25,   24,   42,   29,   30,
 /*   180 */    55,   52,  111,   23,   22,   18,   20,   19,   21,   28,
 /*   190 */    27,   26,   25,   24,  329,   86,  102,   55,  194,   38,
 /*   200 */   200,  239,  201,  202,  203,  204,  205,  206,  207,  208,
 /*   210 */   209,  210,  211,  110,  214,   17,  101,  256,  240,  106,
 /*   220 */   112,   42,   29,   30,   37,  108,  223,   23,   22,   18,
 /*   230 */    20,   19,   21,   28,   27,   26,   25,   24,   42,   29,
 /*   240 */    30,   55,  195,  111,   23,   22,   18,   20,   19,   21,
 /*   250 */    28,   27,   26,   25,   24,  328,  338,  308,   55,  115,
 /*   260 */   308,   83,  237,  107,  305,  361,   94,  219,   46,   56,
 /*   270 */   116,  225,    1,  117,   49,   89,    9,   10,    4,   87,
 /*   280 */   344,   60,  357,  358,  356,  247,  359,  242,  238,  307,
 /*   290 */   224,   59,  115,   55,   83,   12,  107,  305,  361,   94,
 /*   300 */    32,   54,    5,    6,    7,    3,   43,  215,   44,  114,
 /*   310 */    39,    4,   93,  344,   60,  357,  358,  356,  297,  359,
 /*   320 */   217,  115,  307,   83,   35,  107,  305,  361,   94,  404,
 /*   330 */   300,  300,  300,  300,  300,  300,  300,  300,  300,  300,
 /*   340 */     4,   95,  344,   60,  357,  358,  356,  300,  359,  300,
 /*   350 */   300,  307,  300,  300,   42,   29,   30,  300,  300,  300,
 /*   360 */    23,   22,   18,   20,   19,   21,   28,   27,   26,   25,
 /*   370 */    24,  300,  300,  300,   55,  300,  300,  300,  300,  300,
 /*   380 */   300,  300,  115,  300,   83,  300,  107,  305,  361,   94,
 /*   390 */    85,  300,  201,  202,  203,  204,  205,  206,  207,  208,
 /*   400 */   209,  210,  211,  343,   60,  357,  358,  356,  300,  359,
 /*   410 */    30,  300,  307,  300,   23,   22,   18,   20,   19,   21,
 /*   420 */    28,   27,   26,   25,   24,  115,  300,   83,   55,  107,
 /*   430 */   305,  361,  300,  300,  115,  300,   83,  300,  107,  305,
 /*   440 */   361,  300,  300,  300,  300,  300,  300,   68,  357,  358,
 /*   450 */   356,   82,  359,  300,  300,  307,   68,  357,  358,  356,
 /*   460 */    80,  359,  300,  300,  307,  300,  300,  300,  300,  300,
 /*   470 */   300,  300,  300,  300,  115,  300,   83,  300,  107,  305,
 /*   480 */   361,  200,  300,  201,  202,  203,  204,  205,  206,  207,
 /*   490 */   208,  209,  210,  211,  300,  300,   68,  357,  358,  356,
 /*   500 */    79,  359,  300,  300,  307,  406,  406,  406,  406,  406,
 /*   510 */   406,   28,   27,   26,   25,   24,  300,  300,  300,   55,
 /*   520 */    23,   22,   18,   20,   19,   21,   28,   27,   26,   25,
 /*   530 */    24,  115,  300,   83,   55,  107,  305,  361,  300,  300,
 /*   540 */   300,  300,  300,  300,  300,  115,  300,   83,  300,  107,
 /*   550 */   305,  361,  300,   61,  357,  358,  356,  300,  359,  300,
 /*   560 */   115,  307,   83,  300,  107,  305,  361,   64,  357,  358,
 /*   570 */   356,  300,  359,  300,  300,  307,  300,  300,  300,  300,
 /*   580 */   300,  300,   65,  357,  358,  356,  300,  359,  300,  300,
 /*   590 */   307,  115,  300,   83,  300,  107,  305,  361,  300,  300,
 /*   600 */   115,  300,   83,  300,  107,  305,  361,  300,  300,  300,
 /*   610 */   300,  300,  300,   62,  357,  358,  356,  300,  359,  300,
 /*   620 */   300,  307,   66,  357,  358,  356,  300,  359,  300,  115,
 /*   630 */   307,   83,  300,  107,  305,  361,  300,  300,  115,  300,
 /*   640 */    83,  300,  107,  305,  361,  300,  300,  300,  300,  300,
 /*   650 */   300,   97,  357,  358,  356,  300,  359,  300,  300,  307,
 /*   660 */    98,  357,  358,  356,  300,  359,  300,  115,  307,   83,
 /*   670 */   300,  107,  305,  361,  300,  300,  115,  300,   83,  300,
 /*   680 */   107,  305,  361,  300,  300,  300,  300,  300,  300,   99,
 /*   690 */   357,  358,  356,  300,  359,  300,  300,  307,  100,  357,
 /*   700 */   358,  356,  300,  359,  300,  115,  307,   83,  300,  107,
 /*   710 */   305,  361,  300,  300,  115,  300,   83,  300,  107,  305,
 /*   720 */   361,  300,  300,  300,  300,  300,  300,   67,  357,  358,
 /*   730 */   356,  300,  359,  300,  300,  307,   70,  357,  358,  356,
 /*   740 */   300,  359,  300,  115,  307,   83,  300,  107,  305,  361,
 /*   750 */   300,  300,  115,  300,   83,  300,  107,  305,  361,  300,
 /*   760 */   300,  300,  300,  300,  300,   71,  357,  358,  356,  300,
 /*   770 */   359,  300,  300,  307,   72,  357,  358,  356,  300,  359,
 /*   780 */   300,  115,  307,   83,  300,  107,  305,  361,  300,  300,
 /*   790 */   115,  300,   83,  300,  107,  305,  361,  300,  300,  300,
 /*   800 */   300,  300,  300,   73,  357,  358,  356,  300,  359,  300,
 /*   810 */   300,  307,   74,  357,  358,  356,  300,  359,  300,  115,
 /*   820 */   307,   83,  300,  107,  305,  361,  300,  300,  115,  300,
 /*   830 */    83,  300,  107,  305,  361,  300,  300,  300,  300,  300,
 /*   840 */   300,   75,  357,  358,  356,  300,  359,  300,  300,  307,
 /*   850 */   103,  357,  358,  356,  300,  359,  300,  115,  307,   83,
 /*   860 */   300,  107,  305,  361,  300,  300,  115,  300,   83,  300,
 /*   870 */   107,  305,  361,  300,  300,  300,  300,  300,  300,  104,
 /*   880 */   357,  358,  356,  300,  359,  300,  300,  307,  105,  357,
 /*   890 */   358,  356,  300,  359,  300,  115,  307,   83,  300,  107,
 /*   900 */   305,  361,  300,  300,  115,  300,   83,  300,  107,  305,
 /*   910 */   361,  300,  300,  300,  300,  300,  300,   77,  357,  358,
 /*   920 */   356,  300,  359,  300,  300,  307,   78,  357,  358,  356,
 /*   930 */   300,  359,  300,  115,  307,   83,  300,  107,  305,  361,
 /*   940 */   300,  300,  115,  300,   83,  300,  107,  305,  361,  300,
 /*   950 */   300,  300,  300,  300,  300,   69,  357,  358,  356,  300,
 /*   960 */   359,  300,  300,  307,   76,  357,  358,  356,  300,  359,
 /*   970 */   300,  115,  307,   83,  300,  107,  305,  361,  300,  300,
 /*   980 */   300,  300,  300,  300,  300,  300,  300,  300,  300,  300,
 /*   990 */   300,  300,  300,   63,  357,  358,  356,  300,  359,  300,
 /*  1000 */   300,  307,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    15,   70,   70,   72,   72,   20,   21,   24,   25,   24,
 /*    10 */    25,   26,   27,   28,   29,   30,   31,   32,   33,   34,
 /*    20 */    35,   36,   39,   40,   70,   40,   72,   71,   74,   73,
 /*    30 */    45,  100,  100,   48,   24,   74,   51,   52,   53,   54,
 /*    40 */    55,   56,   57,   58,   59,   60,   61,   62,   63,   64,
 /*    50 */    65,   66,   15,   42,  100,   68,   69,   20,   21,   98,
 /*    60 */    99,   24,   25,   26,   27,   28,   29,   30,   31,   32,
 /*    70 */    33,   34,   35,   36,   73,   74,   81,   40,   73,   74,
 /*    80 */    43,   39,   40,   82,   83,   48,   48,   71,   83,   73,
 /*    90 */    84,   54,   55,   56,   57,   58,   59,   60,   61,   62,
 /*   100 */    63,   64,   65,   66,   78,   79,   80,   15,   74,   74,
 /*   110 */    88,   85,   20,   21,   46,   47,   24,   25,   26,   27,
 /*   120 */    28,   29,   30,   31,   32,   33,   34,   35,   36,   16,
 /*   130 */    17,   18,   40,   99,   71,   22,   73,   71,   42,   73,
 /*   140 */    48,   71,   47,   73,   49,   74,   54,   55,   56,   57,
 /*   150 */    58,   59,   60,   61,   62,   63,   64,   65,   66,    1,
 /*   160 */     2,    3,    4,   47,   88,   49,    8,    9,   10,   11,
 /*   170 */    12,   13,   14,   15,   16,   17,   18,    2,    3,    4,
 /*   180 */    22,   74,   74,    8,    9,   10,   11,   12,   13,   14,
 /*   190 */    15,   16,   17,   18,   86,   87,   42,   22,   24,   45,
 /*   200 */    24,   43,   26,   27,   28,   29,   30,   31,   32,   33,
 /*   210 */    34,   35,   36,   39,   40,   47,   74,   49,   43,   74,
 /*   220 */    44,    2,    3,    4,   47,   81,   49,    8,    9,   10,
 /*   230 */    11,   12,   13,   14,   15,   16,   17,   18,    2,    3,
 /*   240 */     4,   22,   25,   74,    8,    9,   10,   11,   12,   13,
 /*   250 */    14,   15,   16,   17,   18,   86,   75,   42,   22,   70,
 /*   260 */    45,   72,   43,   74,   75,   76,   77,   46,   47,   84,
 /*   270 */    74,   46,   45,   23,   42,   42,    1,    1,   89,   90,
 /*   280 */    91,   92,   93,   94,   95,   49,   97,   46,   43,  100,
 /*   290 */    46,   41,   70,   22,   72,   42,   74,   75,   76,   77,
 /*   300 */    50,   42,   48,   48,   48,   45,   42,   40,   42,   37,
 /*   310 */    45,   89,   90,   91,   92,   93,   94,   95,   38,   97,
 /*   320 */    43,   70,  100,   72,   42,   74,   75,   76,   77,    0,
 /*   330 */   101,  101,  101,  101,  101,  101,  101,  101,  101,  101,
 /*   340 */    89,   90,   91,   92,   93,   94,   95,  101,   97,  101,
 /*   350 */   101,  100,  101,  101,    2,    3,    4,  101,  101,  101,
 /*   360 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*   370 */    18,  101,  101,  101,   22,  101,  101,  101,  101,  101,
 /*   380 */   101,  101,   70,  101,   72,  101,   74,   75,   76,   77,
 /*   390 */    24,  101,   26,   27,   28,   29,   30,   31,   32,   33,
 /*   400 */    34,   35,   36,   91,   92,   93,   94,   95,  101,   97,
 /*   410 */     4,  101,  100,  101,    8,    9,   10,   11,   12,   13,
 /*   420 */    14,   15,   16,   17,   18,   70,  101,   72,   22,   74,
 /*   430 */    75,   76,  101,  101,   70,  101,   72,  101,   74,   75,
 /*   440 */    76,  101,  101,  101,  101,  101,  101,   92,   93,   94,
 /*   450 */    95,   96,   97,  101,  101,  100,   92,   93,   94,   95,
 /*   460 */    96,   97,  101,  101,  100,  101,  101,  101,  101,  101,
 /*   470 */   101,  101,  101,  101,   70,  101,   72,  101,   74,   75,
 /*   480 */    76,   24,  101,   26,   27,   28,   29,   30,   31,   32,
 /*   490 */    33,   34,   35,   36,  101,  101,   92,   93,   94,   95,
 /*   500 */    96,   97,  101,  101,  100,    8,    9,   10,   11,   12,
 /*   510 */    13,   14,   15,   16,   17,   18,  101,  101,  101,   22,
 /*   520 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*   530 */    18,   70,  101,   72,   22,   74,   75,   76,  101,  101,
 /*   540 */   101,  101,  101,  101,  101,   70,  101,   72,  101,   74,
 /*   550 */    75,   76,  101,   92,   93,   94,   95,  101,   97,  101,
 /*   560 */    70,  100,   72,  101,   74,   75,   76,   92,   93,   94,
 /*   570 */    95,  101,   97,  101,  101,  100,  101,  101,  101,  101,
 /*   580 */   101,  101,   92,   93,   94,   95,  101,   97,  101,  101,
 /*   590 */   100,   70,  101,   72,  101,   74,   75,   76,  101,  101,
 /*   600 */    70,  101,   72,  101,   74,   75,   76,  101,  101,  101,
 /*   610 */   101,  101,  101,   92,   93,   94,   95,  101,   97,  101,
 /*   620 */   101,  100,   92,   93,   94,   95,  101,   97,  101,   70,
 /*   630 */   100,   72,  101,   74,   75,   76,  101,  101,   70,  101,
 /*   640 */    72,  101,   74,   75,   76,  101,  101,  101,  101,  101,
 /*   650 */   101,   92,   93,   94,   95,  101,   97,  101,  101,  100,
 /*   660 */    92,   93,   94,   95,  101,   97,  101,   70,  100,   72,
 /*   670 */   101,   74,   75,   76,  101,  101,   70,  101,   72,  101,
 /*   680 */    74,   75,   76,  101,  101,  101,  101,  101,  101,   92,
 /*   690 */    93,   94,   95,  101,   97,  101,  101,  100,   92,   93,
 /*   700 */    94,   95,  101,   97,  101,   70,  100,   72,  101,   74,
 /*   710 */    75,   76,  101,  101,   70,  101,   72,  101,   74,   75,
 /*   720 */    76,  101,  101,  101,  101,  101,  101,   92,   93,   94,
 /*   730 */    95,  101,   97,  101,  101,  100,   92,   93,   94,   95,
 /*   740 */   101,   97,  101,   70,  100,   72,  101,   74,   75,   76,
 /*   750 */   101,  101,   70,  101,   72,  101,   74,   75,   76,  101,
 /*   760 */   101,  101,  101,  101,  101,   92,   93,   94,   95,  101,
 /*   770 */    97,  101,  101,  100,   92,   93,   94,   95,  101,   97,
 /*   780 */   101,   70,  100,   72,  101,   74,   75,   76,  101,  101,
 /*   790 */    70,  101,   72,  101,   74,   75,   76,  101,  101,  101,
 /*   800 */   101,  101,  101,   92,   93,   94,   95,  101,   97,  101,
 /*   810 */   101,  100,   92,   93,   94,   95,  101,   97,  101,   70,
 /*   820 */   100,   72,  101,   74,   75,   76,  101,  101,   70,  101,
 /*   830 */    72,  101,   74,   75,   76,  101,  101,  101,  101,  101,
 /*   840 */   101,   92,   93,   94,   95,  101,   97,  101,  101,  100,
 /*   850 */    92,   93,   94,   95,  101,   97,  101,   70,  100,   72,
 /*   860 */   101,   74,   75,   76,  101,  101,   70,  101,   72,  101,
 /*   870 */    74,   75,   76,  101,  101,  101,  101,  101,  101,   92,
 /*   880 */    93,   94,   95,  101,   97,  101,  101,  100,   92,   93,
 /*   890 */    94,   95,  101,   97,  101,   70,  100,   72,  101,   74,
 /*   900 */    75,   76,  101,  101,   70,  101,   72,  101,   74,   75,
 /*   910 */    76,  101,  101,  101,  101,  101,  101,   92,   93,   94,
 /*   920 */    95,  101,   97,  101,  101,  100,   92,   93,   94,   95,
 /*   930 */   101,   97,  101,   70,  100,   72,  101,   74,   75,   76,
 /*   940 */   101,  101,   70,  101,   72,  101,   74,   75,   76,  101,
 /*   950 */   101,  101,  101,  101,  101,   92,   93,   94,   95,  101,
 /*   960 */    97,  101,  101,  100,   92,   93,   94,   95,  101,   97,
 /*   970 */   101,   70,  100,   72,  101,   74,   75,   76,  101,  101,
 /*   980 */   101,  101,  101,  101,  101,  101,  101,  101,  101,  101,
 /*   990 */   101,  101,  101,   92,   93,   94,   95,  101,   97,  101,
 /*  1000 */   101,  100,
};
#define YY_SHIFT_USE_DFLT (1002)
#define YY_SHIFT_COUNT    (117)
#define YY_SHIFT_MIN      (-17)
#define YY_SHIFT_MAX      (512)
static const short yy_shift_ofst[] = {
 /*     0 */  1002,  -15,  -15,  -15,  -15,   92,   92,   92,   37,   92,
 /*    10 */    92,   92,   92,   92,   92,   92,   92,   92,   92,   92,
 /*    20 */    92,   92,   92,   92,   92,   92,   92,   92,   92,   92,
 /*    30 */    92,   92,  366,  -17,  250,  176,  457,  174,   10,   10,
 /*    40 */    42,   42,   42,   42,   42,   10,   10,   11,   38,   10,
 /*    50 */    96,   10,   96,   10,   10,   10,   11,  217,   38,   10,
 /*    60 */   158,  175,  219,  236,  352,  352,  352,  352,  352,  406,
 /*    70 */   497,  497,  497,  497,  497,  497,  512,  113,  113,   95,
 /*    80 */   168,   68,  116,  154,  177,  215,  221,  225,  227,  232,
 /*    90 */   233,  275,  276,  241,  245,  244,  253,  271,  271,  271,
 /*   100 */   271,  254,  259,  271,  271,  271,  255,  256,  260,  264,
 /*   110 */   267,  266,  265,  277,  280,  272,  282,  329,
};
#define YY_REDUCE_USE_DFLT (-70)
#define YY_REDUCE_COUNT (59)
#define YY_REDUCE_MIN   (-69)
#define YY_REDUCE_MAX   (901)
static const short yy_reduce_ofst[] = {
 /*     0 */   -13,  189,  222,  251,  312,  355,  364,  404,  461,  475,
 /*    10 */   490,  521,  530,  559,  568,  597,  606,  635,  644,  673,
 /*    20 */   682,  711,  720,  749,  758,  787,  796,  825,  834,  863,
 /*    30 */   872,  901,  -46,    1,   26,  -69,  -68,    5,  -39,  108,
 /*    40 */   -44,   16,   63,   66,   70,   34,  169,   -5,    6,   35,
 /*    50 */    22,   71,   76,  107,  142,  145,  144,  181,  185,  196,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   301,  341,  341,  341,  342,  385,  385,  385,  406,  406,
 /*    10 */   406,  406,  406,  406,  406,  406,  406,  406,  406,  406,
 /*    20 */   406,  406,  406,  406,  406,  406,  406,  406,  406,  406,
 /*    30 */   406,  406,  406,  336,  406,  406,  406,  320,  387,  406,
 /*    40 */   320,  320,  320,  320,  320,  406,  406,  340,  406,  406,
 /*    50 */   354,  406,  354,  406,  406,  406,  340,  406,  406,  406,
 /*    60 */   406,  406,  406,  406,  352,  351,  390,  383,  384,  367,
 /*    70 */   378,  377,  376,  375,  374,  373,  366,  369,  368,  406,
 /*    80 */   406,  406,  406,  406,  406,  302,  406,  406,  406,  406,
 /*    90 */   406,  406,  406,  406,  406,  406,  406,  382,  381,  380,
 /*   100 */   379,  406,  406,  372,  371,  370,  362,  304,  406,  406,
 /*   110 */   321,  406,  406,  406,  406,  306,  406,  406,
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
  "$",             "ASSIGN",        "AS",            "OR",          
  "AND",           "BIT_OR",        "BIT_XOR",       "BIT_AND",     
  "EQ",            "NE",            "GT",            "LT",          
  "GE",            "LE",            "PLUS",          "MINUS",       
  "MULT",          "DIV",           "MOD",           "NEGATE",      
  "BIT_NOT",       "NOT",           "DOT",           "END",         
  "IDENTIFIER",    "SELF",          "TYPE_BOOL",     "TYPE_INT8",   
  "TYPE_UINT8",    "TYPE_INT16",    "TYPE_UINT16",   "TYPE_INT32",  
  "TYPE_UINT32",   "TYPE_INT64",    "TYPE_UINT64",   "TYPE_FLOAT",  
  "TYPE_DOUBLE",   "ARRAY_BEGIN",   "ARRAY_END",     "MUT",         
  "REFERENCE",     "TYPE",          "COLON",         "SEMI_COLON",  
  "STRUCT",        "SCOPE_BEGIN",   "SCOPE_END",     "COMMA",       
  "PAR_BEGIN",     "PAR_END",       "FUNCTION",      "RETURN",      
  "VAR",           "LET",           "INTEGER_LITERAL",  "INTEGER_LITERAL_I8",
  "INTEGER_LITERAL_I16",  "INTEGER_LITERAL_I32",  "INTEGER_LITERAL_I64",  "INTEGER_LITERAL_U8",
  "INTEGER_LITERAL_U16",  "INTEGER_LITERAL_U32",  "INTEGER_LITERAL_U64",  "FLOAT_LITERAL",
  "FLOAT_LITERAL_32",  "FLOAT_LITERAL_64",  "BOOL_LITERAL",  "error",       
  "decls",         "module",        "type_builtin",  "type_qualified",
  "type_specifier",  "qualifier",     "identifier",    "self",        
  "identifier_or_self",  "decl_var",      "decl_type",     "decl_function",
  "decl_type_function",  "function_return_decl",  "function_args_decl",  "function_arg_decl",
  "function_param_list",  "decl_struct",   "struct_decl_var",  "struct_decl_vars",
  "var_type_spec",  "statement_list",  "statement_list_or_empty",  "statement",   
  "expression",    "unaryexp",      "binaryexp",     "literal",     
  "function_call_args",  "struct_init",   "struct_member_init_list",  "struct_member_init",
  "type_array",  
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
 /*  52 */ "statement ::= SCOPE_BEGIN statement_list_or_empty SCOPE_END",
 /*  53 */ "decl_var ::= VAR identifier var_type_spec ASSIGN expression",
 /*  54 */ "decl_var ::= LET identifier var_type_spec ASSIGN expression",
 /*  55 */ "var_type_spec ::= COLON type_qualified",
 /*  56 */ "var_type_spec ::=",
 /*  57 */ "expression ::= PAR_BEGIN expression PAR_END",
 /*  58 */ "expression ::= literal",
 /*  59 */ "expression ::= unaryexp",
 /*  60 */ "expression ::= binaryexp",
 /*  61 */ "expression ::= struct_init",
 /*  62 */ "expression ::= expression AS type_qualified",
 /*  63 */ "expression ::= identifier_or_self",
 /*  64 */ "expression ::= expression DOT identifier",
 /*  65 */ "expression ::= identifier PAR_BEGIN function_call_args PAR_END",
 /*  66 */ "expression ::= expression DOT identifier PAR_BEGIN function_call_args PAR_END",
 /*  67 */ "expression ::= type_specifier COLON COLON identifier PAR_BEGIN function_call_args PAR_END",
 /*  68 */ "binaryexp ::= expression AND expression",
 /*  69 */ "binaryexp ::= expression OR expression",
 /*  70 */ "binaryexp ::= expression PLUS expression",
 /*  71 */ "binaryexp ::= expression MINUS expression",
 /*  72 */ "binaryexp ::= expression MULT expression",
 /*  73 */ "binaryexp ::= expression DIV expression",
 /*  74 */ "binaryexp ::= expression MOD expression",
 /*  75 */ "binaryexp ::= expression EQ expression",
 /*  76 */ "binaryexp ::= expression NE expression",
 /*  77 */ "binaryexp ::= expression LE expression",
 /*  78 */ "binaryexp ::= expression LT expression",
 /*  79 */ "binaryexp ::= expression GE expression",
 /*  80 */ "binaryexp ::= expression GT expression",
 /*  81 */ "unaryexp ::= NOT expression",
 /*  82 */ "unaryexp ::= BIT_NOT expression",
 /*  83 */ "unaryexp ::= MINUS expression",
 /*  84 */ "unaryexp ::= REFERENCE expression",
 /*  85 */ "function_call_args ::= function_call_args COMMA expression",
 /*  86 */ "function_call_args ::= expression",
 /*  87 */ "function_call_args ::=",
 /*  88 */ "struct_init ::= type_specifier SCOPE_BEGIN struct_member_init_list SCOPE_END",
 /*  89 */ "struct_member_init_list ::=",
 /*  90 */ "struct_member_init_list ::= struct_member_init_list COMMA struct_member_init",
 /*  91 */ "struct_member_init_list ::= struct_member_init",
 /*  92 */ "struct_member_init ::= identifier COLON expression",
 /*  93 */ "literal ::= INTEGER_LITERAL",
 /*  94 */ "literal ::= INTEGER_LITERAL_I8",
 /*  95 */ "literal ::= INTEGER_LITERAL_I16",
 /*  96 */ "literal ::= INTEGER_LITERAL_I32",
 /*  97 */ "literal ::= INTEGER_LITERAL_I64",
 /*  98 */ "literal ::= INTEGER_LITERAL_U8",
 /*  99 */ "literal ::= INTEGER_LITERAL_U16",
 /* 100 */ "literal ::= INTEGER_LITERAL_U32",
 /* 101 */ "literal ::= INTEGER_LITERAL_U64",
 /* 102 */ "literal ::= FLOAT_LITERAL",
 /* 103 */ "literal ::= FLOAT_LITERAL_32",
 /* 104 */ "literal ::= FLOAT_LITERAL_64",
 /* 105 */ "literal ::= BOOL_LITERAL",
 /* 106 */ "module ::= decls END",
 /* 107 */ "type_array ::= type_builtin ARRAY_BEGIN ARRAY_END",
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
  { 90, 0 },
  { 90, -1 },
  { 89, -2 },
  { 89, -1 },
  { 91, -4 },
  { 91, -2 },
  { 91, -2 },
  { 91, -3 },
  { 91, -2 },
  { 91, -3 },
  { 77, -5 },
  { 77, -5 },
  { 88, -2 },
  { 88, 0 },
  { 92, -3 },
  { 92, -1 },
  { 92, -1 },
  { 92, -1 },
  { 92, -1 },
  { 92, -3 },
  { 92, -1 },
  { 92, -3 },
  { 92, -4 },
  { 92, -6 },
  { 92, -7 },
  { 94, -3 },
  { 94, -3 },
  { 94, -3 },
  { 94, -3 },
  { 94, -3 },
  { 94, -3 },
  { 94, -3 },
  { 94, -3 },
  { 94, -3 },
  { 94, -3 },
  { 94, -3 },
  { 94, -3 },
  { 94, -3 },
  { 93, -2 },
  { 93, -2 },
  { 93, -2 },
  { 93, -2 },
  { 96, -3 },
  { 96, -1 },
  { 96, 0 },
  { 97, -4 },
  { 98, 0 },
  { 98, -3 },
  { 98, -1 },
  { 99, -3 },
  { 95, -1 },
  { 95, -1 },
  { 95, -1 },
  { 95, -1 },
  { 95, -1 },
  { 95, -1 },
  { 95, -1 },
  { 95, -1 },
  { 95, -1 },
  { 95, -1 },
  { 95, -1 },
  { 95, -1 },
  { 95, -1 },
  { 69, -2 },
  { 100, -3 },
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
    yylhsminor.yy66 =yymsp[-1].minor.yy66; yymsp[-1].minor.yy66->addDecl(yymsp[0].minor.yy108);
}
#line 1332 "stparserimpl.c"
  yymsp[-1].minor.yy66 = yylhsminor.yy66;
        break;
      case 1: /* decls ::= decls decl_function */
      case 2: /* decls ::= decls decl_type_function */ yytestcase(yyruleno==2);
#line 128 "stparserimpl.lemon"
{
        yylhsminor.yy66 =yymsp[-1].minor.yy66; yymsp[-1].minor.yy66->addDecl(yymsp[0].minor.yy68);
}
#line 1341 "stparserimpl.c"
  yymsp[-1].minor.yy66 = yylhsminor.yy66;
        break;
      case 3: /* decls ::= */
#line 134 "stparserimpl.lemon"
{
        yymsp[1].minor.yy66 =  pParser->getDeclModule();
     }
#line 1349 "stparserimpl.c"
        break;
      case 4: /* identifier ::= IDENTIFIER */
      case 5: /* self ::= SELF */ yytestcase(yyruleno==5);
#line 141 "stparserimpl.lemon"
{
        yylhsminor.yy57 = pParser->createNode<yal::frontend::STIdentifier>(yymsp[0].minor.yy0.tokenStr);
        auto srcInfo = CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy57->setSourceInfo(srcInfo);
}
#line 1359 "stparserimpl.c"
  yymsp[0].minor.yy57 = yylhsminor.yy57;
        break;
      case 6: /* identifier_or_self ::= identifier */
      case 7: /* identifier_or_self ::= self */ yytestcase(yyruleno==7);
#line 153 "stparserimpl.lemon"
{yylhsminor.yy57=yymsp[0].minor.yy57;}
#line 1366 "stparserimpl.c"
  yymsp[0].minor.yy57 = yylhsminor.yy57;
        break;
      case 8: /* type_specifier ::= type_builtin */
#line 156 "stparserimpl.lemon"
{
        yylhsminor.yy74=yymsp[0].minor.yy74;
}
#line 1374 "stparserimpl.c"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 9: /* type_specifier ::= type_array */
#line 160 "stparserimpl.lemon"
{yymsp[0].minor.yy74 = nullptr;}
#line 1380 "stparserimpl.c"
        break;
      case 10: /* type_specifier ::= IDENTIFIER */
#line 161 "stparserimpl.lemon"
{
    yylhsminor.yy74 = pParser->createNode<yal::frontend::STType>(yymsp[0].minor.yy0.tokenStr);
    yylhsminor.yy74->setSourceInfo(yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0,yymsp[0].minor.yy0, pParser->getSourceHandle()));
}
#line 1388 "stparserimpl.c"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 11: /* type_builtin ::= TYPE_BOOL */
#line 166 "stparserimpl.lemon"
{
    yylhsminor.yy74 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Bool);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy74->setSourceInfo(srcInfo);
    }
#line 1398 "stparserimpl.c"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 12: /* type_builtin ::= TYPE_INT8 */
#line 171 "stparserimpl.lemon"
{
        yylhsminor.yy74 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int8);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy74->setSourceInfo(srcInfo);
    }
#line 1408 "stparserimpl.c"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 13: /* type_builtin ::= TYPE_UINT8 */
#line 176 "stparserimpl.lemon"
{
        yylhsminor.yy74 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt8);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy74->setSourceInfo(srcInfo);
    }
#line 1418 "stparserimpl.c"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 14: /* type_builtin ::= TYPE_INT16 */
#line 181 "stparserimpl.lemon"
{
        yylhsminor.yy74 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int16);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy74->setSourceInfo(srcInfo);
    }
#line 1428 "stparserimpl.c"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 15: /* type_builtin ::= TYPE_UINT16 */
#line 186 "stparserimpl.lemon"
{
        yylhsminor.yy74 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt16);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy74->setSourceInfo(srcInfo);
    }
#line 1438 "stparserimpl.c"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 16: /* type_builtin ::= TYPE_INT32 */
#line 191 "stparserimpl.lemon"
{
        yylhsminor.yy74 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int32);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy74->setSourceInfo(srcInfo);
    }
#line 1448 "stparserimpl.c"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 17: /* type_builtin ::= TYPE_UINT32 */
#line 196 "stparserimpl.lemon"
{
        yylhsminor.yy74 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt32);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy74->setSourceInfo(srcInfo);
    }
#line 1458 "stparserimpl.c"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 18: /* type_builtin ::= TYPE_INT64 */
#line 201 "stparserimpl.lemon"
{
        yylhsminor.yy74 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int64);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy74->setSourceInfo(srcInfo);
    }
#line 1468 "stparserimpl.c"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 19: /* type_builtin ::= TYPE_UINT64 */
#line 206 "stparserimpl.lemon"
{
        yylhsminor.yy74 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt64);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy74->setSourceInfo(srcInfo);
    }
#line 1478 "stparserimpl.c"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 20: /* type_builtin ::= TYPE_FLOAT */
#line 211 "stparserimpl.lemon"
{
        yylhsminor.yy74 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Float32);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy74->setSourceInfo(srcInfo);
    }
#line 1488 "stparserimpl.c"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 21: /* type_builtin ::= TYPE_DOUBLE */
#line 216 "stparserimpl.lemon"
{
        yylhsminor.yy74 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Float64);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy74->setSourceInfo(srcInfo);
    }
#line 1498 "stparserimpl.c"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 22: /* qualifier ::= */
#line 224 "stparserimpl.lemon"
{yymsp[1].minor.yy72 = 0;}
#line 1504 "stparserimpl.c"
        break;
      case 23: /* qualifier ::= MUT */
#line 225 "stparserimpl.lemon"
{yymsp[0].minor.yy72 = yal::frontend::STQualType::Qualifiers::kQualMutable;}
#line 1509 "stparserimpl.c"
        break;
      case 24: /* qualifier ::= REFERENCE */
#line 226 "stparserimpl.lemon"
{yymsp[0].minor.yy72 = yal::frontend::STQualType::Qualifiers::kQualReference;}
#line 1514 "stparserimpl.c"
        break;
      case 25: /* qualifier ::= MUT REFERENCE */
#line 227 "stparserimpl.lemon"
{
        yymsp[-1].minor.yy72 = yal::frontend::STQualType::Qualifiers::kQualReference | yal::frontend::STQualType::Qualifiers::kQualMutable;
 }
#line 1521 "stparserimpl.c"
        break;
      case 26: /* type_qualified ::= qualifier type_specifier */
#line 230 "stparserimpl.lemon"
{
    yylhsminor.yy71 = pParser->createNode<yal::frontend::STQualType>(yymsp[0].minor.yy74, yymsp[-1].minor.yy72);
}
#line 1528 "stparserimpl.c"
  yymsp[-1].minor.yy71 = yylhsminor.yy71;
        break;
      case 27: /* decl_type ::= TYPE identifier COLON type_specifier SEMI_COLON */
#line 236 "stparserimpl.lemon"
{
    yymsp[-4].minor.yy108 = nullptr;
}
#line 1536 "stparserimpl.c"
        break;
      case 28: /* decl_type ::= decl_struct */
#line 240 "stparserimpl.lemon"
{ yylhsminor.yy108 = yymsp[0].minor.yy77;}
#line 1541 "stparserimpl.c"
  yymsp[0].minor.yy108 = yylhsminor.yy108;
        break;
      case 29: /* decl_struct ::= TYPE identifier COLON STRUCT SCOPE_BEGIN struct_decl_vars SCOPE_END */
#line 245 "stparserimpl.lemon"
{
    yylhsminor.yy77 = pParser->createNode<yal::frontend::STDeclStruct>(yymsp[-5].minor.yy57, yymsp[-1].minor.yy34, *pParser);
    auto srcInfo = CreateSourceInfo(yymsp[-6].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy77->setSourceInfo(srcInfo);
}
#line 1551 "stparserimpl.c"
  yymsp[-6].minor.yy77 = yylhsminor.yy77;
        break;
      case 30: /* struct_decl_vars ::= struct_decl_vars COMMA struct_decl_var */
#line 251 "stparserimpl.lemon"
{
    auto& list = pParser->getStructMemberList();
    yylhsminor.yy34 = list.add(yymsp[-2].minor.yy34, yymsp[0].minor.yy25);
}
#line 1560 "stparserimpl.c"
  yymsp[-2].minor.yy34 = yylhsminor.yy34;
        break;
      case 31: /* struct_decl_vars ::= struct_decl_var */
#line 255 "stparserimpl.lemon"
{
    auto& list = pParser->getStructMemberList();
    yylhsminor.yy34 = list.add(yymsp[0].minor.yy25);
}
#line 1569 "stparserimpl.c"
  yymsp[0].minor.yy34 = yylhsminor.yy34;
        break;
      case 32: /* struct_decl_var ::= identifier COLON type_qualified */
#line 260 "stparserimpl.lemon"
{
    yylhsminor.yy25 = pParser->createNode<yal::frontend::STStructMember>(yymsp[-2].minor.yy57, yymsp[0].minor.yy71);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy57->getSourceInfo(), yymsp[0].minor.yy71->getSourceInfo(),pParser->getSourceHandle());
    yylhsminor.yy25->setSourceInfo(srcInfo);
}
#line 1579 "stparserimpl.c"
  yymsp[-2].minor.yy25 = yylhsminor.yy25;
        break;
      case 33: /* function_param_list ::= PAR_BEGIN function_args_decl PAR_END */
#line 269 "stparserimpl.lemon"
{
    yymsp[-2].minor.yy69 = yymsp[-1].minor.yy69;
}
#line 1587 "stparserimpl.c"
        break;
      case 34: /* decl_function ::= FUNCTION identifier function_param_list function_return_decl SCOPE_BEGIN statement_list_or_empty SCOPE_END */
#line 274 "stparserimpl.lemon"
{
    yylhsminor.yy68 = pParser->createNode<yal::frontend::STDeclFunction>(yymsp[-5].minor.yy57, nullptr, yymsp[-3].minor.yy71, yymsp[-4].minor.yy69, yymsp[-1].minor.yy47, *pParser);
    yylhsminor.yy68->setSourceInfo(CreateSourceInfo(yymsp[-6].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle()));
}
#line 1595 "stparserimpl.c"
  yymsp[-6].minor.yy68 = yylhsminor.yy68;
        break;
      case 35: /* decl_type_function ::= FUNCTION type_specifier COLON COLON identifier function_param_list function_return_decl SCOPE_BEGIN statement_list_or_empty SCOPE_END */
#line 282 "stparserimpl.lemon"
{
    yylhsminor.yy68 = pParser->createNode<yal::frontend::STDeclFunction>(yymsp[-5].minor.yy57, yymsp[-8].minor.yy74, yymsp[-3].minor.yy71, yymsp[-4].minor.yy69, yymsp[-1].minor.yy47, *pParser);
    yylhsminor.yy68->setSourceInfo(CreateSourceInfo(yymsp[-9].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle()));
}
#line 1604 "stparserimpl.c"
  yymsp[-9].minor.yy68 = yylhsminor.yy68;
        break;
      case 36: /* function_args_decl ::= function_args_decl COMMA function_arg_decl */
#line 289 "stparserimpl.lemon"
{
        auto& list = pParser->getDeclParamList();
        yylhsminor.yy69 = list.add(yymsp[-2].minor.yy69, yymsp[0].minor.yy3);
}
#line 1613 "stparserimpl.c"
  yymsp[-2].minor.yy69 = yylhsminor.yy69;
        break;
      case 37: /* function_args_decl ::= function_arg_decl */
#line 294 "stparserimpl.lemon"
{
    auto& list = pParser->getDeclParamList();
    yylhsminor.yy69 = list.add(yymsp[0].minor.yy3);
}
#line 1622 "stparserimpl.c"
  yymsp[0].minor.yy69 = yylhsminor.yy69;
        break;
      case 38: /* function_args_decl ::= */
#line 299 "stparserimpl.lemon"
{
   yymsp[1].minor.yy69 = pParser->getDeclParamList().getRangeEmpty();
}
#line 1630 "stparserimpl.c"
        break;
      case 39: /* function_arg_decl ::= identifier COLON type_qualified */
#line 303 "stparserimpl.lemon"
{
        yylhsminor.yy3 = pParser->createNode<yal::frontend::STDeclParam>(yymsp[-2].minor.yy57, yymsp[0].minor.yy71);
        yylhsminor.yy3->setSourceInfo(yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy57->getSourceInfo(),yymsp[0].minor.yy71->getSourceInfo(), pParser->getSourceHandle()));
}
#line 1638 "stparserimpl.c"
  yymsp[-2].minor.yy3 = yylhsminor.yy3;
        break;
      case 40: /* function_arg_decl ::= qualifier self */
#line 308 "stparserimpl.lemon"
{
        auto qualType = pParser->createNode<yal::frontend::STQualType>(nullptr, yymsp[-1].minor.yy72);
        yylhsminor.yy3 = pParser->createNode<yal::frontend::STDeclParam>(yymsp[0].minor.yy57, qualType);
        yylhsminor.yy3->setSourceInfo(yymsp[0].minor.yy57->getSourceInfo());
}
#line 1648 "stparserimpl.c"
  yymsp[-1].minor.yy3 = yylhsminor.yy3;
        break;
      case 41: /* function_return_decl ::= COLON type_qualified */
#line 314 "stparserimpl.lemon"
{ yymsp[-1].minor.yy71 = yymsp[0].minor.yy71;}
#line 1654 "stparserimpl.c"
        break;
      case 42: /* function_return_decl ::= */
      case 56: /* var_type_spec ::= */ yytestcase(yyruleno==56);
#line 315 "stparserimpl.lemon"
{yymsp[1].minor.yy71 = nullptr;}
#line 1660 "stparserimpl.c"
        break;
      case 43: /* statement_list_or_empty ::= */
#line 319 "stparserimpl.lemon"
{yymsp[1].minor.yy47 = pParser->getStmtList().getRangeEmpty();}
#line 1665 "stparserimpl.c"
        break;
      case 44: /* statement_list_or_empty ::= statement_list */
#line 320 "stparserimpl.lemon"
{yylhsminor.yy47 = yymsp[0].minor.yy47;}
#line 1670 "stparserimpl.c"
  yymsp[0].minor.yy47 = yylhsminor.yy47;
        break;
      case 45: /* statement_list ::= statement_list statement */
#line 323 "stparserimpl.lemon"
{
    auto& list = pParser->getStmtList();
    yylhsminor.yy47 = list.add(yymsp[-1].minor.yy47,yymsp[0].minor.yy129);
}
#line 1679 "stparserimpl.c"
  yymsp[-1].minor.yy47 = yylhsminor.yy47;
        break;
      case 46: /* statement_list ::= statement */
#line 327 "stparserimpl.lemon"
{
    auto& list = pParser->getStmtList();
    yylhsminor.yy47 = list.add(yymsp[0].minor.yy129);
}
#line 1688 "stparserimpl.c"
  yymsp[0].minor.yy47 = yylhsminor.yy47;
        break;
      case 47: /* statement ::= expression ASSIGN expression SEMI_COLON */
#line 333 "stparserimpl.lemon"
{
   yylhsminor.yy129 = pParser->createNode<yal::frontend::STStmtAssign>(yymsp[-3].minor.yy162,yymsp[-1].minor.yy162);
   yylhsminor.yy129->setSourceInfo(yal::frontend::CreateSourceInfo(yymsp[-3].minor.yy162->getSourceInfo(), yymsp[-1].minor.yy162->getSourceInfo(), pParser->getSourceHandle()));
}
#line 1697 "stparserimpl.c"
  yymsp[-3].minor.yy129 = yylhsminor.yy129;
        break;
      case 48: /* statement ::= decl_var SEMI_COLON */
#line 337 "stparserimpl.lemon"
{
    yylhsminor.yy129 = pParser->createNode<yal::frontend::STStmtDecl>(yymsp[-1].minor.yy167);
    yylhsminor.yy129->setSourceInfo(yymsp[-1].minor.yy167->getSourceInfo());
}
#line 1706 "stparserimpl.c"
  yymsp[-1].minor.yy129 = yylhsminor.yy129;
        break;
      case 49: /* statement ::= expression SEMI_COLON */
#line 342 "stparserimpl.lemon"
{
    yylhsminor.yy129 = pParser->createNode<yal::frontend::STStmtExpression>(yymsp[-1].minor.yy162);
    yylhsminor.yy129->setSourceInfo(yymsp[-1].minor.yy162->getSourceInfo());
}
#line 1715 "stparserimpl.c"
  yymsp[-1].minor.yy129 = yylhsminor.yy129;
        break;
      case 50: /* statement ::= RETURN expression SEMI_COLON */
#line 347 "stparserimpl.lemon"
{
    yylhsminor.yy129 = pParser->createNode<yal::frontend::STStmtReturn>(yymsp[-1].minor.yy162);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy0, yymsp[-1].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy129->setSourceInfo(srcInfo);
}
#line 1725 "stparserimpl.c"
  yymsp[-2].minor.yy129 = yylhsminor.yy129;
        break;
      case 51: /* statement ::= RETURN SEMI_COLON */
#line 353 "stparserimpl.lemon"
{
    yylhsminor.yy129 = pParser->createNode<yal::frontend::STStmtReturn>(nullptr);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[-1].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy129->setSourceInfo(srcInfo);
}
#line 1735 "stparserimpl.c"
  yymsp[-1].minor.yy129 = yylhsminor.yy129;
        break;
      case 52: /* statement ::= SCOPE_BEGIN statement_list_or_empty SCOPE_END */
#line 359 "stparserimpl.lemon"
{
    yylhsminor.yy129 = pParser->createNode<yal::frontend::STStmtListScoped>(yymsp[-1].minor.yy47, *pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy129->setSourceInfo(srcInfo);
}
#line 1745 "stparserimpl.c"
  yymsp[-2].minor.yy129 = yylhsminor.yy129;
        break;
      case 53: /* decl_var ::= VAR identifier var_type_spec ASSIGN expression */
      case 54: /* decl_var ::= LET identifier var_type_spec ASSIGN expression */ yytestcase(yyruleno==54);
#line 367 "stparserimpl.lemon"
{
    yylhsminor.yy167 = pParser->createNode<yal::frontend::STDeclVar>(yymsp[-3].minor.yy57, yymsp[-2].minor.yy71, yymsp[0].minor.yy162);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy167->setSourceInfo(srcInfo);
}
#line 1756 "stparserimpl.c"
  yymsp[-4].minor.yy167 = yylhsminor.yy167;
        break;
      case 55: /* var_type_spec ::= COLON type_qualified */
#line 379 "stparserimpl.lemon"
{yymsp[-1].minor.yy71 = yymsp[0].minor.yy71;}
#line 1762 "stparserimpl.c"
        break;
      case 57: /* expression ::= PAR_BEGIN expression PAR_END */
#line 385 "stparserimpl.lemon"
{yymsp[-2].minor.yy162 = yymsp[-1].minor.yy162;}
#line 1767 "stparserimpl.c"
        break;
      case 58: /* expression ::= literal */
#line 386 "stparserimpl.lemon"
{yylhsminor.yy162 = yymsp[0].minor.yy162;}
#line 1772 "stparserimpl.c"
  yymsp[0].minor.yy162 = yylhsminor.yy162;
        break;
      case 59: /* expression ::= unaryexp */
#line 387 "stparserimpl.lemon"
{yylhsminor.yy162 = yymsp[0].minor.yy2;}
#line 1778 "stparserimpl.c"
  yymsp[0].minor.yy162 = yylhsminor.yy162;
        break;
      case 60: /* expression ::= binaryexp */
#line 388 "stparserimpl.lemon"
{yylhsminor.yy162 = yymsp[0].minor.yy4;}
#line 1784 "stparserimpl.c"
  yymsp[0].minor.yy162 = yylhsminor.yy162;
        break;
      case 61: /* expression ::= struct_init */
#line 389 "stparserimpl.lemon"
{ yylhsminor.yy162 = yymsp[0].minor.yy100;}
#line 1790 "stparserimpl.c"
  yymsp[0].minor.yy162 = yylhsminor.yy162;
        break;
      case 62: /* expression ::= expression AS type_qualified */
#line 391 "stparserimpl.lemon"
{
    yylhsminor.yy162 = pParser->createNode<yal::frontend::STExprCast>(yymsp[-2].minor.yy162, yymsp[0].minor.yy71);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy162->getSourceInfo(),
                                                   yymsp[0].minor.yy71->getSourceInfo(),
                                                   pParser->getSourceHandle());
    yylhsminor.yy162->setSourceInfo(srcInfo);
}
#line 1802 "stparserimpl.c"
  yymsp[-2].minor.yy162 = yylhsminor.yy162;
        break;
      case 63: /* expression ::= identifier_or_self */
#line 399 "stparserimpl.lemon"
{
    yylhsminor.yy162 = pParser->createNode<yal::frontend::STExprVarRef>(yymsp[0].minor.yy57);
    yylhsminor.yy162->setSourceInfo(yymsp[0].minor.yy57->getSourceInfo());
}
#line 1811 "stparserimpl.c"
  yymsp[0].minor.yy162 = yylhsminor.yy162;
        break;
      case 64: /* expression ::= expression DOT identifier */
#line 404 "stparserimpl.lemon"
{
    yylhsminor.yy162 = pParser->createNode<yal::frontend::STExprStructVarRef>(yymsp[-2].minor.yy162, yymsp[0].minor.yy57);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy162->getSourceInfo(), yymsp[0].minor.yy57->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy162->setSourceInfo(srcInfo);
}
#line 1821 "stparserimpl.c"
  yymsp[-2].minor.yy162 = yylhsminor.yy162;
        break;
      case 65: /* expression ::= identifier PAR_BEGIN function_call_args PAR_END */
#line 410 "stparserimpl.lemon"
{
    yylhsminor.yy162 = pParser->createNode<yal::frontend::STExprFnCall>(yymsp[-3].minor.yy57,yymsp[-1].minor.yy200,*pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-3].minor.yy57->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy162->setSourceInfo(srcInfo);
}
#line 1831 "stparserimpl.c"
  yymsp[-3].minor.yy162 = yylhsminor.yy162;
        break;
      case 66: /* expression ::= expression DOT identifier PAR_BEGIN function_call_args PAR_END */
#line 416 "stparserimpl.lemon"
{
    yylhsminor.yy162 = pParser->createNode<yal::frontend::STExprFnCall>(yymsp[-3].minor.yy57,yymsp[-5].minor.yy162,yymsp[-1].minor.yy200,*pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-5].minor.yy162->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy162->setSourceInfo(srcInfo);
}
#line 1841 "stparserimpl.c"
  yymsp[-5].minor.yy162 = yylhsminor.yy162;
        break;
      case 67: /* expression ::= type_specifier COLON COLON identifier PAR_BEGIN function_call_args PAR_END */
#line 422 "stparserimpl.lemon"
{
    yylhsminor.yy162 = pParser->createNode<yal::frontend::STExprFnCall>(yymsp[-3].minor.yy57,yymsp[-6].minor.yy74,yymsp[-1].minor.yy200,*pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-6].minor.yy74->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy162->setSourceInfo(srcInfo);
}
#line 1851 "stparserimpl.c"
  yymsp[-6].minor.yy162 = yylhsminor.yy162;
        break;
      case 68: /* binaryexp ::= expression AND expression */
#line 430 "stparserimpl.lemon"
{
        yylhsminor.yy4 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::And,
                                                        yymsp[-2].minor.yy162, yymsp[0].minor.yy162);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy162->getSourceInfo(), yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy4->setSourceInfo(srcInfo);
}
#line 1862 "stparserimpl.c"
  yymsp[-2].minor.yy4 = yylhsminor.yy4;
        break;
      case 69: /* binaryexp ::= expression OR expression */
#line 436 "stparserimpl.lemon"
{
        yylhsminor.yy4 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Or,
                                                        yymsp[-2].minor.yy162, yymsp[0].minor.yy162);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy162->getSourceInfo(), yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy4->setSourceInfo(srcInfo);
}
#line 1873 "stparserimpl.c"
  yymsp[-2].minor.yy4 = yylhsminor.yy4;
        break;
      case 70: /* binaryexp ::= expression PLUS expression */
#line 442 "stparserimpl.lemon"
{
        yylhsminor.yy4 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Plus,
                                                        yymsp[-2].minor.yy162, yymsp[0].minor.yy162);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy162->getSourceInfo(), yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy4->setSourceInfo(srcInfo);
}
#line 1884 "stparserimpl.c"
  yymsp[-2].minor.yy4 = yylhsminor.yy4;
        break;
      case 71: /* binaryexp ::= expression MINUS expression */
#line 448 "stparserimpl.lemon"
{
        yylhsminor.yy4 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Minus,
                                                        yymsp[-2].minor.yy162, yymsp[0].minor.yy162);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy162->getSourceInfo(), yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy4->setSourceInfo(srcInfo);
}
#line 1895 "stparserimpl.c"
  yymsp[-2].minor.yy4 = yylhsminor.yy4;
        break;
      case 72: /* binaryexp ::= expression MULT expression */
#line 454 "stparserimpl.lemon"
{
        yylhsminor.yy4 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Mult,
                                                        yymsp[-2].minor.yy162, yymsp[0].minor.yy162);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy162->getSourceInfo(), yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy4->setSourceInfo(srcInfo);
}
#line 1906 "stparserimpl.c"
  yymsp[-2].minor.yy4 = yylhsminor.yy4;
        break;
      case 73: /* binaryexp ::= expression DIV expression */
#line 460 "stparserimpl.lemon"
{
        yylhsminor.yy4 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Div,
                                                        yymsp[-2].minor.yy162, yymsp[0].minor.yy162);
}
#line 1915 "stparserimpl.c"
  yymsp[-2].minor.yy4 = yylhsminor.yy4;
        break;
      case 74: /* binaryexp ::= expression MOD expression */
#line 464 "stparserimpl.lemon"
{
        yylhsminor.yy4 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Mod,
                                                        yymsp[-2].minor.yy162, yymsp[0].minor.yy162);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy162->getSourceInfo(), yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy4->setSourceInfo(srcInfo);
}
#line 1926 "stparserimpl.c"
  yymsp[-2].minor.yy4 = yylhsminor.yy4;
        break;
      case 75: /* binaryexp ::= expression EQ expression */
#line 471 "stparserimpl.lemon"
{
        yylhsminor.yy4 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Eq,
                                                        yymsp[-2].minor.yy162, yymsp[0].minor.yy162);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy162->getSourceInfo(), yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy4->setSourceInfo(srcInfo);
}
#line 1937 "stparserimpl.c"
  yymsp[-2].minor.yy4 = yylhsminor.yy4;
        break;
      case 76: /* binaryexp ::= expression NE expression */
#line 478 "stparserimpl.lemon"
{
        yylhsminor.yy4 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Ne,
                                                        yymsp[-2].minor.yy162, yymsp[0].minor.yy162);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy162->getSourceInfo(), yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy4->setSourceInfo(srcInfo);
}
#line 1948 "stparserimpl.c"
  yymsp[-2].minor.yy4 = yylhsminor.yy4;
        break;
      case 77: /* binaryexp ::= expression LE expression */
#line 485 "stparserimpl.lemon"
{
        yylhsminor.yy4 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Le,
                                                        yymsp[-2].minor.yy162, yymsp[0].minor.yy162);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy162->getSourceInfo(), yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy4->setSourceInfo(srcInfo);
}
#line 1959 "stparserimpl.c"
  yymsp[-2].minor.yy4 = yylhsminor.yy4;
        break;
      case 78: /* binaryexp ::= expression LT expression */
#line 492 "stparserimpl.lemon"
{
        yylhsminor.yy4 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Lt,
                                                        yymsp[-2].minor.yy162, yymsp[0].minor.yy162);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy162->getSourceInfo(), yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy4->setSourceInfo(srcInfo);
}
#line 1970 "stparserimpl.c"
  yymsp[-2].minor.yy4 = yylhsminor.yy4;
        break;
      case 79: /* binaryexp ::= expression GE expression */
#line 499 "stparserimpl.lemon"
{
        yylhsminor.yy4 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Ge,
                                                        yymsp[-2].minor.yy162, yymsp[0].minor.yy162);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy162->getSourceInfo(), yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy4->setSourceInfo(srcInfo);
}
#line 1981 "stparserimpl.c"
  yymsp[-2].minor.yy4 = yylhsminor.yy4;
        break;
      case 80: /* binaryexp ::= expression GT expression */
#line 506 "stparserimpl.lemon"
{
        yylhsminor.yy4 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Gt,
                                                        yymsp[-2].minor.yy162, yymsp[0].minor.yy162);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy162->getSourceInfo(), yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy4->setSourceInfo(srcInfo);
}
#line 1992 "stparserimpl.c"
  yymsp[-2].minor.yy4 = yylhsminor.yy4;
        break;
      case 81: /* unaryexp ::= NOT expression */
#line 515 "stparserimpl.lemon"
{
    yylhsminor.yy2 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Not,
                                               yymsp[0].minor.yy162);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy2->setSourceInfo(srcInfo);
}
#line 2003 "stparserimpl.c"
  yymsp[-1].minor.yy2 = yylhsminor.yy2;
        break;
      case 82: /* unaryexp ::= BIT_NOT expression */
#line 521 "stparserimpl.lemon"
{
    yylhsminor.yy2 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::BitNot,
                                               yymsp[0].minor.yy162);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy2->setSourceInfo(srcInfo);
}
#line 2014 "stparserimpl.c"
  yymsp[-1].minor.yy2 = yylhsminor.yy2;
        break;
      case 83: /* unaryexp ::= MINUS expression */
#line 527 "stparserimpl.lemon"
{
    yylhsminor.yy2 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Negate,
                                               yymsp[0].minor.yy162);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy2->setSourceInfo(srcInfo);
}
#line 2025 "stparserimpl.c"
  yymsp[-1].minor.yy2 = yylhsminor.yy2;
        break;
      case 84: /* unaryexp ::= REFERENCE expression */
#line 534 "stparserimpl.lemon"
{
    yylhsminor.yy2 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Reference,
                                               yymsp[0].minor.yy162);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy2->setSourceInfo(srcInfo);
}
#line 2036 "stparserimpl.c"
  yymsp[-1].minor.yy2 = yylhsminor.yy2;
        break;
      case 85: /* function_call_args ::= function_call_args COMMA expression */
#line 543 "stparserimpl.lemon"
{
    auto& list = pParser->getExprList();
    yylhsminor.yy200 = list.add(yymsp[-2].minor.yy200, yymsp[0].minor.yy162);
}
#line 2045 "stparserimpl.c"
  yymsp[-2].minor.yy200 = yylhsminor.yy200;
        break;
      case 86: /* function_call_args ::= expression */
#line 547 "stparserimpl.lemon"
{
    auto& list = pParser->getExprList();
    yylhsminor.yy200 = list.add(yymsp[0].minor.yy162);
}
#line 2054 "stparserimpl.c"
  yymsp[0].minor.yy200 = yylhsminor.yy200;
        break;
      case 87: /* function_call_args ::= */
#line 551 "stparserimpl.lemon"
{
    yymsp[1].minor.yy200= pParser->getExprList().getRangeEmpty();
}
#line 2062 "stparserimpl.c"
        break;
      case 88: /* struct_init ::= type_specifier SCOPE_BEGIN struct_member_init_list SCOPE_END */
#line 558 "stparserimpl.lemon"
{
    yylhsminor.yy100 = pParser->createNode<yal::frontend::STExprStructInit>(yymsp[-3].minor.yy74, yymsp[-1].minor.yy32, *pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-3].minor.yy74->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy100->setSourceInfo(srcInfo);
}
#line 2071 "stparserimpl.c"
  yymsp[-3].minor.yy100 = yylhsminor.yy100;
        break;
      case 89: /* struct_member_init_list ::= */
#line 564 "stparserimpl.lemon"
{yymsp[1].minor.yy32 = pParser->getStructInitList().getRangeEmpty();}
#line 2077 "stparserimpl.c"
        break;
      case 90: /* struct_member_init_list ::= struct_member_init_list COMMA struct_member_init */
#line 565 "stparserimpl.lemon"
{
     auto& list = pParser->getStructInitList();
     yylhsminor.yy32 = list.add(yymsp[-2].minor.yy32, yymsp[0].minor.yy33);
}
#line 2085 "stparserimpl.c"
  yymsp[-2].minor.yy32 = yylhsminor.yy32;
        break;
      case 91: /* struct_member_init_list ::= struct_member_init */
#line 569 "stparserimpl.lemon"
{
    auto& list = pParser->getStructInitList();
    yylhsminor.yy32 = list.add(yymsp[0].minor.yy33);
}
#line 2094 "stparserimpl.c"
  yymsp[0].minor.yy32 = yylhsminor.yy32;
        break;
      case 92: /* struct_member_init ::= identifier COLON expression */
#line 574 "stparserimpl.lemon"
{
    yylhsminor.yy33 = pParser->createNode<yal::frontend::STStructMemberInit>(yymsp[-2].minor.yy57,yymsp[0].minor.yy162);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy57->getSourceInfo(), yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy33->setSourceInfo(srcInfo);
}
#line 2104 "stparserimpl.c"
  yymsp[-2].minor.yy33 = yylhsminor.yy33;
        break;
      case 93: /* literal ::= INTEGER_LITERAL */
      case 94: /* literal ::= INTEGER_LITERAL_I8 */ yytestcase(yyruleno==94);
      case 95: /* literal ::= INTEGER_LITERAL_I16 */ yytestcase(yyruleno==95);
      case 96: /* literal ::= INTEGER_LITERAL_I32 */ yytestcase(yyruleno==96);
      case 97: /* literal ::= INTEGER_LITERAL_I64 */ yytestcase(yyruleno==97);
      case 98: /* literal ::= INTEGER_LITERAL_U8 */ yytestcase(yyruleno==98);
      case 99: /* literal ::= INTEGER_LITERAL_U16 */ yytestcase(yyruleno==99);
      case 100: /* literal ::= INTEGER_LITERAL_U32 */ yytestcase(yyruleno==100);
      case 101: /* literal ::= INTEGER_LITERAL_U64 */ yytestcase(yyruleno==101);
#line 582 "stparserimpl.lemon"
{
        yylhsminor.yy162 = pParser->createNode<yal::frontend::STExprIntegerLiteral>(yymsp[0].minor.yy0);
        auto srcInfo =yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy162->setSourceInfo(srcInfo);
   }
#line 2122 "stparserimpl.c"
  yymsp[0].minor.yy162 = yylhsminor.yy162;
        break;
      case 102: /* literal ::= FLOAT_LITERAL */
      case 103: /* literal ::= FLOAT_LITERAL_32 */ yytestcase(yyruleno==103);
      case 104: /* literal ::= FLOAT_LITERAL_64 */ yytestcase(yyruleno==104);
#line 627 "stparserimpl.lemon"
{
        yylhsminor.yy162 = pParser->createNode<yal::frontend::STExprFloatLiteral>(yymsp[0].minor.yy0);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy162->setSourceInfo(srcInfo);
}
#line 2134 "stparserimpl.c"
  yymsp[0].minor.yy162 = yylhsminor.yy162;
        break;
      case 105: /* literal ::= BOOL_LITERAL */
#line 644 "stparserimpl.lemon"
{
        yylhsminor.yy162 = pParser->createNode<yal::frontend::STExprBoolLiteral>(yymsp[0].minor.yy0.tokenStr);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy162->setSourceInfo(srcInfo);
}
#line 2144 "stparserimpl.c"
  yymsp[0].minor.yy162 = yylhsminor.yy162;
        break;
      default:
      /* (106) module ::= decls END */ yytestcase(yyruleno==106);
      /* (107) type_array ::= type_builtin ARRAY_BEGIN ARRAY_END */ yytestcase(yyruleno==107);
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
#line 2228 "stparserimpl.c"
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
