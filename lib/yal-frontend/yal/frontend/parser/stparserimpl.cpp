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
#define YAL_TOKEN_DEREF                          22
#define YAL_TOKEN_REFERENCE                      23
#define YAL_TOKEN_DOT                            24
#define YAL_TOKEN_END                            25
#define YAL_TOKEN_IDENTIFIER                     26
#define YAL_TOKEN_SELF                           27
#define YAL_TOKEN_TYPE_BOOL                      28
#define YAL_TOKEN_TYPE_INT8                      29
#define YAL_TOKEN_TYPE_UINT8                     30
#define YAL_TOKEN_TYPE_INT16                     31
#define YAL_TOKEN_TYPE_UINT16                    32
#define YAL_TOKEN_TYPE_INT32                     33
#define YAL_TOKEN_TYPE_UINT32                    34
#define YAL_TOKEN_TYPE_INT64                     35
#define YAL_TOKEN_TYPE_UINT64                    36
#define YAL_TOKEN_TYPE_FLOAT                     37
#define YAL_TOKEN_TYPE_DOUBLE                    38
#define YAL_TOKEN_ARRAY_BEGIN                    39
#define YAL_TOKEN_ARRAY_END                      40
#define YAL_TOKEN_MUT                            41
#define YAL_TOKEN_TYPE                           42
#define YAL_TOKEN_ALIAS                          43
#define YAL_TOKEN_SEMI_COLON                     44
#define YAL_TOKEN_STRUCT                         45
#define YAL_TOKEN_SCOPE_BEGIN                    46
#define YAL_TOKEN_SCOPE_END                      47
#define YAL_TOKEN_COMMA                          48
#define YAL_TOKEN_COLON                          49
#define YAL_TOKEN_PAR_BEGIN                      50
#define YAL_TOKEN_PAR_END                        51
#define YAL_TOKEN_FUNCTION                       52
#define YAL_TOKEN_IMPL                           53
#define YAL_TOKEN_RETURN                         54
#define YAL_TOKEN_VAR                            55
#define YAL_TOKEN_LET                            56
#define YAL_TOKEN_INTEGER_LITERAL                57
#define YAL_TOKEN_INTEGER_LITERAL_I8             58
#define YAL_TOKEN_INTEGER_LITERAL_I16            59
#define YAL_TOKEN_INTEGER_LITERAL_I32            60
#define YAL_TOKEN_INTEGER_LITERAL_I64            61
#define YAL_TOKEN_INTEGER_LITERAL_U8             62
#define YAL_TOKEN_INTEGER_LITERAL_U16            63
#define YAL_TOKEN_INTEGER_LITERAL_U32            64
#define YAL_TOKEN_INTEGER_LITERAL_U64            65
#define YAL_TOKEN_FLOAT_LITERAL                  66
#define YAL_TOKEN_FLOAT_LITERAL_32               67
#define YAL_TOKEN_FLOAT_LITERAL_64               68
#define YAL_TOKEN_BOOL_LITERAL                   69
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
#define YYNOCODE 107
#define YYACTIONTYPE unsigned short int
#if INTERFACE
#define YALSTParserTOKENTYPE yal::frontend::TokenInfo
#endif
typedef union {
  int yyinit;
  YALSTParserTOKENTYPE yy0;
  yal::frontend::STDeclModule* yy2;
  yal::frontend::STDeclAlias* yy3;
  yal::frontend::STDeclTypeFunctions* yy4;
  yal::frontend::STExprBinaryOperator* yy12;
  yal::frontend::STQualType* yy37;
  yal::frontend::STDeclParam* yy45;
  yal::frontend::ParseListStmt::Range yy47;
  yal::frontend::STDecl* yy48;
  yal::frontend::STDeclStruct* yy57;
  yal::frontend::STIdentifier* yy67;
  yal::frontend::STStructMember* yy93;
  yal::frontend::STDeclFunction* yy94;
  yal::frontend::ParseListDecl::Range yy99;
  yal::frontend::STExpression* yy104;
  yal::frontend::STExprStructInit* yy116;
  yal::frontend::STStatement* yy121;
  yal::frontend::STStructMemberInit* yy122;
  yal::frontend::ParseListDeclParam::Range yy136;
  yal::frontend::STType* yy154;
  yal::frontend::STExprUnaryOperator* yy155;
  yal::frontend::ParseListExpr::Range yy162;
  yal::frontend::ParseListStructInit::Range yy176;
  yal::frontend::STDeclVar* yy177;
  uint32_t yy185;
  yal::frontend::ParseListStructMember::Range yy188;
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
#define YYNSTATE             116
#define YYNRULE              112
#define YY_MAX_SHIFT         115
#define YY_MIN_SHIFTREDUCE   191
#define YY_MAX_SHIFTREDUCE   302
#define YY_MIN_REDUCE        303
#define YY_MAX_REDUCE        414
#define YY_ERROR_ACTION      415
#define YY_ACCEPT_ACTION     416
#define YY_NO_ACTION         417
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
#define YY_ACTTAB_COUNT (1029)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    14,   12,   26,   25,   24,   15,   16,  341,   13,  115,
 /*    10 */    54,   84,  198,  204,  205,  206,  207,  208,  209,  210,
 /*    20 */   211,  212,  213,  214,   56,  108,   59,  114,   48,   88,
 /*    30 */   114,    1,  331,   85,  343,   31,   58,   34,  340,    7,
 /*    40 */    52,   50,  288,  289,  290,  291,  292,  293,  294,  295,
 /*    50 */   296,  297,  298,  299,  300,   14,   12,  197,   94,  314,
 /*    60 */    15,   16,  314,   13,   32,  416,   84,  198,  204,  205,
 /*    70 */   206,  207,  208,  209,  210,  211,  212,  213,  214,  114,
 /*    80 */    58,  112,   81,  398,  245,  347,  361,   35,   35,  368,
 /*    90 */    31,   35,  217,   41,  217,  197,  327,  288,  289,  290,
 /*   100 */   291,  292,  293,  294,  295,  296,  297,  298,  299,  300,
 /*   110 */   109,  314,  109,   14,   12,  345,   89,   35,   15,   16,
 /*   120 */   336,   13,   35,   90,   84,  198,  204,  205,  206,  207,
 /*   130 */   208,  209,  210,  211,  212,  213,  214,  415,  415,  415,
 /*   140 */   415,  415,  415,   28,   27,   26,   25,   24,   31,   94,
 /*   150 */    36,   17,  111,   54,  259,  288,  289,  290,  291,  292,
 /*   160 */   293,  294,  295,  296,  297,  298,  299,  300,   10,   42,
 /*   170 */    29,   30,  283,   46,  397,   23,   22,   18,   20,   19,
 /*   180 */    21,   28,   27,   26,   25,   24,   42,   29,   30,  221,
 /*   190 */    47,   54,   23,   22,   18,   20,   19,   21,   28,   27,
 /*   200 */    26,   25,   24,   56,  108,  227,   38,   49,   54,  101,
 /*   210 */    58,  243,  203,  342,  204,  205,  206,  207,  208,  209,
 /*   220 */   210,  211,  212,  213,  214,   51,   17,   17,  244,  260,
 /*   230 */   261,   42,   29,   30,  100,  107,  105,   23,   22,   18,
 /*   240 */    20,   19,   21,   28,   27,   26,   25,   24,   42,   29,
 /*   250 */    30,  198,   40,   54,   23,   22,   18,   20,   19,   21,
 /*   260 */    28,   27,   26,   25,   24,  217,  346,   33,  197,  303,
 /*   270 */    54,  305,  307,  241,   55,   37,   57,  306,  225,  110,
 /*   280 */    45,  110,  114,  109,   83,   87,  106,  312,  369,    8,
 /*   290 */    92,  334,  308,  335,   86,  315,    9,  251,  315,  246,
 /*   300 */   242,    3,   91,  352,   60,  365,  366,  364,  226,  367,
 /*   310 */    54,  114,   11,   83,  314,  106,  312,  369,    4,   92,
 /*   320 */    53,    2,    5,    6,  218,   43,   44,   39,  220,  302,
 /*   330 */     3,   93,  352,   60,  365,  366,  364,  113,  367,   42,
 /*   340 */    29,   30,  305,  314,  413,   23,   22,   18,   20,   19,
 /*   350 */    21,   28,   27,   26,   25,   24,  305,  305,  114,  305,
 /*   360 */    83,   54,  106,  312,  369,  305,   92,  305,  305,  305,
 /*   370 */   305,  305,  305,  305,  305,  305,  305,  305,  305,  351,
 /*   380 */    60,  365,  366,  364,  305,  367,  305,   30,  305,  305,
 /*   390 */   314,   23,   22,   18,   20,   19,   21,   28,   27,   26,
 /*   400 */    25,   24,  305,  305,  114,  305,   83,   54,  106,  312,
 /*   410 */   369,  305,  305,  305,  305,  114,  305,   83,  305,  106,
 /*   420 */   312,  369,  305,  305,  305,  305,   68,  365,  366,  364,
 /*   430 */    82,  367,  305,  305,  305,  305,  314,   68,  365,  366,
 /*   440 */   364,   80,  367,  305,  305,  305,  305,  314,  305,  305,
 /*   450 */   305,  305,  305,  114,  305,   83,  305,  106,  312,  369,
 /*   460 */   305,  305,  305,  305,  305,  305,  305,  305,  305,  305,
 /*   470 */   305,  305,  305,  305,  305,   68,  365,  366,  364,   79,
 /*   480 */   367,  305,  305,  305,  305,  314,   23,   22,   18,   20,
 /*   490 */    19,   21,   28,   27,   26,   25,   24,  305,  305,  305,
 /*   500 */   305,  114,   54,   83,  305,  106,  312,  369,  305,  305,
 /*   510 */   305,  305,  114,  305,   83,  305,  106,  312,  369,  305,
 /*   520 */   305,  305,  305,   61,  365,  366,  364,  305,  367,  305,
 /*   530 */   305,  305,  305,  314,   64,  365,  366,  364,  305,  367,
 /*   540 */   305,  305,  305,  305,  314,  305,  305,  305,  305,  114,
 /*   550 */   305,   83,  305,  106,  312,  369,  305,  305,  305,  305,
 /*   560 */   114,  305,   83,  305,  106,  312,  369,  305,  305,  305,
 /*   570 */   305,   65,  365,  366,  364,  305,  367,  305,  305,  305,
 /*   580 */   305,  314,   62,  365,  366,  364,  305,  367,  305,  305,
 /*   590 */   305,  114,  314,   83,  305,  106,  312,  369,  305,  305,
 /*   600 */   305,  305,  114,  305,   83,  305,  106,  312,  369,  305,
 /*   610 */   305,  305,  305,   66,  365,  366,  364,  305,  367,  305,
 /*   620 */   305,  305,  305,  314,   95,  365,  366,  364,  305,  367,
 /*   630 */   305,  305,  305,  305,  314,  305,  114,  305,   83,  305,
 /*   640 */   106,  312,  369,  305,  305,  305,  305,  114,  305,   83,
 /*   650 */   305,  106,  312,  369,  305,  305,  305,  305,   96,  365,
 /*   660 */   366,  364,  305,  367,  305,  305,  305,  305,  314,   97,
 /*   670 */   365,  366,  364,  305,  367,  305,  114,  305,   83,  314,
 /*   680 */   106,  312,  369,  305,  305,  305,  305,  114,  305,   83,
 /*   690 */   305,  106,  312,  369,  305,  305,  305,  305,   98,  365,
 /*   700 */   366,  364,  305,  367,  305,  305,  305,  305,  314,   99,
 /*   710 */   365,  366,  364,  305,  367,  305,  114,  305,   83,  314,
 /*   720 */   106,  312,  369,  305,  305,  305,  305,  114,  305,   83,
 /*   730 */   305,  106,  312,  369,  305,  305,  305,  305,   67,  365,
 /*   740 */   366,  364,  305,  367,  305,  305,  305,  305,  314,   70,
 /*   750 */   365,  366,  364,  305,  367,  305,  114,  305,   83,  314,
 /*   760 */   106,  312,  369,  305,  305,  305,  305,  114,  305,   83,
 /*   770 */   305,  106,  312,  369,  305,  305,  305,  305,   71,  365,
 /*   780 */   366,  364,  305,  367,  305,  305,  305,  305,  314,   72,
 /*   790 */   365,  366,  364,  305,  367,  305,  114,  305,   83,  314,
 /*   800 */   106,  312,  369,  305,  305,  305,  305,  114,  305,   83,
 /*   810 */   305,  106,  312,  369,  305,  305,  305,  305,   73,  365,
 /*   820 */   366,  364,  305,  367,  305,  305,  305,  305,  314,   74,
 /*   830 */   365,  366,  364,  305,  367,  305,  114,  305,   83,  314,
 /*   840 */   106,  312,  369,  305,  305,  305,  305,  114,  305,   83,
 /*   850 */   305,  106,  312,  369,  305,  305,  305,  305,   75,  365,
 /*   860 */   366,  364,  305,  367,  305,  305,  305,  305,  314,  102,
 /*   870 */   365,  366,  364,  305,  367,  305,  114,  305,   83,  314,
 /*   880 */   106,  312,  369,  305,  305,  305,  305,  114,  305,   83,
 /*   890 */   305,  106,  312,  369,  305,  305,  305,  305,  103,  365,
 /*   900 */   366,  364,  305,  367,  305,  305,  305,  305,  314,  104,
 /*   910 */   365,  366,  364,  305,  367,  305,  114,  305,   83,  314,
 /*   920 */   106,  312,  369,  305,  305,  305,  305,  114,  305,   83,
 /*   930 */   305,  106,  312,  369,  305,  305,  305,  305,   77,  365,
 /*   940 */   366,  364,  305,  367,  305,  305,  305,  305,  314,   78,
 /*   950 */   365,  366,  364,  305,  367,  305,  114,  305,   83,  314,
 /*   960 */   106,  312,  369,  305,  305,  305,  305,  114,  305,   83,
 /*   970 */   305,  106,  312,  369,  305,  305,  305,  305,   69,  365,
 /*   980 */   366,  364,  305,  367,  305,  305,  305,  305,  314,   76,
 /*   990 */   365,  366,  364,  305,  367,  305,  114,  305,   83,  314,
 /*  1000 */   106,  312,  369,  305,  305,  305,  305,  305,  305,  305,
 /*  1010 */   305,  305,  305,  305,  305,  305,  305,  305,   63,  365,
 /*  1020 */   366,  364,  305,  367,  305,  305,  305,  305,  314,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    15,   16,   16,   17,   18,   20,   21,   83,   23,   25,
 /*    10 */    24,   26,   27,   28,   29,   30,   31,   32,   33,   34,
 /*    20 */    35,   36,   37,   38,   76,   77,   42,   73,  104,   75,
 /*    30 */    73,   46,   75,   85,   86,   50,   52,   53,   83,   54,
 /*    40 */    55,   56,   57,   58,   59,   60,   61,   62,   63,   64,
 /*    50 */    65,   66,   67,   68,   69,   15,   16,   26,   77,  105,
 /*    60 */    20,   21,  105,   23,   71,   72,   26,   27,   28,   29,
 /*    70 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   73,
 /*    80 */    52,   75,  101,  102,   44,   74,   74,   76,   76,   74,
 /*    90 */    50,   76,   23,   49,   23,   26,   27,   57,   58,   59,
 /*   100 */    60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
 /*   110 */    41,  105,   41,   15,   16,   74,   91,   76,   20,   21,
 /*   120 */    74,   23,   76,   91,   26,   27,   28,   29,   30,   31,
 /*   130 */    32,   33,   34,   35,   36,   37,   38,    8,    9,   10,
 /*   140 */    11,   12,   13,   14,   15,   16,   17,   18,   50,   77,
 /*   150 */    43,   48,   45,   24,   51,   57,   58,   59,   60,   61,
 /*   160 */    62,   63,   64,   65,   66,   67,   68,   69,    1,    2,
 /*   170 */     3,    4,   47,   48,  102,    8,    9,   10,   11,   12,
 /*   180 */    13,   14,   15,   16,   17,   18,    2,    3,    4,   47,
 /*   190 */    48,   24,    8,    9,   10,   11,   12,   13,   14,   15,
 /*   200 */    16,   17,   18,   76,   77,   47,   46,   77,   24,   49,
 /*   210 */    52,   44,   26,   86,   28,   29,   30,   31,   32,   33,
 /*   220 */    34,   35,   36,   37,   38,   77,   48,   48,   44,   51,
 /*   230 */    51,    2,    3,    4,   77,   84,   77,    8,    9,   10,
 /*   240 */    11,   12,   13,   14,   15,   16,   17,   18,    2,    3,
 /*   250 */     4,   27,   49,   24,    8,    9,   10,   11,   12,   13,
 /*   260 */    14,   15,   16,   17,   18,   23,   78,   50,   26,   80,
 /*   270 */    24,   82,   83,   44,   87,   48,   77,   88,   51,   77,
 /*   280 */    46,   77,   73,   41,   75,   77,   77,   78,   79,    1,
 /*   290 */    81,   89,  103,   89,   90,   46,    1,   51,   49,   47,
 /*   300 */    44,   92,   93,   94,   95,   96,   97,   98,   47,  100,
 /*   310 */    24,   73,   49,   75,  105,   77,   78,   79,   50,   81,
 /*   320 */    49,   46,   50,   50,   23,   49,   49,   46,   44,   40,
 /*   330 */    92,   93,   94,   95,   96,   97,   98,   39,  100,    2,
 /*   340 */     3,    4,  106,  105,    0,    8,    9,   10,   11,   12,
 /*   350 */    13,   14,   15,   16,   17,   18,  106,  106,   73,  106,
 /*   360 */    75,   24,   77,   78,   79,  106,   81,  106,  106,  106,
 /*   370 */   106,  106,  106,  106,  106,  106,  106,  106,  106,   94,
 /*   380 */    95,   96,   97,   98,  106,  100,  106,    4,  106,  106,
 /*   390 */   105,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*   400 */    17,   18,  106,  106,   73,  106,   75,   24,   77,   78,
 /*   410 */    79,  106,  106,  106,  106,   73,  106,   75,  106,   77,
 /*   420 */    78,   79,  106,  106,  106,  106,   95,   96,   97,   98,
 /*   430 */    99,  100,  106,  106,  106,  106,  105,   95,   96,   97,
 /*   440 */    98,   99,  100,  106,  106,  106,  106,  105,  106,  106,
 /*   450 */   106,  106,  106,   73,  106,   75,  106,   77,   78,   79,
 /*   460 */   106,  106,  106,  106,  106,  106,  106,  106,  106,  106,
 /*   470 */   106,  106,  106,  106,  106,   95,   96,   97,   98,   99,
 /*   480 */   100,  106,  106,  106,  106,  105,    8,    9,   10,   11,
 /*   490 */    12,   13,   14,   15,   16,   17,   18,  106,  106,  106,
 /*   500 */   106,   73,   24,   75,  106,   77,   78,   79,  106,  106,
 /*   510 */   106,  106,   73,  106,   75,  106,   77,   78,   79,  106,
 /*   520 */   106,  106,  106,   95,   96,   97,   98,  106,  100,  106,
 /*   530 */   106,  106,  106,  105,   95,   96,   97,   98,  106,  100,
 /*   540 */   106,  106,  106,  106,  105,  106,  106,  106,  106,   73,
 /*   550 */   106,   75,  106,   77,   78,   79,  106,  106,  106,  106,
 /*   560 */    73,  106,   75,  106,   77,   78,   79,  106,  106,  106,
 /*   570 */   106,   95,   96,   97,   98,  106,  100,  106,  106,  106,
 /*   580 */   106,  105,   95,   96,   97,   98,  106,  100,  106,  106,
 /*   590 */   106,   73,  105,   75,  106,   77,   78,   79,  106,  106,
 /*   600 */   106,  106,   73,  106,   75,  106,   77,   78,   79,  106,
 /*   610 */   106,  106,  106,   95,   96,   97,   98,  106,  100,  106,
 /*   620 */   106,  106,  106,  105,   95,   96,   97,   98,  106,  100,
 /*   630 */   106,  106,  106,  106,  105,  106,   73,  106,   75,  106,
 /*   640 */    77,   78,   79,  106,  106,  106,  106,   73,  106,   75,
 /*   650 */   106,   77,   78,   79,  106,  106,  106,  106,   95,   96,
 /*   660 */    97,   98,  106,  100,  106,  106,  106,  106,  105,   95,
 /*   670 */    96,   97,   98,  106,  100,  106,   73,  106,   75,  105,
 /*   680 */    77,   78,   79,  106,  106,  106,  106,   73,  106,   75,
 /*   690 */   106,   77,   78,   79,  106,  106,  106,  106,   95,   96,
 /*   700 */    97,   98,  106,  100,  106,  106,  106,  106,  105,   95,
 /*   710 */    96,   97,   98,  106,  100,  106,   73,  106,   75,  105,
 /*   720 */    77,   78,   79,  106,  106,  106,  106,   73,  106,   75,
 /*   730 */   106,   77,   78,   79,  106,  106,  106,  106,   95,   96,
 /*   740 */    97,   98,  106,  100,  106,  106,  106,  106,  105,   95,
 /*   750 */    96,   97,   98,  106,  100,  106,   73,  106,   75,  105,
 /*   760 */    77,   78,   79,  106,  106,  106,  106,   73,  106,   75,
 /*   770 */   106,   77,   78,   79,  106,  106,  106,  106,   95,   96,
 /*   780 */    97,   98,  106,  100,  106,  106,  106,  106,  105,   95,
 /*   790 */    96,   97,   98,  106,  100,  106,   73,  106,   75,  105,
 /*   800 */    77,   78,   79,  106,  106,  106,  106,   73,  106,   75,
 /*   810 */   106,   77,   78,   79,  106,  106,  106,  106,   95,   96,
 /*   820 */    97,   98,  106,  100,  106,  106,  106,  106,  105,   95,
 /*   830 */    96,   97,   98,  106,  100,  106,   73,  106,   75,  105,
 /*   840 */    77,   78,   79,  106,  106,  106,  106,   73,  106,   75,
 /*   850 */   106,   77,   78,   79,  106,  106,  106,  106,   95,   96,
 /*   860 */    97,   98,  106,  100,  106,  106,  106,  106,  105,   95,
 /*   870 */    96,   97,   98,  106,  100,  106,   73,  106,   75,  105,
 /*   880 */    77,   78,   79,  106,  106,  106,  106,   73,  106,   75,
 /*   890 */   106,   77,   78,   79,  106,  106,  106,  106,   95,   96,
 /*   900 */    97,   98,  106,  100,  106,  106,  106,  106,  105,   95,
 /*   910 */    96,   97,   98,  106,  100,  106,   73,  106,   75,  105,
 /*   920 */    77,   78,   79,  106,  106,  106,  106,   73,  106,   75,
 /*   930 */   106,   77,   78,   79,  106,  106,  106,  106,   95,   96,
 /*   940 */    97,   98,  106,  100,  106,  106,  106,  106,  105,   95,
 /*   950 */    96,   97,   98,  106,  100,  106,   73,  106,   75,  105,
 /*   960 */    77,   78,   79,  106,  106,  106,  106,   73,  106,   75,
 /*   970 */   106,   77,   78,   79,  106,  106,  106,  106,   95,   96,
 /*   980 */    97,   98,  106,  100,  106,  106,  106,  106,  105,   95,
 /*   990 */    96,   97,   98,  106,  100,  106,   73,  106,   75,  105,
 /*  1000 */    77,   78,   79,  106,  106,  106,  106,  106,  106,  106,
 /*  1010 */   106,  106,  106,  106,  106,  106,  106,  106,   95,   96,
 /*  1020 */    97,   98,  106,  100,  106,  106,  106,  106,  105,
};
#define YY_SHIFT_USE_DFLT (1029)
#define YY_SHIFT_COUNT    (115)
#define YY_SHIFT_MIN      (-16)
#define YY_SHIFT_MAX      (478)
static const short yy_shift_ofst[] = {
 /*     0 */  1029,  -15,  -15,  -15,   98,   98,   98,   40,   98,   98,
 /*    10 */    98,   98,   98,   98,   98,   98,   98,   98,   98,   98,
 /*    20 */    98,   98,   98,   98,   98,   98,   98,   98,   98,   98,
 /*    30 */    98,   98,  -16,   69,  186,  186,  186,  242,   31,   31,
 /*    40 */    71,   71,   71,   71,   71,   28,   31,   31,  158,   44,
 /*    50 */    31,   44,   31,   31,   31,  203,  224,  217,   31,   31,
 /*    60 */   167,  184,  229,  246,  337,  337,  337,  337,  337,  383,
 /*    70 */   129,  129,  129,  129,  129,  129,  478,  -14,  -14,  103,
 /*    80 */   178,  125,  179,  160,  249,  227,  142,  107,  234,  288,
 /*    90 */   295,  252,  256,  261,  263,  286,  286,  286,  286,  286,
 /*   100 */   268,  271,  286,  286,  286,  272,  273,  275,  276,  301,
 /*   110 */   277,  281,  284,  289,  298,  344,
};
#define YY_REDUCE_USE_DFLT (-77)
#define YY_REDUCE_COUNT (59)
#define YY_REDUCE_MIN   (-76)
#define YY_REDUCE_MAX   (923)
static const short yy_reduce_ofst[] = {
 /*     0 */    -7,  209,  238,  285,  331,  342,  380,  428,  439,  476,
 /*    10 */   487,  518,  529,  563,  574,  603,  614,  643,  654,  683,
 /*    20 */   694,  723,  734,  763,  774,  803,  814,  843,  854,  883,
 /*    30 */   894,  923,  189,  -52,  -46,  -43,    6,  127,  -19,  204,
 /*    40 */    11,   12,   15,   41,   46,  -76,   72,  202,  -45,   25,
 /*    50 */   130,   32,  148,  157,  159,  151,  188,  187,  199,  208,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   304,  349,  349,  350,  394,  394,  394,  415,  415,  415,
 /*    10 */   415,  415,  415,  415,  415,  415,  415,  415,  415,  415,
 /*    20 */   415,  415,  415,  415,  415,  415,  415,  415,  415,  415,
 /*    30 */   415,  415,  415,  344,  415,  415,  415,  327,  396,  415,
 /*    40 */   327,  327,  327,  327,  327,  415,  415,  415,  415,  362,
 /*    50 */   415,  362,  415,  415,  415,  348,  415,  415,  415,  415,
 /*    60 */   415,  415,  415,  415,  360,  359,  399,  392,  393,  375,
 /*    70 */   386,  385,  384,  383,  382,  381,  374,  377,  376,  415,
 /*    80 */   415,  415,  415,  415,  309,  415,  415,  415,  415,  415,
 /*    90 */   415,  415,  415,  415,  415,  391,  390,  389,  388,  387,
 /*   100 */   415,  415,  380,  379,  378,  370,  311,  415,  415,  328,
 /*   110 */   415,  415,  415,  415,  313,  415,
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
  "BIT_NOT",       "NOT",           "DEREF",         "REFERENCE",   
  "DOT",           "END",           "IDENTIFIER",    "SELF",        
  "TYPE_BOOL",     "TYPE_INT8",     "TYPE_UINT8",    "TYPE_INT16",  
  "TYPE_UINT16",   "TYPE_INT32",    "TYPE_UINT32",   "TYPE_INT64",  
  "TYPE_UINT64",   "TYPE_FLOAT",    "TYPE_DOUBLE",   "ARRAY_BEGIN", 
  "ARRAY_END",     "MUT",           "TYPE",          "ALIAS",       
  "SEMI_COLON",    "STRUCT",        "SCOPE_BEGIN",   "SCOPE_END",   
  "COMMA",         "COLON",         "PAR_BEGIN",     "PAR_END",     
  "FUNCTION",      "IMPL",          "RETURN",        "VAR",         
  "LET",           "INTEGER_LITERAL",  "INTEGER_LITERAL_I8",  "INTEGER_LITERAL_I16",
  "INTEGER_LITERAL_I32",  "INTEGER_LITERAL_I64",  "INTEGER_LITERAL_U8",  "INTEGER_LITERAL_U16",
  "INTEGER_LITERAL_U32",  "INTEGER_LITERAL_U64",  "FLOAT_LITERAL",  "FLOAT_LITERAL_32",
  "FLOAT_LITERAL_64",  "BOOL_LITERAL",  "error",         "decls",       
  "module",        "type_builtin",  "type_qualified",  "type_specifier",
  "qualifier",     "identifier",    "self",          "identifier_or_self",
  "decl",          "decl_var",      "decl_alias",    "decl_function",
  "function_return_decl",  "function_args_decl",  "function_arg_decl",  "function_param_list",
  "decl_struct",   "struct_decl_var",  "struct_decl_vars",  "var_type_spec",
  "statement_list",  "statement_list_or_empty",  "statement",     "expression",  
  "unaryexp",      "binaryexp",     "literal",       "function_call_args",
  "struct_init",   "struct_member_init_list",  "struct_member_init",  "decl_type_functions_scope",
  "decl_type_function_list",  "type_array",  
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "decls ::= decls decl",
 /*   1 */ "decls ::=",
 /*   2 */ "decl ::= decl_alias",
 /*   3 */ "decl ::= decl_struct",
 /*   4 */ "decl ::= decl_function",
 /*   5 */ "decl ::= decl_type_functions_scope",
 /*   6 */ "identifier ::= IDENTIFIER",
 /*   7 */ "self ::= SELF",
 /*   8 */ "identifier_or_self ::= identifier",
 /*   9 */ "identifier_or_self ::= self",
 /*  10 */ "type_specifier ::= type_builtin",
 /*  11 */ "type_specifier ::= type_array",
 /*  12 */ "type_specifier ::= IDENTIFIER",
 /*  13 */ "type_builtin ::= TYPE_BOOL",
 /*  14 */ "type_builtin ::= TYPE_INT8",
 /*  15 */ "type_builtin ::= TYPE_UINT8",
 /*  16 */ "type_builtin ::= TYPE_INT16",
 /*  17 */ "type_builtin ::= TYPE_UINT16",
 /*  18 */ "type_builtin ::= TYPE_INT32",
 /*  19 */ "type_builtin ::= TYPE_UINT32",
 /*  20 */ "type_builtin ::= TYPE_INT64",
 /*  21 */ "type_builtin ::= TYPE_UINT64",
 /*  22 */ "type_builtin ::= TYPE_FLOAT",
 /*  23 */ "type_builtin ::= TYPE_DOUBLE",
 /*  24 */ "qualifier ::=",
 /*  25 */ "qualifier ::= MUT",
 /*  26 */ "qualifier ::= REFERENCE",
 /*  27 */ "qualifier ::= MUT REFERENCE",
 /*  28 */ "type_qualified ::= qualifier type_specifier",
 /*  29 */ "decl_alias ::= TYPE identifier ALIAS type_specifier SEMI_COLON",
 /*  30 */ "decl_struct ::= TYPE identifier STRUCT SCOPE_BEGIN struct_decl_vars SCOPE_END",
 /*  31 */ "struct_decl_vars ::= struct_decl_vars COMMA struct_decl_var",
 /*  32 */ "struct_decl_vars ::= struct_decl_var",
 /*  33 */ "struct_decl_var ::= identifier COLON type_qualified",
 /*  34 */ "function_param_list ::= PAR_BEGIN function_args_decl PAR_END",
 /*  35 */ "decl_function ::= FUNCTION identifier function_param_list function_return_decl SCOPE_BEGIN statement_list_or_empty SCOPE_END",
 /*  36 */ "decl_type_functions_scope ::= IMPL type_specifier SCOPE_BEGIN decl_type_function_list SCOPE_END",
 /*  37 */ "decl_type_function_list ::= decl_type_function_list decl_function",
 /*  38 */ "decl_type_function_list ::= decl_function",
 /*  39 */ "function_args_decl ::= function_args_decl COMMA function_arg_decl",
 /*  40 */ "function_args_decl ::= function_arg_decl",
 /*  41 */ "function_args_decl ::=",
 /*  42 */ "function_arg_decl ::= identifier COLON type_qualified",
 /*  43 */ "function_arg_decl ::= qualifier self",
 /*  44 */ "function_return_decl ::= COLON type_qualified",
 /*  45 */ "function_return_decl ::=",
 /*  46 */ "statement_list_or_empty ::=",
 /*  47 */ "statement_list_or_empty ::= statement_list",
 /*  48 */ "statement_list ::= statement_list statement",
 /*  49 */ "statement_list ::= statement",
 /*  50 */ "statement ::= expression ASSIGN expression SEMI_COLON",
 /*  51 */ "statement ::= decl_var SEMI_COLON",
 /*  52 */ "statement ::= expression SEMI_COLON",
 /*  53 */ "statement ::= RETURN expression SEMI_COLON",
 /*  54 */ "statement ::= RETURN SEMI_COLON",
 /*  55 */ "statement ::= SCOPE_BEGIN statement_list_or_empty SCOPE_END",
 /*  56 */ "decl_var ::= VAR identifier var_type_spec ASSIGN expression",
 /*  57 */ "decl_var ::= LET identifier var_type_spec ASSIGN expression",
 /*  58 */ "var_type_spec ::= COLON type_qualified",
 /*  59 */ "var_type_spec ::=",
 /*  60 */ "expression ::= PAR_BEGIN expression PAR_END",
 /*  61 */ "expression ::= literal",
 /*  62 */ "expression ::= unaryexp",
 /*  63 */ "expression ::= binaryexp",
 /*  64 */ "expression ::= struct_init",
 /*  65 */ "expression ::= expression AS type_qualified",
 /*  66 */ "expression ::= identifier_or_self",
 /*  67 */ "expression ::= expression DOT identifier",
 /*  68 */ "expression ::= identifier PAR_BEGIN function_call_args PAR_END",
 /*  69 */ "expression ::= expression DOT identifier PAR_BEGIN function_call_args PAR_END",
 /*  70 */ "expression ::= type_specifier COLON COLON identifier PAR_BEGIN function_call_args PAR_END",
 /*  71 */ "binaryexp ::= expression AND expression",
 /*  72 */ "binaryexp ::= expression OR expression",
 /*  73 */ "binaryexp ::= expression PLUS expression",
 /*  74 */ "binaryexp ::= expression MINUS expression",
 /*  75 */ "binaryexp ::= expression MULT expression",
 /*  76 */ "binaryexp ::= expression DIV expression",
 /*  77 */ "binaryexp ::= expression MOD expression",
 /*  78 */ "binaryexp ::= expression EQ expression",
 /*  79 */ "binaryexp ::= expression NE expression",
 /*  80 */ "binaryexp ::= expression LE expression",
 /*  81 */ "binaryexp ::= expression LT expression",
 /*  82 */ "binaryexp ::= expression GE expression",
 /*  83 */ "binaryexp ::= expression GT expression",
 /*  84 */ "unaryexp ::= NOT expression",
 /*  85 */ "unaryexp ::= BIT_NOT expression",
 /*  86 */ "unaryexp ::= MINUS expression",
 /*  87 */ "unaryexp ::= REFERENCE expression",
 /*  88 */ "unaryexp ::= MULT expression",
 /*  89 */ "function_call_args ::= function_call_args COMMA expression",
 /*  90 */ "function_call_args ::= expression",
 /*  91 */ "function_call_args ::=",
 /*  92 */ "struct_init ::= type_specifier SCOPE_BEGIN struct_member_init_list SCOPE_END",
 /*  93 */ "struct_member_init_list ::=",
 /*  94 */ "struct_member_init_list ::= struct_member_init_list COMMA struct_member_init",
 /*  95 */ "struct_member_init_list ::= struct_member_init",
 /*  96 */ "struct_member_init ::= identifier COLON expression",
 /*  97 */ "literal ::= INTEGER_LITERAL",
 /*  98 */ "literal ::= INTEGER_LITERAL_I8",
 /*  99 */ "literal ::= INTEGER_LITERAL_I16",
 /* 100 */ "literal ::= INTEGER_LITERAL_I32",
 /* 101 */ "literal ::= INTEGER_LITERAL_I64",
 /* 102 */ "literal ::= INTEGER_LITERAL_U8",
 /* 103 */ "literal ::= INTEGER_LITERAL_U16",
 /* 104 */ "literal ::= INTEGER_LITERAL_U32",
 /* 105 */ "literal ::= INTEGER_LITERAL_U64",
 /* 106 */ "literal ::= FLOAT_LITERAL",
 /* 107 */ "literal ::= FLOAT_LITERAL_32",
 /* 108 */ "literal ::= FLOAT_LITERAL_64",
 /* 109 */ "literal ::= BOOL_LITERAL",
 /* 110 */ "module ::= decls END",
 /* 111 */ "type_array ::= type_builtin ARRAY_BEGIN ARRAY_END",
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
  { 71, -2 },
  { 71, 0 },
  { 80, -1 },
  { 80, -1 },
  { 80, -1 },
  { 80, -1 },
  { 77, -1 },
  { 78, -1 },
  { 79, -1 },
  { 79, -1 },
  { 75, -1 },
  { 75, -1 },
  { 75, -1 },
  { 73, -1 },
  { 73, -1 },
  { 73, -1 },
  { 73, -1 },
  { 73, -1 },
  { 73, -1 },
  { 73, -1 },
  { 73, -1 },
  { 73, -1 },
  { 73, -1 },
  { 73, -1 },
  { 76, 0 },
  { 76, -1 },
  { 76, -1 },
  { 76, -2 },
  { 74, -2 },
  { 82, -5 },
  { 88, -6 },
  { 90, -3 },
  { 90, -1 },
  { 89, -3 },
  { 87, -3 },
  { 83, -7 },
  { 103, -5 },
  { 104, -2 },
  { 104, -1 },
  { 85, -3 },
  { 85, -1 },
  { 85, 0 },
  { 86, -3 },
  { 86, -2 },
  { 84, -2 },
  { 84, 0 },
  { 93, 0 },
  { 93, -1 },
  { 92, -2 },
  { 92, -1 },
  { 94, -4 },
  { 94, -2 },
  { 94, -2 },
  { 94, -3 },
  { 94, -2 },
  { 94, -3 },
  { 81, -5 },
  { 81, -5 },
  { 91, -2 },
  { 91, 0 },
  { 95, -3 },
  { 95, -1 },
  { 95, -1 },
  { 95, -1 },
  { 95, -1 },
  { 95, -3 },
  { 95, -1 },
  { 95, -3 },
  { 95, -4 },
  { 95, -6 },
  { 95, -7 },
  { 97, -3 },
  { 97, -3 },
  { 97, -3 },
  { 97, -3 },
  { 97, -3 },
  { 97, -3 },
  { 97, -3 },
  { 97, -3 },
  { 97, -3 },
  { 97, -3 },
  { 97, -3 },
  { 97, -3 },
  { 97, -3 },
  { 96, -2 },
  { 96, -2 },
  { 96, -2 },
  { 96, -2 },
  { 96, -2 },
  { 99, -3 },
  { 99, -1 },
  { 99, 0 },
  { 100, -4 },
  { 101, 0 },
  { 101, -3 },
  { 101, -1 },
  { 102, -3 },
  { 98, -1 },
  { 98, -1 },
  { 98, -1 },
  { 98, -1 },
  { 98, -1 },
  { 98, -1 },
  { 98, -1 },
  { 98, -1 },
  { 98, -1 },
  { 98, -1 },
  { 98, -1 },
  { 98, -1 },
  { 98, -1 },
  { 72, -2 },
  { 105, -3 },
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
      case 0: /* decls ::= decls decl */
#line 124 "stparserimpl.lemon"
{
    yylhsminor.yy2 =yymsp[-1].minor.yy2; yymsp[-1].minor.yy2->addDecl(yymsp[0].minor.yy48);
}
#line 1351 "stparserimpl.c"
  yymsp[-1].minor.yy2 = yylhsminor.yy2;
        break;
      case 1: /* decls ::= */
#line 127 "stparserimpl.lemon"
{
        yymsp[1].minor.yy2 =  pParser->getDeclModule();
}
#line 1359 "stparserimpl.c"
        break;
      case 2: /* decl ::= decl_alias */
#line 131 "stparserimpl.lemon"
{
    yylhsminor.yy48 = yymsp[0].minor.yy3;
}
#line 1366 "stparserimpl.c"
  yymsp[0].minor.yy48 = yylhsminor.yy48;
        break;
      case 3: /* decl ::= decl_struct */
#line 134 "stparserimpl.lemon"
{
    yylhsminor.yy48 = yymsp[0].minor.yy57;
}
#line 1374 "stparserimpl.c"
  yymsp[0].minor.yy48 = yylhsminor.yy48;
        break;
      case 4: /* decl ::= decl_function */
#line 137 "stparserimpl.lemon"
{
    yylhsminor.yy48 = yymsp[0].minor.yy94;
}
#line 1382 "stparserimpl.c"
  yymsp[0].minor.yy48 = yylhsminor.yy48;
        break;
      case 5: /* decl ::= decl_type_functions_scope */
#line 140 "stparserimpl.lemon"
{
    yylhsminor.yy48 = yymsp[0].minor.yy4;
}
#line 1390 "stparserimpl.c"
  yymsp[0].minor.yy48 = yylhsminor.yy48;
        break;
      case 6: /* identifier ::= IDENTIFIER */
      case 7: /* self ::= SELF */ yytestcase(yyruleno==7);
#line 146 "stparserimpl.lemon"
{
        yylhsminor.yy67 = pParser->createNode<yal::frontend::STIdentifier>(yymsp[0].minor.yy0.tokenStr);
        auto srcInfo = CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy67->setSourceInfo(srcInfo);
}
#line 1401 "stparserimpl.c"
  yymsp[0].minor.yy67 = yylhsminor.yy67;
        break;
      case 8: /* identifier_or_self ::= identifier */
      case 9: /* identifier_or_self ::= self */ yytestcase(yyruleno==9);
#line 158 "stparserimpl.lemon"
{yylhsminor.yy67=yymsp[0].minor.yy67;}
#line 1408 "stparserimpl.c"
  yymsp[0].minor.yy67 = yylhsminor.yy67;
        break;
      case 10: /* type_specifier ::= type_builtin */
#line 161 "stparserimpl.lemon"
{
        yylhsminor.yy154=yymsp[0].minor.yy154;
}
#line 1416 "stparserimpl.c"
  yymsp[0].minor.yy154 = yylhsminor.yy154;
        break;
      case 11: /* type_specifier ::= type_array */
#line 165 "stparserimpl.lemon"
{yymsp[0].minor.yy154 = nullptr;}
#line 1422 "stparserimpl.c"
        break;
      case 12: /* type_specifier ::= IDENTIFIER */
#line 166 "stparserimpl.lemon"
{
    yylhsminor.yy154 = pParser->createNode<yal::frontend::STType>(yymsp[0].minor.yy0.tokenStr);
    yylhsminor.yy154->setSourceInfo(yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0,yymsp[0].minor.yy0, pParser->getSourceHandle()));
}
#line 1430 "stparserimpl.c"
  yymsp[0].minor.yy154 = yylhsminor.yy154;
        break;
      case 13: /* type_builtin ::= TYPE_BOOL */
#line 171 "stparserimpl.lemon"
{
    yylhsminor.yy154 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Bool);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy154->setSourceInfo(srcInfo);
    }
#line 1440 "stparserimpl.c"
  yymsp[0].minor.yy154 = yylhsminor.yy154;
        break;
      case 14: /* type_builtin ::= TYPE_INT8 */
#line 176 "stparserimpl.lemon"
{
        yylhsminor.yy154 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int8);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy154->setSourceInfo(srcInfo);
    }
#line 1450 "stparserimpl.c"
  yymsp[0].minor.yy154 = yylhsminor.yy154;
        break;
      case 15: /* type_builtin ::= TYPE_UINT8 */
#line 181 "stparserimpl.lemon"
{
        yylhsminor.yy154 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt8);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy154->setSourceInfo(srcInfo);
    }
#line 1460 "stparserimpl.c"
  yymsp[0].minor.yy154 = yylhsminor.yy154;
        break;
      case 16: /* type_builtin ::= TYPE_INT16 */
#line 186 "stparserimpl.lemon"
{
        yylhsminor.yy154 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int16);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy154->setSourceInfo(srcInfo);
    }
#line 1470 "stparserimpl.c"
  yymsp[0].minor.yy154 = yylhsminor.yy154;
        break;
      case 17: /* type_builtin ::= TYPE_UINT16 */
#line 191 "stparserimpl.lemon"
{
        yylhsminor.yy154 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt16);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy154->setSourceInfo(srcInfo);
    }
#line 1480 "stparserimpl.c"
  yymsp[0].minor.yy154 = yylhsminor.yy154;
        break;
      case 18: /* type_builtin ::= TYPE_INT32 */
#line 196 "stparserimpl.lemon"
{
        yylhsminor.yy154 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int32);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy154->setSourceInfo(srcInfo);
    }
#line 1490 "stparserimpl.c"
  yymsp[0].minor.yy154 = yylhsminor.yy154;
        break;
      case 19: /* type_builtin ::= TYPE_UINT32 */
#line 201 "stparserimpl.lemon"
{
        yylhsminor.yy154 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt32);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy154->setSourceInfo(srcInfo);
    }
#line 1500 "stparserimpl.c"
  yymsp[0].minor.yy154 = yylhsminor.yy154;
        break;
      case 20: /* type_builtin ::= TYPE_INT64 */
#line 206 "stparserimpl.lemon"
{
        yylhsminor.yy154 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int64);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy154->setSourceInfo(srcInfo);
    }
#line 1510 "stparserimpl.c"
  yymsp[0].minor.yy154 = yylhsminor.yy154;
        break;
      case 21: /* type_builtin ::= TYPE_UINT64 */
#line 211 "stparserimpl.lemon"
{
        yylhsminor.yy154 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt64);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy154->setSourceInfo(srcInfo);
    }
#line 1520 "stparserimpl.c"
  yymsp[0].minor.yy154 = yylhsminor.yy154;
        break;
      case 22: /* type_builtin ::= TYPE_FLOAT */
#line 216 "stparserimpl.lemon"
{
        yylhsminor.yy154 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Float32);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy154->setSourceInfo(srcInfo);
    }
#line 1530 "stparserimpl.c"
  yymsp[0].minor.yy154 = yylhsminor.yy154;
        break;
      case 23: /* type_builtin ::= TYPE_DOUBLE */
#line 221 "stparserimpl.lemon"
{
        yylhsminor.yy154 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Float64);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy154->setSourceInfo(srcInfo);
    }
#line 1540 "stparserimpl.c"
  yymsp[0].minor.yy154 = yylhsminor.yy154;
        break;
      case 24: /* qualifier ::= */
#line 229 "stparserimpl.lemon"
{yymsp[1].minor.yy185 = 0;}
#line 1546 "stparserimpl.c"
        break;
      case 25: /* qualifier ::= MUT */
#line 230 "stparserimpl.lemon"
{yymsp[0].minor.yy185 = yal::frontend::STQualType::Qualifiers::kQualMutable;}
#line 1551 "stparserimpl.c"
        break;
      case 26: /* qualifier ::= REFERENCE */
#line 231 "stparserimpl.lemon"
{yymsp[0].minor.yy185 = yal::frontend::STQualType::Qualifiers::kQualReference;}
#line 1556 "stparserimpl.c"
        break;
      case 27: /* qualifier ::= MUT REFERENCE */
#line 232 "stparserimpl.lemon"
{
        yymsp[-1].minor.yy185 = yal::frontend::STQualType::Qualifiers::kQualReference | yal::frontend::STQualType::Qualifiers::kQualMutable;
 }
#line 1563 "stparserimpl.c"
        break;
      case 28: /* type_qualified ::= qualifier type_specifier */
#line 235 "stparserimpl.lemon"
{
    yylhsminor.yy37 = pParser->createNode<yal::frontend::STQualType>(yymsp[0].minor.yy154, yymsp[-1].minor.yy185);
}
#line 1570 "stparserimpl.c"
  yymsp[-1].minor.yy37 = yylhsminor.yy37;
        break;
      case 29: /* decl_alias ::= TYPE identifier ALIAS type_specifier SEMI_COLON */
#line 240 "stparserimpl.lemon"
{
    yylhsminor.yy3 = pParser->createNode<yal::frontend::STDeclAlias>(yymsp[-3].minor.yy67, yymsp[-1].minor.yy154);
    auto srcInfo = CreateSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy3->setSourceInfo(srcInfo);
}
#line 1580 "stparserimpl.c"
  yymsp[-4].minor.yy3 = yylhsminor.yy3;
        break;
      case 30: /* decl_struct ::= TYPE identifier STRUCT SCOPE_BEGIN struct_decl_vars SCOPE_END */
#line 248 "stparserimpl.lemon"
{
    yylhsminor.yy57 = pParser->createNode<yal::frontend::STDeclStruct>(yymsp[-4].minor.yy67, yymsp[-1].minor.yy188, *pParser);
    auto srcInfo = CreateSourceInfo(yymsp[-5].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy57->setSourceInfo(srcInfo);
}
#line 1590 "stparserimpl.c"
  yymsp[-5].minor.yy57 = yylhsminor.yy57;
        break;
      case 31: /* struct_decl_vars ::= struct_decl_vars COMMA struct_decl_var */
#line 254 "stparserimpl.lemon"
{
    auto& list = pParser->getStructMemberList();
    yylhsminor.yy188 = list.add(yymsp[-2].minor.yy188, yymsp[0].minor.yy93);
}
#line 1599 "stparserimpl.c"
  yymsp[-2].minor.yy188 = yylhsminor.yy188;
        break;
      case 32: /* struct_decl_vars ::= struct_decl_var */
#line 258 "stparserimpl.lemon"
{
    auto& list = pParser->getStructMemberList();
    yylhsminor.yy188 = list.add(yymsp[0].minor.yy93);
}
#line 1608 "stparserimpl.c"
  yymsp[0].minor.yy188 = yylhsminor.yy188;
        break;
      case 33: /* struct_decl_var ::= identifier COLON type_qualified */
#line 263 "stparserimpl.lemon"
{
    yylhsminor.yy93 = pParser->createNode<yal::frontend::STStructMember>(yymsp[-2].minor.yy67, yymsp[0].minor.yy37);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy67->getSourceInfo(), yymsp[0].minor.yy37->getSourceInfo(),pParser->getSourceHandle());
    yylhsminor.yy93->setSourceInfo(srcInfo);
}
#line 1618 "stparserimpl.c"
  yymsp[-2].minor.yy93 = yylhsminor.yy93;
        break;
      case 34: /* function_param_list ::= PAR_BEGIN function_args_decl PAR_END */
#line 272 "stparserimpl.lemon"
{
    yymsp[-2].minor.yy136 = yymsp[-1].minor.yy136;
}
#line 1626 "stparserimpl.c"
        break;
      case 35: /* decl_function ::= FUNCTION identifier function_param_list function_return_decl SCOPE_BEGIN statement_list_or_empty SCOPE_END */
#line 277 "stparserimpl.lemon"
{
    yylhsminor.yy94 = pParser->createNode<yal::frontend::STDeclFunction>(yymsp[-5].minor.yy67, yymsp[-3].minor.yy37, yymsp[-4].minor.yy136, yymsp[-1].minor.yy47, *pParser);
    yylhsminor.yy94->setSourceInfo(CreateSourceInfo(yymsp[-6].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle()));
}
#line 1634 "stparserimpl.c"
  yymsp[-6].minor.yy94 = yylhsminor.yy94;
        break;
      case 36: /* decl_type_functions_scope ::= IMPL type_specifier SCOPE_BEGIN decl_type_function_list SCOPE_END */
#line 284 "stparserimpl.lemon"
{
        yylhsminor.yy4 = pParser->createNode<yal::frontend::STDeclTypeFunctions>(yymsp[-3].minor.yy154,yymsp[-1].minor.yy99, *pParser);
        yylhsminor.yy4->setSourceInfo(CreateSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle()));
}
#line 1643 "stparserimpl.c"
  yymsp[-4].minor.yy4 = yylhsminor.yy4;
        break;
      case 37: /* decl_type_function_list ::= decl_type_function_list decl_function */
#line 289 "stparserimpl.lemon"
{
    auto& list = pParser->getDeclList();
    yylhsminor.yy99 = list.add(yymsp[-1].minor.yy99, yymsp[0].minor.yy94);
}
#line 1652 "stparserimpl.c"
  yymsp[-1].minor.yy99 = yylhsminor.yy99;
        break;
      case 38: /* decl_type_function_list ::= decl_function */
#line 294 "stparserimpl.lemon"
{
    auto& list = pParser->getDeclList();
    yylhsminor.yy99 = list.add(yymsp[0].minor.yy94);
}
#line 1661 "stparserimpl.c"
  yymsp[0].minor.yy99 = yylhsminor.yy99;
        break;
      case 39: /* function_args_decl ::= function_args_decl COMMA function_arg_decl */
#line 302 "stparserimpl.lemon"
{
        auto& list = pParser->getDeclParamList();
        yylhsminor.yy136 = list.add(yymsp[-2].minor.yy136, yymsp[0].minor.yy45);
}
#line 1670 "stparserimpl.c"
  yymsp[-2].minor.yy136 = yylhsminor.yy136;
        break;
      case 40: /* function_args_decl ::= function_arg_decl */
#line 307 "stparserimpl.lemon"
{
    auto& list = pParser->getDeclParamList();
    yylhsminor.yy136 = list.add(yymsp[0].minor.yy45);
}
#line 1679 "stparserimpl.c"
  yymsp[0].minor.yy136 = yylhsminor.yy136;
        break;
      case 41: /* function_args_decl ::= */
#line 312 "stparserimpl.lemon"
{
   yymsp[1].minor.yy136 = pParser->getDeclParamList().getRangeEmpty();
}
#line 1687 "stparserimpl.c"
        break;
      case 42: /* function_arg_decl ::= identifier COLON type_qualified */
#line 316 "stparserimpl.lemon"
{
        yylhsminor.yy45 = pParser->createNode<yal::frontend::STDeclParam>(yymsp[-2].minor.yy67, yymsp[0].minor.yy37);
        yylhsminor.yy45->setSourceInfo(yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy67->getSourceInfo(),yymsp[0].minor.yy37->getSourceInfo(), pParser->getSourceHandle()));
}
#line 1695 "stparserimpl.c"
  yymsp[-2].minor.yy45 = yylhsminor.yy45;
        break;
      case 43: /* function_arg_decl ::= qualifier self */
#line 321 "stparserimpl.lemon"
{
        auto qualType = pParser->createNode<yal::frontend::STQualType>(nullptr, yymsp[-1].minor.yy185);
        yylhsminor.yy45 = pParser->createNode<yal::frontend::STDeclParam>(yymsp[0].minor.yy67, qualType);
        yylhsminor.yy45->setSourceInfo(yymsp[0].minor.yy67->getSourceInfo());
}
#line 1705 "stparserimpl.c"
  yymsp[-1].minor.yy45 = yylhsminor.yy45;
        break;
      case 44: /* function_return_decl ::= COLON type_qualified */
#line 327 "stparserimpl.lemon"
{ yymsp[-1].minor.yy37 = yymsp[0].minor.yy37;}
#line 1711 "stparserimpl.c"
        break;
      case 45: /* function_return_decl ::= */
      case 59: /* var_type_spec ::= */ yytestcase(yyruleno==59);
#line 328 "stparserimpl.lemon"
{yymsp[1].minor.yy37 = nullptr;}
#line 1717 "stparserimpl.c"
        break;
      case 46: /* statement_list_or_empty ::= */
#line 332 "stparserimpl.lemon"
{yymsp[1].minor.yy47 = pParser->getStmtList().getRangeEmpty();}
#line 1722 "stparserimpl.c"
        break;
      case 47: /* statement_list_or_empty ::= statement_list */
#line 333 "stparserimpl.lemon"
{yylhsminor.yy47 = yymsp[0].minor.yy47;}
#line 1727 "stparserimpl.c"
  yymsp[0].minor.yy47 = yylhsminor.yy47;
        break;
      case 48: /* statement_list ::= statement_list statement */
#line 336 "stparserimpl.lemon"
{
    auto& list = pParser->getStmtList();
    yylhsminor.yy47 = list.add(yymsp[-1].minor.yy47,yymsp[0].minor.yy121);
}
#line 1736 "stparserimpl.c"
  yymsp[-1].minor.yy47 = yylhsminor.yy47;
        break;
      case 49: /* statement_list ::= statement */
#line 340 "stparserimpl.lemon"
{
    auto& list = pParser->getStmtList();
    yylhsminor.yy47 = list.add(yymsp[0].minor.yy121);
}
#line 1745 "stparserimpl.c"
  yymsp[0].minor.yy47 = yylhsminor.yy47;
        break;
      case 50: /* statement ::= expression ASSIGN expression SEMI_COLON */
#line 346 "stparserimpl.lemon"
{
   yylhsminor.yy121 = pParser->createNode<yal::frontend::STStmtAssign>(yymsp[-3].minor.yy104,yymsp[-1].minor.yy104);
   yylhsminor.yy121->setSourceInfo(yal::frontend::CreateSourceInfo(yymsp[-3].minor.yy104->getSourceInfo(), yymsp[-1].minor.yy104->getSourceInfo(), pParser->getSourceHandle()));
}
#line 1754 "stparserimpl.c"
  yymsp[-3].minor.yy121 = yylhsminor.yy121;
        break;
      case 51: /* statement ::= decl_var SEMI_COLON */
#line 350 "stparserimpl.lemon"
{
    yylhsminor.yy121 = pParser->createNode<yal::frontend::STStmtDecl>(yymsp[-1].minor.yy177);
    yylhsminor.yy121->setSourceInfo(yymsp[-1].minor.yy177->getSourceInfo());
}
#line 1763 "stparserimpl.c"
  yymsp[-1].minor.yy121 = yylhsminor.yy121;
        break;
      case 52: /* statement ::= expression SEMI_COLON */
#line 355 "stparserimpl.lemon"
{
    yylhsminor.yy121 = pParser->createNode<yal::frontend::STStmtExpression>(yymsp[-1].minor.yy104);
    yylhsminor.yy121->setSourceInfo(yymsp[-1].minor.yy104->getSourceInfo());
}
#line 1772 "stparserimpl.c"
  yymsp[-1].minor.yy121 = yylhsminor.yy121;
        break;
      case 53: /* statement ::= RETURN expression SEMI_COLON */
#line 360 "stparserimpl.lemon"
{
    yylhsminor.yy121 = pParser->createNode<yal::frontend::STStmtReturn>(yymsp[-1].minor.yy104);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy0, yymsp[-1].minor.yy104->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy121->setSourceInfo(srcInfo);
}
#line 1782 "stparserimpl.c"
  yymsp[-2].minor.yy121 = yylhsminor.yy121;
        break;
      case 54: /* statement ::= RETURN SEMI_COLON */
#line 366 "stparserimpl.lemon"
{
    yylhsminor.yy121 = pParser->createNode<yal::frontend::STStmtReturn>(nullptr);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[-1].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy121->setSourceInfo(srcInfo);
}
#line 1792 "stparserimpl.c"
  yymsp[-1].minor.yy121 = yylhsminor.yy121;
        break;
      case 55: /* statement ::= SCOPE_BEGIN statement_list_or_empty SCOPE_END */
#line 372 "stparserimpl.lemon"
{
    yylhsminor.yy121 = pParser->createNode<yal::frontend::STStmtListScoped>(yymsp[-1].minor.yy47, *pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy121->setSourceInfo(srcInfo);
}
#line 1802 "stparserimpl.c"
  yymsp[-2].minor.yy121 = yylhsminor.yy121;
        break;
      case 56: /* decl_var ::= VAR identifier var_type_spec ASSIGN expression */
#line 380 "stparserimpl.lemon"
{
    yylhsminor.yy177 = pParser->createNode<yal::frontend::STDeclVar>(yymsp[-3].minor.yy67, yymsp[-2].minor.yy37, yymsp[0].minor.yy104, false);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy104->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy177->setSourceInfo(srcInfo);
}
#line 1812 "stparserimpl.c"
  yymsp[-4].minor.yy177 = yylhsminor.yy177;
        break;
      case 57: /* decl_var ::= LET identifier var_type_spec ASSIGN expression */
#line 386 "stparserimpl.lemon"
{
    yylhsminor.yy177 = pParser->createNode<yal::frontend::STDeclVar>(yymsp[-3].minor.yy67, yymsp[-2].minor.yy37, yymsp[0].minor.yy104, true);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy104->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy177->setSourceInfo(srcInfo);
}
#line 1822 "stparserimpl.c"
  yymsp[-4].minor.yy177 = yylhsminor.yy177;
        break;
      case 58: /* var_type_spec ::= COLON type_qualified */
#line 392 "stparserimpl.lemon"
{yymsp[-1].minor.yy37 = yymsp[0].minor.yy37;}
#line 1828 "stparserimpl.c"
        break;
      case 60: /* expression ::= PAR_BEGIN expression PAR_END */
#line 398 "stparserimpl.lemon"
{yymsp[-2].minor.yy104 = yymsp[-1].minor.yy104;}
#line 1833 "stparserimpl.c"
        break;
      case 61: /* expression ::= literal */
#line 399 "stparserimpl.lemon"
{yylhsminor.yy104 = yymsp[0].minor.yy104;}
#line 1838 "stparserimpl.c"
  yymsp[0].minor.yy104 = yylhsminor.yy104;
        break;
      case 62: /* expression ::= unaryexp */
#line 400 "stparserimpl.lemon"
{yylhsminor.yy104 = yymsp[0].minor.yy155;}
#line 1844 "stparserimpl.c"
  yymsp[0].minor.yy104 = yylhsminor.yy104;
        break;
      case 63: /* expression ::= binaryexp */
#line 401 "stparserimpl.lemon"
{yylhsminor.yy104 = yymsp[0].minor.yy12;}
#line 1850 "stparserimpl.c"
  yymsp[0].minor.yy104 = yylhsminor.yy104;
        break;
      case 64: /* expression ::= struct_init */
#line 402 "stparserimpl.lemon"
{ yylhsminor.yy104 = yymsp[0].minor.yy116;}
#line 1856 "stparserimpl.c"
  yymsp[0].minor.yy104 = yylhsminor.yy104;
        break;
      case 65: /* expression ::= expression AS type_qualified */
#line 404 "stparserimpl.lemon"
{
    yylhsminor.yy104 = pParser->createNode<yal::frontend::STExprCast>(yymsp[-2].minor.yy104, yymsp[0].minor.yy37);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy104->getSourceInfo(),
                                                   yymsp[0].minor.yy37->getSourceInfo(),
                                                   pParser->getSourceHandle());
    yylhsminor.yy104->setSourceInfo(srcInfo);
}
#line 1868 "stparserimpl.c"
  yymsp[-2].minor.yy104 = yylhsminor.yy104;
        break;
      case 66: /* expression ::= identifier_or_self */
#line 412 "stparserimpl.lemon"
{
    yylhsminor.yy104 = pParser->createNode<yal::frontend::STExprVarRef>(yymsp[0].minor.yy67);
    yylhsminor.yy104->setSourceInfo(yymsp[0].minor.yy67->getSourceInfo());
}
#line 1877 "stparserimpl.c"
  yymsp[0].minor.yy104 = yylhsminor.yy104;
        break;
      case 67: /* expression ::= expression DOT identifier */
#line 417 "stparserimpl.lemon"
{
    yylhsminor.yy104 = pParser->createNode<yal::frontend::STExprStructVarRef>(yymsp[-2].minor.yy104, yymsp[0].minor.yy67);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy104->getSourceInfo(), yymsp[0].minor.yy67->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy104->setSourceInfo(srcInfo);
}
#line 1887 "stparserimpl.c"
  yymsp[-2].minor.yy104 = yylhsminor.yy104;
        break;
      case 68: /* expression ::= identifier PAR_BEGIN function_call_args PAR_END */
#line 423 "stparserimpl.lemon"
{
    yylhsminor.yy104 = pParser->createNode<yal::frontend::STExprFnCall>(yymsp[-3].minor.yy67,yymsp[-1].minor.yy162,*pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-3].minor.yy67->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy104->setSourceInfo(srcInfo);
}
#line 1897 "stparserimpl.c"
  yymsp[-3].minor.yy104 = yylhsminor.yy104;
        break;
      case 69: /* expression ::= expression DOT identifier PAR_BEGIN function_call_args PAR_END */
#line 429 "stparserimpl.lemon"
{
    yylhsminor.yy104 = pParser->createNode<yal::frontend::STExprFnCall>(yymsp[-3].minor.yy67,yymsp[-5].minor.yy104,yymsp[-1].minor.yy162,*pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-5].minor.yy104->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy104->setSourceInfo(srcInfo);
}
#line 1907 "stparserimpl.c"
  yymsp[-5].minor.yy104 = yylhsminor.yy104;
        break;
      case 70: /* expression ::= type_specifier COLON COLON identifier PAR_BEGIN function_call_args PAR_END */
#line 435 "stparserimpl.lemon"
{
    yylhsminor.yy104 = pParser->createNode<yal::frontend::STExprFnCall>(yymsp[-3].minor.yy67,yymsp[-6].minor.yy154,yymsp[-1].minor.yy162,*pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-6].minor.yy154->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy104->setSourceInfo(srcInfo);
}
#line 1917 "stparserimpl.c"
  yymsp[-6].minor.yy104 = yylhsminor.yy104;
        break;
      case 71: /* binaryexp ::= expression AND expression */
#line 443 "stparserimpl.lemon"
{
        yylhsminor.yy12 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::And,
                                                        yymsp[-2].minor.yy104, yymsp[0].minor.yy104);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy104->getSourceInfo(), yymsp[0].minor.yy104->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy12->setSourceInfo(srcInfo);
}
#line 1928 "stparserimpl.c"
  yymsp[-2].minor.yy12 = yylhsminor.yy12;
        break;
      case 72: /* binaryexp ::= expression OR expression */
#line 449 "stparserimpl.lemon"
{
        yylhsminor.yy12 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Or,
                                                        yymsp[-2].minor.yy104, yymsp[0].minor.yy104);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy104->getSourceInfo(), yymsp[0].minor.yy104->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy12->setSourceInfo(srcInfo);
}
#line 1939 "stparserimpl.c"
  yymsp[-2].minor.yy12 = yylhsminor.yy12;
        break;
      case 73: /* binaryexp ::= expression PLUS expression */
#line 455 "stparserimpl.lemon"
{
        yylhsminor.yy12 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Plus,
                                                        yymsp[-2].minor.yy104, yymsp[0].minor.yy104);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy104->getSourceInfo(), yymsp[0].minor.yy104->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy12->setSourceInfo(srcInfo);
}
#line 1950 "stparserimpl.c"
  yymsp[-2].minor.yy12 = yylhsminor.yy12;
        break;
      case 74: /* binaryexp ::= expression MINUS expression */
#line 461 "stparserimpl.lemon"
{
        yylhsminor.yy12 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Minus,
                                                        yymsp[-2].minor.yy104, yymsp[0].minor.yy104);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy104->getSourceInfo(), yymsp[0].minor.yy104->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy12->setSourceInfo(srcInfo);
}
#line 1961 "stparserimpl.c"
  yymsp[-2].minor.yy12 = yylhsminor.yy12;
        break;
      case 75: /* binaryexp ::= expression MULT expression */
#line 467 "stparserimpl.lemon"
{
        yylhsminor.yy12 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Mult,
                                                        yymsp[-2].minor.yy104, yymsp[0].minor.yy104);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy104->getSourceInfo(), yymsp[0].minor.yy104->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy12->setSourceInfo(srcInfo);
}
#line 1972 "stparserimpl.c"
  yymsp[-2].minor.yy12 = yylhsminor.yy12;
        break;
      case 76: /* binaryexp ::= expression DIV expression */
#line 473 "stparserimpl.lemon"
{
        yylhsminor.yy12 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Div,
                                                        yymsp[-2].minor.yy104, yymsp[0].minor.yy104);
}
#line 1981 "stparserimpl.c"
  yymsp[-2].minor.yy12 = yylhsminor.yy12;
        break;
      case 77: /* binaryexp ::= expression MOD expression */
#line 477 "stparserimpl.lemon"
{
        yylhsminor.yy12 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Mod,
                                                        yymsp[-2].minor.yy104, yymsp[0].minor.yy104);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy104->getSourceInfo(), yymsp[0].minor.yy104->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy12->setSourceInfo(srcInfo);
}
#line 1992 "stparserimpl.c"
  yymsp[-2].minor.yy12 = yylhsminor.yy12;
        break;
      case 78: /* binaryexp ::= expression EQ expression */
#line 484 "stparserimpl.lemon"
{
        yylhsminor.yy12 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Eq,
                                                        yymsp[-2].minor.yy104, yymsp[0].minor.yy104);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy104->getSourceInfo(), yymsp[0].minor.yy104->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy12->setSourceInfo(srcInfo);
}
#line 2003 "stparserimpl.c"
  yymsp[-2].minor.yy12 = yylhsminor.yy12;
        break;
      case 79: /* binaryexp ::= expression NE expression */
#line 491 "stparserimpl.lemon"
{
        yylhsminor.yy12 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Ne,
                                                        yymsp[-2].minor.yy104, yymsp[0].minor.yy104);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy104->getSourceInfo(), yymsp[0].minor.yy104->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy12->setSourceInfo(srcInfo);
}
#line 2014 "stparserimpl.c"
  yymsp[-2].minor.yy12 = yylhsminor.yy12;
        break;
      case 80: /* binaryexp ::= expression LE expression */
#line 498 "stparserimpl.lemon"
{
        yylhsminor.yy12 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Le,
                                                        yymsp[-2].minor.yy104, yymsp[0].minor.yy104);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy104->getSourceInfo(), yymsp[0].minor.yy104->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy12->setSourceInfo(srcInfo);
}
#line 2025 "stparserimpl.c"
  yymsp[-2].minor.yy12 = yylhsminor.yy12;
        break;
      case 81: /* binaryexp ::= expression LT expression */
#line 505 "stparserimpl.lemon"
{
        yylhsminor.yy12 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Lt,
                                                        yymsp[-2].minor.yy104, yymsp[0].minor.yy104);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy104->getSourceInfo(), yymsp[0].minor.yy104->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy12->setSourceInfo(srcInfo);
}
#line 2036 "stparserimpl.c"
  yymsp[-2].minor.yy12 = yylhsminor.yy12;
        break;
      case 82: /* binaryexp ::= expression GE expression */
#line 512 "stparserimpl.lemon"
{
        yylhsminor.yy12 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Ge,
                                                        yymsp[-2].minor.yy104, yymsp[0].minor.yy104);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy104->getSourceInfo(), yymsp[0].minor.yy104->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy12->setSourceInfo(srcInfo);
}
#line 2047 "stparserimpl.c"
  yymsp[-2].minor.yy12 = yylhsminor.yy12;
        break;
      case 83: /* binaryexp ::= expression GT expression */
#line 519 "stparserimpl.lemon"
{
        yylhsminor.yy12 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Gt,
                                                        yymsp[-2].minor.yy104, yymsp[0].minor.yy104);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy104->getSourceInfo(), yymsp[0].minor.yy104->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy12->setSourceInfo(srcInfo);
}
#line 2058 "stparserimpl.c"
  yymsp[-2].minor.yy12 = yylhsminor.yy12;
        break;
      case 84: /* unaryexp ::= NOT expression */
#line 528 "stparserimpl.lemon"
{
    yylhsminor.yy155 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Not,
                                               yymsp[0].minor.yy104);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy104->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy155->setSourceInfo(srcInfo);
}
#line 2069 "stparserimpl.c"
  yymsp[-1].minor.yy155 = yylhsminor.yy155;
        break;
      case 85: /* unaryexp ::= BIT_NOT expression */
#line 534 "stparserimpl.lemon"
{
    yylhsminor.yy155 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::BitNot,
                                               yymsp[0].minor.yy104);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy104->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy155->setSourceInfo(srcInfo);
}
#line 2080 "stparserimpl.c"
  yymsp[-1].minor.yy155 = yylhsminor.yy155;
        break;
      case 86: /* unaryexp ::= MINUS expression */
#line 540 "stparserimpl.lemon"
{
    yylhsminor.yy155 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Negate,
                                               yymsp[0].minor.yy104);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy104->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy155->setSourceInfo(srcInfo);
}
#line 2091 "stparserimpl.c"
  yymsp[-1].minor.yy155 = yylhsminor.yy155;
        break;
      case 87: /* unaryexp ::= REFERENCE expression */
#line 547 "stparserimpl.lemon"
{
    yylhsminor.yy155 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Reference,
                                               yymsp[0].minor.yy104);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy104->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy155->setSourceInfo(srcInfo);
}
#line 2102 "stparserimpl.c"
  yymsp[-1].minor.yy155 = yylhsminor.yy155;
        break;
      case 88: /* unaryexp ::= MULT expression */
#line 554 "stparserimpl.lemon"
{
    yylhsminor.yy155 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Derefence,
                                               yymsp[0].minor.yy104);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy104->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy155->setSourceInfo(srcInfo);
}
#line 2113 "stparserimpl.c"
  yymsp[-1].minor.yy155 = yylhsminor.yy155;
        break;
      case 89: /* function_call_args ::= function_call_args COMMA expression */
#line 564 "stparserimpl.lemon"
{
    auto& list = pParser->getExprList();
    yylhsminor.yy162 = list.add(yymsp[-2].minor.yy162, yymsp[0].minor.yy104);
}
#line 2122 "stparserimpl.c"
  yymsp[-2].minor.yy162 = yylhsminor.yy162;
        break;
      case 90: /* function_call_args ::= expression */
#line 568 "stparserimpl.lemon"
{
    auto& list = pParser->getExprList();
    yylhsminor.yy162 = list.add(yymsp[0].minor.yy104);
}
#line 2131 "stparserimpl.c"
  yymsp[0].minor.yy162 = yylhsminor.yy162;
        break;
      case 91: /* function_call_args ::= */
#line 572 "stparserimpl.lemon"
{
    yymsp[1].minor.yy162= pParser->getExprList().getRangeEmpty();
}
#line 2139 "stparserimpl.c"
        break;
      case 92: /* struct_init ::= type_specifier SCOPE_BEGIN struct_member_init_list SCOPE_END */
#line 579 "stparserimpl.lemon"
{
    yylhsminor.yy116 = pParser->createNode<yal::frontend::STExprStructInit>(yymsp[-3].minor.yy154, yymsp[-1].minor.yy176, *pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-3].minor.yy154->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy116->setSourceInfo(srcInfo);
}
#line 2148 "stparserimpl.c"
  yymsp[-3].minor.yy116 = yylhsminor.yy116;
        break;
      case 93: /* struct_member_init_list ::= */
#line 585 "stparserimpl.lemon"
{yymsp[1].minor.yy176 = pParser->getStructInitList().getRangeEmpty();}
#line 2154 "stparserimpl.c"
        break;
      case 94: /* struct_member_init_list ::= struct_member_init_list COMMA struct_member_init */
#line 586 "stparserimpl.lemon"
{
     auto& list = pParser->getStructInitList();
     yylhsminor.yy176 = list.add(yymsp[-2].minor.yy176, yymsp[0].minor.yy122);
}
#line 2162 "stparserimpl.c"
  yymsp[-2].minor.yy176 = yylhsminor.yy176;
        break;
      case 95: /* struct_member_init_list ::= struct_member_init */
#line 590 "stparserimpl.lemon"
{
    auto& list = pParser->getStructInitList();
    yylhsminor.yy176 = list.add(yymsp[0].minor.yy122);
}
#line 2171 "stparserimpl.c"
  yymsp[0].minor.yy176 = yylhsminor.yy176;
        break;
      case 96: /* struct_member_init ::= identifier COLON expression */
#line 595 "stparserimpl.lemon"
{
    yylhsminor.yy122 = pParser->createNode<yal::frontend::STStructMemberInit>(yymsp[-2].minor.yy67,yymsp[0].minor.yy104);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy67->getSourceInfo(), yymsp[0].minor.yy104->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy122->setSourceInfo(srcInfo);
}
#line 2181 "stparserimpl.c"
  yymsp[-2].minor.yy122 = yylhsminor.yy122;
        break;
      case 97: /* literal ::= INTEGER_LITERAL */
      case 98: /* literal ::= INTEGER_LITERAL_I8 */ yytestcase(yyruleno==98);
      case 99: /* literal ::= INTEGER_LITERAL_I16 */ yytestcase(yyruleno==99);
      case 100: /* literal ::= INTEGER_LITERAL_I32 */ yytestcase(yyruleno==100);
      case 101: /* literal ::= INTEGER_LITERAL_I64 */ yytestcase(yyruleno==101);
      case 102: /* literal ::= INTEGER_LITERAL_U8 */ yytestcase(yyruleno==102);
      case 103: /* literal ::= INTEGER_LITERAL_U16 */ yytestcase(yyruleno==103);
      case 104: /* literal ::= INTEGER_LITERAL_U32 */ yytestcase(yyruleno==104);
      case 105: /* literal ::= INTEGER_LITERAL_U64 */ yytestcase(yyruleno==105);
#line 603 "stparserimpl.lemon"
{
        yylhsminor.yy104 = pParser->createNode<yal::frontend::STExprIntegerLiteral>(yymsp[0].minor.yy0);
        auto srcInfo =yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy104->setSourceInfo(srcInfo);
   }
#line 2199 "stparserimpl.c"
  yymsp[0].minor.yy104 = yylhsminor.yy104;
        break;
      case 106: /* literal ::= FLOAT_LITERAL */
      case 107: /* literal ::= FLOAT_LITERAL_32 */ yytestcase(yyruleno==107);
      case 108: /* literal ::= FLOAT_LITERAL_64 */ yytestcase(yyruleno==108);
#line 648 "stparserimpl.lemon"
{
        yylhsminor.yy104 = pParser->createNode<yal::frontend::STExprFloatLiteral>(yymsp[0].minor.yy0);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy104->setSourceInfo(srcInfo);
}
#line 2211 "stparserimpl.c"
  yymsp[0].minor.yy104 = yylhsminor.yy104;
        break;
      case 109: /* literal ::= BOOL_LITERAL */
#line 665 "stparserimpl.lemon"
{
        yylhsminor.yy104 = pParser->createNode<yal::frontend::STExprBoolLiteral>(yymsp[0].minor.yy0.tokenStr);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy104->setSourceInfo(srcInfo);
}
#line 2221 "stparserimpl.c"
  yymsp[0].minor.yy104 = yylhsminor.yy104;
        break;
      default:
      /* (110) module ::= decls END */ yytestcase(yyruleno==110);
      /* (111) type_array ::= type_builtin ARRAY_BEGIN ARRAY_END */ yytestcase(yyruleno==111);
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
#line 2305 "stparserimpl.c"
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
