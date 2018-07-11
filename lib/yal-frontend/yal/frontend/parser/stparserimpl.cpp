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
#define YAL_TOKEN_IDENTIFIER_MULTI               26
#define YAL_TOKEN_IDENTIFIER                     27
#define YAL_TOKEN_SELF                           28
#define YAL_TOKEN_TYPE_BOOL                      29
#define YAL_TOKEN_TYPE_INT8                      30
#define YAL_TOKEN_TYPE_UINT8                     31
#define YAL_TOKEN_TYPE_INT16                     32
#define YAL_TOKEN_TYPE_UINT16                    33
#define YAL_TOKEN_TYPE_INT32                     34
#define YAL_TOKEN_TYPE_UINT32                    35
#define YAL_TOKEN_TYPE_INT64                     36
#define YAL_TOKEN_TYPE_UINT64                    37
#define YAL_TOKEN_TYPE_FLOAT                     38
#define YAL_TOKEN_TYPE_DOUBLE                    39
#define YAL_TOKEN_ARRAY_BEGIN                    40
#define YAL_TOKEN_ARRAY_END                      41
#define YAL_TOKEN_MUT                            42
#define YAL_TOKEN_IMPORT                         43
#define YAL_TOKEN_SEMI_COLON                     44
#define YAL_TOKEN_COLON                          45
#define YAL_TOKEN_TYPE                           46
#define YAL_TOKEN_ALIAS                          47
#define YAL_TOKEN_FROM                           48
#define YAL_TOKEN_STRUCT                         49
#define YAL_TOKEN_SCOPE_BEGIN                    50
#define YAL_TOKEN_SCOPE_END                      51
#define YAL_TOKEN_COMMA                          52
#define YAL_TOKEN_PAR_BEGIN                      53
#define YAL_TOKEN_PAR_END                        54
#define YAL_TOKEN_FUNCTION                       55
#define YAL_TOKEN_IMPL                           56
#define YAL_TOKEN_RETURN                         57
#define YAL_TOKEN_VAR                            58
#define YAL_TOKEN_LET                            59
#define YAL_TOKEN_INTEGER_LITERAL                60
#define YAL_TOKEN_INTEGER_LITERAL_I8             61
#define YAL_TOKEN_INTEGER_LITERAL_I16            62
#define YAL_TOKEN_INTEGER_LITERAL_I32            63
#define YAL_TOKEN_INTEGER_LITERAL_I64            64
#define YAL_TOKEN_INTEGER_LITERAL_U8             65
#define YAL_TOKEN_INTEGER_LITERAL_U16            66
#define YAL_TOKEN_INTEGER_LITERAL_U32            67
#define YAL_TOKEN_INTEGER_LITERAL_U64            68
#define YAL_TOKEN_FLOAT_LITERAL                  69
#define YAL_TOKEN_FLOAT_LITERAL_32               70
#define YAL_TOKEN_FLOAT_LITERAL_64               71
#define YAL_TOKEN_BOOL_LITERAL                   72
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
**    YYNTOKEN           Number of terminal symbols
**    YY_MAX_SHIFT       Maximum value for shift actions
**    YY_MIN_SHIFTREDUCE Minimum value for shift-reduce actions
**    YY_MAX_SHIFTREDUCE Maximum value for shift-reduce actions
**    YY_ERROR_ACTION    The yy_action[] code for syntax error
**    YY_ACCEPT_ACTION   The yy_action[] code for accept
**    YY_NO_ACTION       The yy_action[] code for no-op
**    YY_MIN_REDUCE      Minimum value for reduce actions
**    YY_MAX_REDUCE      Maximum value for reduce actions
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/************* Begin control #defines *****************************************/
#define YYCODETYPE unsigned char
#define YYNOCODE 114
#define YYACTIONTYPE unsigned short int
#if INTERFACE
#define YALSTParserTOKENTYPE yal::frontend::TokenInfo
#endif
typedef union {
  int yyinit;
  YALSTParserTOKENTYPE yy0;
  yal::frontend::STStatement* yy11;
  yal::frontend::ParseListStructMember::Range yy12;
  yal::frontend::STDeclModule* yy16;
  yal::frontend::STExprUnaryOperator* yy36;
  yal::frontend::STExprBinaryOperator* yy38;
  yal::frontend::STDeclImport* yy41;
  yal::frontend::STDeclTypeFunctions* yy42;
  yal::frontend::STDeclParam* yy49;
  yal::frontend::STDeclStruct* yy65;
  yal::frontend::STDeclVar* yy67;
  yal::frontend::ParseListDeclParam::Range yy68;
  yal::frontend::STDecl* yy78;
  yal::frontend::STType* yy82;
  yal::frontend::ParseListExpr::Range yy90;
  yal::frontend::ParseListStructInit::Range yy91;
  yal::frontend::STQualType* yy95;
  yal::frontend::STStructMember* yy127;
  yal::frontend::STDeclFunction* yy128;
  yal::frontend::ParseListStmt::Range yy137;
  yal::frontend::STExprStructInit* yy140;
  yal::frontend::STIdentifier* yy173;
  uint32_t yy175;
  yal::frontend::STExpression* yy178;
  yal::frontend::STStructMemberInit* yy209;
  yal::frontend::STDeclAlias* yy210;
  yal::frontend::ParseListDecl::Range yy223;
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
#define YYNSTATE             117
#define YYNRULE              119
#define YYNTOKEN             73
#define YY_MAX_SHIFT         116
#define YY_MIN_SHIFTREDUCE   200
#define YY_MAX_SHIFTREDUCE   318
#define YY_ERROR_ACTION      319
#define YY_ACCEPT_ACTION     320
#define YY_NO_ACTION         321
#define YY_MIN_REDUCE        322
#define YY_MAX_REDUCE        440
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
**   N == YY_ERROR_ACTION               A syntax error has occurred.
**
**   N == YY_ACCEPT_ACTION              The parser accepts its input.
**
**   N == YY_NO_ACTION                  No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
**   N between YY_MIN_REDUCE            Reduce by rule N-YY_MIN_REDUCE
**     and YY_MAX_REDUCE
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as either:
**
**    (A)   N = yy_action[ yy_shift_ofst[S] + X ]
**    (B)   N = yy_default[S]
**
** The (A) formula is preferred.  The B formula is used instead if
** yy_lookahead[yy_shift_ofst[S]+X] is not equal to X.
**
** The formulas above are for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array.
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
#define YY_ACTTAB_COUNT (1076)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    56,   35,  320,  107,  114,  367,   88,   89,  338,  332,
 /*    10 */   331,   51,   90,   85,  370,   14,   12,  357,  332,  331,
 /*    20 */    15,   16,   94,   13,   94,   87,  207,  208,  211,  217,
 /*    30 */   218,  219,  220,  221,  222,  223,  224,  225,  226,  227,
 /*    40 */   337,  114,  208,  354,   53,  338,  332,  331,   83,  424,
 /*    50 */     1,  423,   43,   30,  397,  332,  104,    6,   54,   52,
 /*    60 */   304,  305,  306,  307,  308,  309,  310,  311,  312,  313,
 /*    70 */   314,  315,  316,   14,   12,  207,  208,  337,   15,   16,
 /*    80 */   374,   13,   32,   58,  207,  208,  211,  217,  218,  219,
 /*    90 */   220,  221,  222,  223,  224,  225,  226,  227,   25,   24,
 /*   100 */    23,   56,  262,  388,  107,   32,   39,  395,  114,   32,
 /*   110 */   111,   30,  338,  332,  331,  369,   42,  109,  304,  305,
 /*   120 */   306,  307,  308,  309,  310,  311,  312,  313,  314,  315,
 /*   130 */   316,  362,   86,  106,   14,   12,   34,   33,  110,   15,
 /*   140 */    16,  372,   13,   32,  337,  207,  208,  211,  217,  218,
 /*   150 */   219,  220,  221,  222,  223,  224,  225,  226,  227,  319,
 /*   160 */   319,  319,  319,  319,  319,   27,   26,   25,   24,   23,
 /*   170 */   233,  115,   30,  230,  363,   39,   32,  208,  350,  304,
 /*   180 */   305,  306,  307,  308,  309,  310,  311,  312,  313,  314,
 /*   190 */   315,  316,  108,  230,  368,  211,  114,  208,   95,  230,
 /*   200 */   105,  332,   84,  335,  396,  114,   92,  112,  373,  338,
 /*   210 */   332,  331,  108,  244,  109,   50,   36,   58,  108,    3,
 /*   220 */    91,  379,   61,  392,  393,  391,   57,  394,  361,  114,
 /*   230 */    55,   95,  337,  105,  332,   84,  335,  396,   10,   92,
 /*   240 */   276,  337,   10,   80,  277,  299,   48,  331,  356,   47,
 /*   250 */   331,    7,    3,   93,  379,   61,  392,  393,  391,   38,
 /*   260 */   394,  242,  238,   49,    8,  337,  263,    9,   44,   28,
 /*   270 */    29,  259,  243,   11,   22,   21,   17,   19,   18,   20,
 /*   280 */    27,   26,   25,   24,   23,   44,   28,   29,   40,   39,
 /*   290 */    39,   22,   21,   17,   19,   18,   20,   27,   26,   25,
 /*   300 */    24,   23,    4,    2,  116,    5,   45,   39,  207,  208,
 /*   310 */   260,  217,  218,  219,  220,  221,  222,  223,  224,  225,
 /*   320 */   226,  227,   37,  231,   41,   59,  318,  261,  113,   46,
 /*   330 */    44,   28,   29,  237,   58,   31,   22,   21,   17,   19,
 /*   340 */    18,   20,   27,   26,   25,   24,   23,   44,   28,   29,
 /*   350 */   236,  439,   39,   22,   21,   17,   19,   18,   20,   27,
 /*   360 */    26,   25,   24,   23,   60,  321,  322,  321,  324,   39,
 /*   370 */   328,  326,  258,  321,  321,  114,  325,   95,  321,  105,
 /*   380 */   332,   84,  335,  396,  321,   92,  321,  321,  321,  321,
 /*   390 */   321,  327,  321,  321,  321,  321,  321,  321,  321,  268,
 /*   400 */   378,   61,  392,  393,  391,  321,  394,   44,   28,   29,
 /*   410 */   321,  337,  321,   22,   21,   17,   19,   18,   20,   27,
 /*   420 */    26,   25,   24,   23,  321,  321,  114,  321,   95,   39,
 /*   430 */   105,  332,   84,  335,  396,  321,  321,  114,  321,   95,
 /*   440 */   321,  105,  332,   84,  335,  396,  321,  321,  321,  321,
 /*   450 */   321,  321,   69,  392,  393,  391,   82,  394,  321,  321,
 /*   460 */   321,  321,  337,   69,  392,  393,  391,   81,  394,  321,
 /*   470 */   321,  321,  321,  337,  321,  321,  114,  321,   95,  321,
 /*   480 */   105,  332,   84,  335,  396,  321,  321,  321,  114,  321,
 /*   490 */    95,  321,  105,  332,   84,  335,  396,  321,  321,  321,
 /*   500 */   321,  321,   62,  392,  393,  391,  321,  394,  321,  321,
 /*   510 */   321,  321,  337,  321,   65,  392,  393,  391,  321,  394,
 /*   520 */   321,  114,  321,   95,  337,  105,  332,   84,  335,  396,
 /*   530 */   321,  321,  114,  321,   95,  321,  105,  332,   84,  335,
 /*   540 */   396,  321,  321,  321,  321,  321,  321,   66,  392,  393,
 /*   550 */   391,  321,  394,  321,  321,  321,  321,  337,   63,  392,
 /*   560 */   393,  391,  321,  394,  321,  114,  321,   95,  337,  105,
 /*   570 */   332,   84,  335,  396,  321,  321,  321,  321,  321,  114,
 /*   580 */   321,   95,  321,  105,  332,   84,  335,  396,  321,  321,
 /*   590 */   321,   67,  392,  393,  391,  321,  394,  321,  321,  321,
 /*   600 */   321,  337,  321,  321,  321,   68,  392,  393,  391,  321,
 /*   610 */   394,  321,  321,  321,  114,  337,   95,  321,  105,  332,
 /*   620 */    84,  335,  396,  321,  321,  114,  321,   95,  321,  105,
 /*   630 */   332,   84,  335,  396,  321,  321,  321,  321,  321,  321,
 /*   640 */    96,  392,  393,  391,  321,  394,  321,  321,  321,  321,
 /*   650 */   337,   97,  392,  393,  391,  321,  394,  321,  114,  321,
 /*   660 */    95,  337,  105,  332,   84,  335,  396,  321,  321,  114,
 /*   670 */   321,   95,  321,  105,  332,   84,  335,  396,  321,  321,
 /*   680 */   321,  321,  321,  321,   98,  392,  393,  391,  321,  394,
 /*   690 */   321,  321,  321,  321,  337,   99,  392,  393,  391,  321,
 /*   700 */   394,  321,  114,  321,   95,  337,  105,  332,   84,  335,
 /*   710 */   396,  321,  321,  321,  321,  321,  321,  114,  321,   95,
 /*   720 */   321,  105,  332,   84,  335,  396,  321,  321,  100,  392,
 /*   730 */   393,  391,  321,  394,  321,  321,  321,  321,  337,  321,
 /*   740 */   321,  321,  321,   71,  392,  393,  391,  321,  394,  321,
 /*   750 */   114,  321,   95,  337,  105,  332,   84,  335,  396,  321,
 /*   760 */   321,  114,  321,   95,  321,  105,  332,   84,  335,  396,
 /*   770 */   321,  321,  321,  321,  321,  321,   72,  392,  393,  391,
 /*   780 */   321,  394,  321,  321,  321,  321,  337,   73,  392,  393,
 /*   790 */   391,  321,  394,  321,  114,  321,   95,  337,  105,  332,
 /*   800 */    84,  335,  396,  321,  321,  114,  321,   95,  321,  105,
 /*   810 */   332,   84,  335,  396,  321,  321,  321,  321,  321,  321,
 /*   820 */    74,  392,  393,  391,  321,  394,  321,  321,  321,  321,
 /*   830 */   337,   75,  392,  393,  391,  321,  394,  321,  114,  321,
 /*   840 */    95,  337,  105,  332,   84,  335,  396,  321,  321,  321,
 /*   850 */   321,  321,  321,  114,  321,   95,  321,  105,  332,   84,
 /*   860 */   335,  396,  321,  321,   76,  392,  393,  391,  321,  394,
 /*   870 */   321,  321,  321,  321,  337,  321,  321,  321,  321,  101,
 /*   880 */   392,  393,  391,  321,  394,  321,  114,  321,   95,  337,
 /*   890 */   105,  332,   84,  335,  396,  321,  321,  114,  321,   95,
 /*   900 */   321,  105,  332,   84,  335,  396,  321,  321,  321,  321,
 /*   910 */   321,  321,  102,  392,  393,  391,  321,  394,  321,  321,
 /*   920 */   321,  321,  337,  103,  392,  393,  391,  321,  394,  321,
 /*   930 */   114,  321,   95,  337,  105,  332,   84,  335,  396,  321,
 /*   940 */   321,  114,  321,   95,  321,  105,  332,   84,  335,  396,
 /*   950 */   321,  321,  321,  321,  321,  321,   78,  392,  393,  391,
 /*   960 */   321,  394,  321,  321,  321,  321,  337,   79,  392,  393,
 /*   970 */   391,  321,  394,  321,  114,  321,   95,  337,  105,  332,
 /*   980 */    84,  335,  396,  321,  321,  321,  321,  321,  321,  114,
 /*   990 */   321,   95,  321,  105,  332,   84,  335,  396,  321,  321,
 /*  1000 */    70,  392,  393,  391,  321,  394,  321,  321,  321,  321,
 /*  1010 */   337,  321,  321,  321,  321,   77,  392,  393,  391,  321,
 /*  1020 */   394,  321,  114,  321,   95,  337,  105,  332,   84,  335,
 /*  1030 */   396,  321,   29,  321,  321,  321,   22,   21,   17,   19,
 /*  1040 */    18,   20,   27,   26,   25,   24,   23,  321,   64,  392,
 /*  1050 */   393,  391,   39,  394,  321,  321,  321,  321,  337,   22,
 /*  1060 */    21,   17,   19,   18,   20,   27,   26,   25,   24,   23,
 /*  1070 */   321,  321,  321,  321,  321,   39,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    79,   74,   75,   82,   76,   90,   78,   98,   80,   81,
 /*    10 */    82,   82,   98,   92,   93,   15,   16,   80,   81,   82,
 /*    20 */    20,   21,   82,   23,   82,   88,   26,   27,   28,   29,
 /*    30 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*    40 */   112,   76,   27,   78,   82,   80,   81,   82,  108,  109,
 /*    50 */    50,  109,   45,   53,   80,   81,   82,   57,   58,   59,
 /*    60 */    60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
 /*    70 */    70,   71,   72,   15,   16,   26,   27,  112,   20,   21,
 /*    80 */    77,   23,   79,   55,   26,   27,   28,   29,   30,   31,
 /*    90 */    32,   33,   34,   35,   36,   37,   38,   39,   16,   17,
 /*   100 */    18,   79,   44,   77,   82,   79,   24,   77,   76,   79,
 /*   110 */    78,   53,   80,   81,   82,   93,   45,   82,   60,   61,
 /*   120 */    62,   63,   64,   65,   66,   67,   68,   69,   70,   71,
 /*   130 */    72,   96,   97,   91,   15,   16,   47,   48,   49,   20,
 /*   140 */    21,   77,   23,   79,  112,   26,   27,   28,   29,   30,
 /*   150 */    31,   32,   33,   34,   35,   36,   37,   38,   39,    8,
 /*   160 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*   170 */    44,   45,   53,   23,   77,   24,   79,   27,   28,   60,
 /*   180 */    61,   62,   63,   64,   65,   66,   67,   68,   69,   70,
 /*   190 */    71,   72,   42,   23,   90,   28,   76,   27,   78,   23,
 /*   200 */    80,   81,   82,   83,   84,   76,   86,   78,   83,   80,
 /*   210 */    81,   82,   42,   51,   82,  111,   53,   55,   42,   99,
 /*   220 */   100,  101,  102,  103,  104,  105,   82,  107,   96,   76,
 /*   230 */    94,   78,  112,   80,   81,   82,   83,   84,   52,   86,
 /*   240 */    54,  112,   52,   82,   54,   51,   52,   50,   82,   50,
 /*   250 */    53,    1,   99,  100,  101,  102,  103,  104,  105,   52,
 /*   260 */   107,   54,   51,   52,    1,  112,   51,    1,    2,    3,
 /*   270 */     4,   44,   51,   45,    8,    9,   10,   11,   12,   13,
 /*   280 */    14,   15,   16,   17,   18,    2,    3,    4,   50,   24,
 /*   290 */    24,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*   300 */    17,   18,   53,   50,   25,   53,   45,   24,   26,   27,
 /*   310 */    44,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*   320 */    38,   39,   43,   23,   50,   46,   41,   44,   40,   45,
 /*   330 */     2,    3,    4,   44,   55,   56,    8,    9,   10,   11,
 /*   340 */    12,   13,   14,   15,   16,   17,   18,    2,    3,    4,
 /*   350 */    44,    0,   24,    8,    9,   10,   11,   12,   13,   14,
 /*   360 */    15,   16,   17,   18,   45,  113,   85,  113,   87,   24,
 /*   370 */    89,   90,   44,  113,  113,   76,   95,   78,  113,   80,
 /*   380 */    81,   82,   83,   84,  113,   86,  113,  113,  113,  113,
 /*   390 */   113,  110,  113,  113,  113,  113,  113,  113,  113,   54,
 /*   400 */   101,  102,  103,  104,  105,  113,  107,    2,    3,    4,
 /*   410 */   113,  112,  113,    8,    9,   10,   11,   12,   13,   14,
 /*   420 */    15,   16,   17,   18,  113,  113,   76,  113,   78,   24,
 /*   430 */    80,   81,   82,   83,   84,  113,  113,   76,  113,   78,
 /*   440 */   113,   80,   81,   82,   83,   84,  113,  113,  113,  113,
 /*   450 */   113,  113,  102,  103,  104,  105,  106,  107,  113,  113,
 /*   460 */   113,  113,  112,  102,  103,  104,  105,  106,  107,  113,
 /*   470 */   113,  113,  113,  112,  113,  113,   76,  113,   78,  113,
 /*   480 */    80,   81,   82,   83,   84,  113,  113,  113,   76,  113,
 /*   490 */    78,  113,   80,   81,   82,   83,   84,  113,  113,  113,
 /*   500 */   113,  113,  102,  103,  104,  105,  113,  107,  113,  113,
 /*   510 */   113,  113,  112,  113,  102,  103,  104,  105,  113,  107,
 /*   520 */   113,   76,  113,   78,  112,   80,   81,   82,   83,   84,
 /*   530 */   113,  113,   76,  113,   78,  113,   80,   81,   82,   83,
 /*   540 */    84,  113,  113,  113,  113,  113,  113,  102,  103,  104,
 /*   550 */   105,  113,  107,  113,  113,  113,  113,  112,  102,  103,
 /*   560 */   104,  105,  113,  107,  113,   76,  113,   78,  112,   80,
 /*   570 */    81,   82,   83,   84,  113,  113,  113,  113,  113,   76,
 /*   580 */   113,   78,  113,   80,   81,   82,   83,   84,  113,  113,
 /*   590 */   113,  102,  103,  104,  105,  113,  107,  113,  113,  113,
 /*   600 */   113,  112,  113,  113,  113,  102,  103,  104,  105,  113,
 /*   610 */   107,  113,  113,  113,   76,  112,   78,  113,   80,   81,
 /*   620 */    82,   83,   84,  113,  113,   76,  113,   78,  113,   80,
 /*   630 */    81,   82,   83,   84,  113,  113,  113,  113,  113,  113,
 /*   640 */   102,  103,  104,  105,  113,  107,  113,  113,  113,  113,
 /*   650 */   112,  102,  103,  104,  105,  113,  107,  113,   76,  113,
 /*   660 */    78,  112,   80,   81,   82,   83,   84,  113,  113,   76,
 /*   670 */   113,   78,  113,   80,   81,   82,   83,   84,  113,  113,
 /*   680 */   113,  113,  113,  113,  102,  103,  104,  105,  113,  107,
 /*   690 */   113,  113,  113,  113,  112,  102,  103,  104,  105,  113,
 /*   700 */   107,  113,   76,  113,   78,  112,   80,   81,   82,   83,
 /*   710 */    84,  113,  113,  113,  113,  113,  113,   76,  113,   78,
 /*   720 */   113,   80,   81,   82,   83,   84,  113,  113,  102,  103,
 /*   730 */   104,  105,  113,  107,  113,  113,  113,  113,  112,  113,
 /*   740 */   113,  113,  113,  102,  103,  104,  105,  113,  107,  113,
 /*   750 */    76,  113,   78,  112,   80,   81,   82,   83,   84,  113,
 /*   760 */   113,   76,  113,   78,  113,   80,   81,   82,   83,   84,
 /*   770 */   113,  113,  113,  113,  113,  113,  102,  103,  104,  105,
 /*   780 */   113,  107,  113,  113,  113,  113,  112,  102,  103,  104,
 /*   790 */   105,  113,  107,  113,   76,  113,   78,  112,   80,   81,
 /*   800 */    82,   83,   84,  113,  113,   76,  113,   78,  113,   80,
 /*   810 */    81,   82,   83,   84,  113,  113,  113,  113,  113,  113,
 /*   820 */   102,  103,  104,  105,  113,  107,  113,  113,  113,  113,
 /*   830 */   112,  102,  103,  104,  105,  113,  107,  113,   76,  113,
 /*   840 */    78,  112,   80,   81,   82,   83,   84,  113,  113,  113,
 /*   850 */   113,  113,  113,   76,  113,   78,  113,   80,   81,   82,
 /*   860 */    83,   84,  113,  113,  102,  103,  104,  105,  113,  107,
 /*   870 */   113,  113,  113,  113,  112,  113,  113,  113,  113,  102,
 /*   880 */   103,  104,  105,  113,  107,  113,   76,  113,   78,  112,
 /*   890 */    80,   81,   82,   83,   84,  113,  113,   76,  113,   78,
 /*   900 */   113,   80,   81,   82,   83,   84,  113,  113,  113,  113,
 /*   910 */   113,  113,  102,  103,  104,  105,  113,  107,  113,  113,
 /*   920 */   113,  113,  112,  102,  103,  104,  105,  113,  107,  113,
 /*   930 */    76,  113,   78,  112,   80,   81,   82,   83,   84,  113,
 /*   940 */   113,   76,  113,   78,  113,   80,   81,   82,   83,   84,
 /*   950 */   113,  113,  113,  113,  113,  113,  102,  103,  104,  105,
 /*   960 */   113,  107,  113,  113,  113,  113,  112,  102,  103,  104,
 /*   970 */   105,  113,  107,  113,   76,  113,   78,  112,   80,   81,
 /*   980 */    82,   83,   84,  113,  113,  113,  113,  113,  113,   76,
 /*   990 */   113,   78,  113,   80,   81,   82,   83,   84,  113,  113,
 /*  1000 */   102,  103,  104,  105,  113,  107,  113,  113,  113,  113,
 /*  1010 */   112,  113,  113,  113,  113,  102,  103,  104,  105,  113,
 /*  1020 */   107,  113,   76,  113,   78,  112,   80,   81,   82,   83,
 /*  1030 */    84,  113,    4,  113,  113,  113,    8,    9,   10,   11,
 /*  1040 */    12,   13,   14,   15,   16,   17,   18,  113,  102,  103,
 /*  1050 */   104,  105,   24,  107,  113,  113,  113,  113,  112,    8,
 /*  1060 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  1070 */   113,  113,  113,  113,  113,   24,  113,  113,  113,  113,
 /*  1080 */   113,  113,  113,  113,  113,  113,  113,  113,  113,  113,
 /*  1090 */   113,  113,  113,  113,  113,  113,  113,  113,  113,  113,
 /*  1100 */   113,  113,  113,  113,  113,  113,  113,  113,  113,  113,
 /*  1110 */   113,  113,  113,  113,  113,  113,  113,  113,  113,  113,
 /*  1120 */   113,  113,  113,  113,  113,  113,  113,  113,  113,  113,
 /*  1130 */   113,  113,  113,
};
#define YY_SHIFT_COUNT    (116)
#define YY_SHIFT_MIN      (0)
#define YY_SHIFT_MAX      (1051)
static const unsigned short int yy_shift_ofst[] = {
 /*     0 */  1076,    0,    0,    0,  119,  119,   58,  119,  119,  119,
 /*    10 */   119,  119,  119,  119,  119,  119,  119,  119,  119,  119,
 /*    20 */   119,  119,  119,  119,  119,  119,  119,  119,  119,  119,
 /*    30 */   119,  282,  282,  282,  282,  279,  150,   49,  170,   49,
 /*    40 */    15,   15,  176,  176,  176,  176,  176,   28,   15,   15,
 /*    50 */   162,    7,   15,    7,   15,   71,  167,  163,   15,   15,
 /*    60 */    15,  266,  283,  328,  345,  405,  405,  405,  405,  405,
 /*    70 */  1028,  151,  151,  151,  151,  151,  151, 1051,   82,   82,
 /*    80 */    89,  186,  190,  194,  197,  207,  211,  126,  199,  250,
 /*    90 */   263,  215,  227,  221,  228,  238,  265,  265,  265,  265,
 /*   100 */   265,  265,  265,  265,  249,  252,  253,  261,  300,  284,
 /*   110 */   274,  289,  306,  285,  288,  319,  351,
};
#define YY_REDUCE_COUNT (60)
#define YY_REDUCE_MIN   (-91)
#define YY_REDUCE_MAX   (946)
static const short yy_reduce_ofst[] = {
 /*     0 */   -73,  120,  153,  299,  350,  361,  400,  412,  445,  456,
 /*    10 */   489,  503,  538,  549,  582,  593,  626,  641,  674,  685,
 /*    20 */   718,  729,  762,  777,  810,  821,  854,  865,  898,  913,
 /*    30 */   946,  -72,  -35,   32,  129,  281,  -79,  -63,   22,  -26,
 /*    40 */   -60,   35,    3,   26,   30,   64,   97,  104,  -58,  132,
 /*    50 */   -85,  -91,  -71,  -86,  -38,   42,  125,  136,  144,  161,
 /*    60 */   166,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   323,  376,  376,  377,  420,  420,  319,  319,  319,  319,
 /*    10 */   319,  319,  319,  319,  319,  319,  319,  319,  319,  319,
 /*    20 */   319,  319,  319,  319,  319,  319,  319,  319,  319,  319,
 /*    30 */   319,  319,  319,  319,  319,  319,  371,  319,  350,  319,
 /*    40 */   422,  319,  350,  350,  350,  350,  350,  319,  319,  319,
 /*    50 */   319,  389,  319,  389,  319,  375,  319,  319,  319,  319,
 /*    60 */   319,  319,  319,  319,  319,  387,  386,  418,  425,  419,
 /*    70 */   401,  412,  411,  410,  409,  408,  407,  400,  403,  402,
 /*    80 */   319,  319,  319,  319,  334,  319,  319,  319,  319,  319,
 /*    90 */   319,  319,  319,  319,  319,  319,  417,  416,  415,  414,
 /*   100 */   413,  406,  405,  404,  331,  338,  319,  319,  351,  319,
 /*   110 */   319,  319,  319,  319,  336,  319,  319,
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

#if defined(YYCOVERAGE) || !defined(NDEBUG)
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *const yyTokenName[] = { 
  /*    0 */ "$",
  /*    1 */ "ASSIGN",
  /*    2 */ "AS",
  /*    3 */ "OR",
  /*    4 */ "AND",
  /*    5 */ "BIT_OR",
  /*    6 */ "BIT_XOR",
  /*    7 */ "BIT_AND",
  /*    8 */ "EQ",
  /*    9 */ "NE",
  /*   10 */ "GT",
  /*   11 */ "LT",
  /*   12 */ "GE",
  /*   13 */ "LE",
  /*   14 */ "PLUS",
  /*   15 */ "MINUS",
  /*   16 */ "MULT",
  /*   17 */ "DIV",
  /*   18 */ "MOD",
  /*   19 */ "NEGATE",
  /*   20 */ "BIT_NOT",
  /*   21 */ "NOT",
  /*   22 */ "DEREF",
  /*   23 */ "REFERENCE",
  /*   24 */ "DOT",
  /*   25 */ "END",
  /*   26 */ "IDENTIFIER_MULTI",
  /*   27 */ "IDENTIFIER",
  /*   28 */ "SELF",
  /*   29 */ "TYPE_BOOL",
  /*   30 */ "TYPE_INT8",
  /*   31 */ "TYPE_UINT8",
  /*   32 */ "TYPE_INT16",
  /*   33 */ "TYPE_UINT16",
  /*   34 */ "TYPE_INT32",
  /*   35 */ "TYPE_UINT32",
  /*   36 */ "TYPE_INT64",
  /*   37 */ "TYPE_UINT64",
  /*   38 */ "TYPE_FLOAT",
  /*   39 */ "TYPE_DOUBLE",
  /*   40 */ "ARRAY_BEGIN",
  /*   41 */ "ARRAY_END",
  /*   42 */ "MUT",
  /*   43 */ "IMPORT",
  /*   44 */ "SEMI_COLON",
  /*   45 */ "COLON",
  /*   46 */ "TYPE",
  /*   47 */ "ALIAS",
  /*   48 */ "FROM",
  /*   49 */ "STRUCT",
  /*   50 */ "SCOPE_BEGIN",
  /*   51 */ "SCOPE_END",
  /*   52 */ "COMMA",
  /*   53 */ "PAR_BEGIN",
  /*   54 */ "PAR_END",
  /*   55 */ "FUNCTION",
  /*   56 */ "IMPL",
  /*   57 */ "RETURN",
  /*   58 */ "VAR",
  /*   59 */ "LET",
  /*   60 */ "INTEGER_LITERAL",
  /*   61 */ "INTEGER_LITERAL_I8",
  /*   62 */ "INTEGER_LITERAL_I16",
  /*   63 */ "INTEGER_LITERAL_I32",
  /*   64 */ "INTEGER_LITERAL_I64",
  /*   65 */ "INTEGER_LITERAL_U8",
  /*   66 */ "INTEGER_LITERAL_U16",
  /*   67 */ "INTEGER_LITERAL_U32",
  /*   68 */ "INTEGER_LITERAL_U64",
  /*   69 */ "FLOAT_LITERAL",
  /*   70 */ "FLOAT_LITERAL_32",
  /*   71 */ "FLOAT_LITERAL_64",
  /*   72 */ "BOOL_LITERAL",
  /*   73 */ "error",
  /*   74 */ "decls",
  /*   75 */ "module",
  /*   76 */ "type_builtin",
  /*   77 */ "type_qualified",
  /*   78 */ "type_specifier",
  /*   79 */ "qualifier",
  /*   80 */ "identifier",
  /*   81 */ "identifier_complex",
  /*   82 */ "identifier_simple",
  /*   83 */ "self",
  /*   84 */ "identifier_or_self",
  /*   85 */ "decl",
  /*   86 */ "decl_var",
  /*   87 */ "decl_alias",
  /*   88 */ "decl_import_module",
  /*   89 */ "decl_import",
  /*   90 */ "decl_function",
  /*   91 */ "function_return_decl",
  /*   92 */ "function_args_decl",
  /*   93 */ "function_arg_decl",
  /*   94 */ "function_param_list",
  /*   95 */ "decl_struct",
  /*   96 */ "struct_decl_var",
  /*   97 */ "struct_decl_vars",
  /*   98 */ "var_type_spec",
  /*   99 */ "statement_list",
  /*  100 */ "statement_list_or_empty",
  /*  101 */ "statement",
  /*  102 */ "expression",
  /*  103 */ "unaryexp",
  /*  104 */ "binaryexp",
  /*  105 */ "literal",
  /*  106 */ "function_call_args",
  /*  107 */ "struct_init",
  /*  108 */ "struct_member_init_list",
  /*  109 */ "struct_member_init",
  /*  110 */ "decl_type_functions_scope",
  /*  111 */ "decl_type_function_list",
  /*  112 */ "type_array",
};
#endif /* defined(YYCOVERAGE) || !defined(NDEBUG) */

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
 /*   6 */ "decl ::= decl_import",
 /*   7 */ "identifier_complex ::= IDENTIFIER_MULTI",
 /*   8 */ "identifier_simple ::= IDENTIFIER",
 /*   9 */ "identifier ::= identifier_simple",
 /*  10 */ "identifier ::= identifier_complex",
 /*  11 */ "self ::= SELF",
 /*  12 */ "identifier_or_self ::= identifier_simple",
 /*  13 */ "identifier_or_self ::= self",
 /*  14 */ "type_specifier ::= type_builtin",
 /*  15 */ "type_specifier ::= type_array",
 /*  16 */ "type_specifier ::= identifier",
 /*  17 */ "type_builtin ::= TYPE_BOOL",
 /*  18 */ "type_builtin ::= TYPE_INT8",
 /*  19 */ "type_builtin ::= TYPE_UINT8",
 /*  20 */ "type_builtin ::= TYPE_INT16",
 /*  21 */ "type_builtin ::= TYPE_UINT16",
 /*  22 */ "type_builtin ::= TYPE_INT32",
 /*  23 */ "type_builtin ::= TYPE_UINT32",
 /*  24 */ "type_builtin ::= TYPE_INT64",
 /*  25 */ "type_builtin ::= TYPE_UINT64",
 /*  26 */ "type_builtin ::= TYPE_FLOAT",
 /*  27 */ "type_builtin ::= TYPE_DOUBLE",
 /*  28 */ "qualifier ::=",
 /*  29 */ "qualifier ::= MUT",
 /*  30 */ "qualifier ::= REFERENCE",
 /*  31 */ "qualifier ::= MUT REFERENCE",
 /*  32 */ "type_qualified ::= qualifier type_specifier",
 /*  33 */ "decl_import ::= IMPORT decl_import_module SEMI_COLON",
 /*  34 */ "decl_import_module ::= decl_import_module COLON COLON identifier_simple",
 /*  35 */ "decl_import_module ::= identifier",
 /*  36 */ "decl_alias ::= TYPE identifier_simple ALIAS type_specifier SEMI_COLON",
 /*  37 */ "decl_alias ::= TYPE identifier_simple FROM type_specifier SEMI_COLON",
 /*  38 */ "decl_struct ::= TYPE identifier_simple STRUCT SCOPE_BEGIN struct_decl_vars SCOPE_END",
 /*  39 */ "struct_decl_vars ::= struct_decl_vars COMMA struct_decl_var",
 /*  40 */ "struct_decl_vars ::= struct_decl_var",
 /*  41 */ "struct_decl_var ::= identifier_simple COLON type_qualified",
 /*  42 */ "function_param_list ::= PAR_BEGIN function_args_decl PAR_END",
 /*  43 */ "decl_function ::= FUNCTION identifier_simple function_param_list function_return_decl SCOPE_BEGIN statement_list_or_empty SCOPE_END",
 /*  44 */ "decl_type_functions_scope ::= IMPL type_specifier SCOPE_BEGIN decl_type_function_list SCOPE_END",
 /*  45 */ "decl_type_function_list ::= decl_type_function_list decl_function",
 /*  46 */ "decl_type_function_list ::= decl_function",
 /*  47 */ "function_args_decl ::= function_args_decl COMMA function_arg_decl",
 /*  48 */ "function_args_decl ::= function_arg_decl",
 /*  49 */ "function_args_decl ::=",
 /*  50 */ "function_arg_decl ::= identifier_simple COLON type_qualified",
 /*  51 */ "function_arg_decl ::= qualifier self",
 /*  52 */ "function_return_decl ::= COLON type_qualified",
 /*  53 */ "function_return_decl ::=",
 /*  54 */ "statement_list_or_empty ::=",
 /*  55 */ "statement_list_or_empty ::= statement_list",
 /*  56 */ "statement_list ::= statement_list statement",
 /*  57 */ "statement_list ::= statement",
 /*  58 */ "statement ::= expression ASSIGN expression SEMI_COLON",
 /*  59 */ "statement ::= decl_var SEMI_COLON",
 /*  60 */ "statement ::= expression SEMI_COLON",
 /*  61 */ "statement ::= RETURN expression SEMI_COLON",
 /*  62 */ "statement ::= RETURN SEMI_COLON",
 /*  63 */ "statement ::= SCOPE_BEGIN statement_list_or_empty SCOPE_END",
 /*  64 */ "decl_var ::= VAR identifier_simple var_type_spec ASSIGN expression",
 /*  65 */ "decl_var ::= LET identifier_simple var_type_spec ASSIGN expression",
 /*  66 */ "var_type_spec ::= COLON type_qualified",
 /*  67 */ "var_type_spec ::=",
 /*  68 */ "expression ::= PAR_BEGIN expression PAR_END",
 /*  69 */ "expression ::= literal",
 /*  70 */ "expression ::= unaryexp",
 /*  71 */ "expression ::= binaryexp",
 /*  72 */ "expression ::= struct_init",
 /*  73 */ "expression ::= expression AS type_qualified",
 /*  74 */ "expression ::= identifier_or_self",
 /*  75 */ "expression ::= expression DOT identifier",
 /*  76 */ "expression ::= identifier PAR_BEGIN function_call_args PAR_END",
 /*  77 */ "expression ::= expression DOT identifier_simple PAR_BEGIN function_call_args PAR_END",
 /*  78 */ "binaryexp ::= expression AND expression",
 /*  79 */ "binaryexp ::= expression OR expression",
 /*  80 */ "binaryexp ::= expression PLUS expression",
 /*  81 */ "binaryexp ::= expression MINUS expression",
 /*  82 */ "binaryexp ::= expression MULT expression",
 /*  83 */ "binaryexp ::= expression DIV expression",
 /*  84 */ "binaryexp ::= expression MOD expression",
 /*  85 */ "binaryexp ::= expression EQ expression",
 /*  86 */ "binaryexp ::= expression NE expression",
 /*  87 */ "binaryexp ::= expression LE expression",
 /*  88 */ "binaryexp ::= expression LT expression",
 /*  89 */ "binaryexp ::= expression GE expression",
 /*  90 */ "binaryexp ::= expression GT expression",
 /*  91 */ "unaryexp ::= NOT expression",
 /*  92 */ "unaryexp ::= BIT_NOT expression",
 /*  93 */ "unaryexp ::= MINUS expression",
 /*  94 */ "unaryexp ::= REFERENCE expression",
 /*  95 */ "unaryexp ::= MULT expression",
 /*  96 */ "function_call_args ::= function_call_args COMMA expression",
 /*  97 */ "function_call_args ::= expression",
 /*  98 */ "function_call_args ::=",
 /*  99 */ "struct_init ::= type_specifier SCOPE_BEGIN struct_member_init_list SCOPE_END",
 /* 100 */ "struct_member_init_list ::=",
 /* 101 */ "struct_member_init_list ::= struct_member_init_list COMMA struct_member_init",
 /* 102 */ "struct_member_init_list ::= struct_member_init",
 /* 103 */ "struct_member_init ::= identifier_simple COLON expression",
 /* 104 */ "literal ::= INTEGER_LITERAL",
 /* 105 */ "literal ::= INTEGER_LITERAL_I8",
 /* 106 */ "literal ::= INTEGER_LITERAL_I16",
 /* 107 */ "literal ::= INTEGER_LITERAL_I32",
 /* 108 */ "literal ::= INTEGER_LITERAL_I64",
 /* 109 */ "literal ::= INTEGER_LITERAL_U8",
 /* 110 */ "literal ::= INTEGER_LITERAL_U16",
 /* 111 */ "literal ::= INTEGER_LITERAL_U32",
 /* 112 */ "literal ::= INTEGER_LITERAL_U64",
 /* 113 */ "literal ::= FLOAT_LITERAL",
 /* 114 */ "literal ::= FLOAT_LITERAL_32",
 /* 115 */ "literal ::= FLOAT_LITERAL_64",
 /* 116 */ "literal ::= BOOL_LITERAL",
 /* 117 */ "module ::= decls END",
 /* 118 */ "type_array ::= type_builtin ARRAY_BEGIN ARRAY_END",
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

/* This array of booleans keeps track of the parser statement
** coverage.  The element yycoverage[X][Y] is set when the parser
** is in state X and has a lookahead token Y.  In a well-tested
** systems, every element of this matrix should end up being set.
*/
#if defined(YYCOVERAGE)
static unsigned char yycoverage[YYNSTATE][YYNTOKEN];
#endif

/*
** Write into out a description of every state/lookahead combination that
**
**   (1)  has not been used by the parser, and
**   (2)  is not a syntax error.
**
** Return the number of missed state/lookahead combinations.
*/
#if defined(YYCOVERAGE)
int YALSTParserCoverage(FILE *out){
  int stateno, iLookAhead, i;
  int nMissed = 0;
  for(stateno=0; stateno<YYNSTATE; stateno++){
    i = yy_shift_ofst[stateno];
    for(iLookAhead=0; iLookAhead<YYNTOKEN; iLookAhead++){
      if( yy_lookahead[i+iLookAhead]!=iLookAhead ) continue;
      if( yycoverage[stateno][iLookAhead]==0 ) nMissed++;
      if( out ){
        fprintf(out,"State %d lookahead %s %s\n", stateno,
                yyTokenName[iLookAhead],
                yycoverage[stateno][iLookAhead] ? "ok" : "missed");
      }
    }
  }
  return nMissed;
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
 
  if( stateno>YY_MAX_SHIFT ) return stateno;
  assert( stateno <= YY_SHIFT_COUNT );
#if defined(YYCOVERAGE)
  yycoverage[stateno][iLookAhead] = 1;
#endif
  do{
    i = yy_shift_ofst[stateno];
    assert( i>=0 && i+YYNTOKEN<=sizeof(yy_lookahead)/sizeof(yy_lookahead[0]) );
    assert( iLookAhead!=YYNOCODE );
    assert( iLookAhead < YYNTOKEN );
    i += iLookAhead;
    if( yy_lookahead[i]!=iLookAhead ){
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
static void yyTraceShift(yyParser *yypParser, int yyNewState, const char *zTag){
  if( yyTraceFILE ){
    if( yyNewState<YYNSTATE ){
      fprintf(yyTraceFILE,"%s%s '%s', go to state %d\n",
         yyTracePrompt, zTag, yyTokenName[yypParser->yytos->major],
         yyNewState);
    }else{
      fprintf(yyTraceFILE,"%s%s '%s', pending reduce %d\n",
         yyTracePrompt, zTag, yyTokenName[yypParser->yytos->major],
         yyNewState - YY_MIN_REDUCE);
    }
  }
}
#else
# define yyTraceShift(X,Y,Z)
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
  yyTraceShift(yypParser, yyNewState, "Shift");
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;       /* Symbol on the left-hand side of the rule */
  signed char nrhs;     /* Negative of the number of RHS symbols in the rule */
} yyRuleInfo[] = {
  {   74,   -2 }, /* (0) decls ::= decls decl */
  {   74,    0 }, /* (1) decls ::= */
  {   85,   -1 }, /* (2) decl ::= decl_alias */
  {   85,   -1 }, /* (3) decl ::= decl_struct */
  {   85,   -1 }, /* (4) decl ::= decl_function */
  {   85,   -1 }, /* (5) decl ::= decl_type_functions_scope */
  {   85,   -1 }, /* (6) decl ::= decl_import */
  {   81,   -1 }, /* (7) identifier_complex ::= IDENTIFIER_MULTI */
  {   82,   -1 }, /* (8) identifier_simple ::= IDENTIFIER */
  {   80,   -1 }, /* (9) identifier ::= identifier_simple */
  {   80,   -1 }, /* (10) identifier ::= identifier_complex */
  {   83,   -1 }, /* (11) self ::= SELF */
  {   84,   -1 }, /* (12) identifier_or_self ::= identifier_simple */
  {   84,   -1 }, /* (13) identifier_or_self ::= self */
  {   78,   -1 }, /* (14) type_specifier ::= type_builtin */
  {   78,   -1 }, /* (15) type_specifier ::= type_array */
  {   78,   -1 }, /* (16) type_specifier ::= identifier */
  {   76,   -1 }, /* (17) type_builtin ::= TYPE_BOOL */
  {   76,   -1 }, /* (18) type_builtin ::= TYPE_INT8 */
  {   76,   -1 }, /* (19) type_builtin ::= TYPE_UINT8 */
  {   76,   -1 }, /* (20) type_builtin ::= TYPE_INT16 */
  {   76,   -1 }, /* (21) type_builtin ::= TYPE_UINT16 */
  {   76,   -1 }, /* (22) type_builtin ::= TYPE_INT32 */
  {   76,   -1 }, /* (23) type_builtin ::= TYPE_UINT32 */
  {   76,   -1 }, /* (24) type_builtin ::= TYPE_INT64 */
  {   76,   -1 }, /* (25) type_builtin ::= TYPE_UINT64 */
  {   76,   -1 }, /* (26) type_builtin ::= TYPE_FLOAT */
  {   76,   -1 }, /* (27) type_builtin ::= TYPE_DOUBLE */
  {   79,    0 }, /* (28) qualifier ::= */
  {   79,   -1 }, /* (29) qualifier ::= MUT */
  {   79,   -1 }, /* (30) qualifier ::= REFERENCE */
  {   79,   -2 }, /* (31) qualifier ::= MUT REFERENCE */
  {   77,   -2 }, /* (32) type_qualified ::= qualifier type_specifier */
  {   89,   -3 }, /* (33) decl_import ::= IMPORT decl_import_module SEMI_COLON */
  {   88,   -4 }, /* (34) decl_import_module ::= decl_import_module COLON COLON identifier_simple */
  {   88,   -1 }, /* (35) decl_import_module ::= identifier */
  {   87,   -5 }, /* (36) decl_alias ::= TYPE identifier_simple ALIAS type_specifier SEMI_COLON */
  {   87,   -5 }, /* (37) decl_alias ::= TYPE identifier_simple FROM type_specifier SEMI_COLON */
  {   95,   -6 }, /* (38) decl_struct ::= TYPE identifier_simple STRUCT SCOPE_BEGIN struct_decl_vars SCOPE_END */
  {   97,   -3 }, /* (39) struct_decl_vars ::= struct_decl_vars COMMA struct_decl_var */
  {   97,   -1 }, /* (40) struct_decl_vars ::= struct_decl_var */
  {   96,   -3 }, /* (41) struct_decl_var ::= identifier_simple COLON type_qualified */
  {   94,   -3 }, /* (42) function_param_list ::= PAR_BEGIN function_args_decl PAR_END */
  {   90,   -7 }, /* (43) decl_function ::= FUNCTION identifier_simple function_param_list function_return_decl SCOPE_BEGIN statement_list_or_empty SCOPE_END */
  {  110,   -5 }, /* (44) decl_type_functions_scope ::= IMPL type_specifier SCOPE_BEGIN decl_type_function_list SCOPE_END */
  {  111,   -2 }, /* (45) decl_type_function_list ::= decl_type_function_list decl_function */
  {  111,   -1 }, /* (46) decl_type_function_list ::= decl_function */
  {   92,   -3 }, /* (47) function_args_decl ::= function_args_decl COMMA function_arg_decl */
  {   92,   -1 }, /* (48) function_args_decl ::= function_arg_decl */
  {   92,    0 }, /* (49) function_args_decl ::= */
  {   93,   -3 }, /* (50) function_arg_decl ::= identifier_simple COLON type_qualified */
  {   93,   -2 }, /* (51) function_arg_decl ::= qualifier self */
  {   91,   -2 }, /* (52) function_return_decl ::= COLON type_qualified */
  {   91,    0 }, /* (53) function_return_decl ::= */
  {  100,    0 }, /* (54) statement_list_or_empty ::= */
  {  100,   -1 }, /* (55) statement_list_or_empty ::= statement_list */
  {   99,   -2 }, /* (56) statement_list ::= statement_list statement */
  {   99,   -1 }, /* (57) statement_list ::= statement */
  {  101,   -4 }, /* (58) statement ::= expression ASSIGN expression SEMI_COLON */
  {  101,   -2 }, /* (59) statement ::= decl_var SEMI_COLON */
  {  101,   -2 }, /* (60) statement ::= expression SEMI_COLON */
  {  101,   -3 }, /* (61) statement ::= RETURN expression SEMI_COLON */
  {  101,   -2 }, /* (62) statement ::= RETURN SEMI_COLON */
  {  101,   -3 }, /* (63) statement ::= SCOPE_BEGIN statement_list_or_empty SCOPE_END */
  {   86,   -5 }, /* (64) decl_var ::= VAR identifier_simple var_type_spec ASSIGN expression */
  {   86,   -5 }, /* (65) decl_var ::= LET identifier_simple var_type_spec ASSIGN expression */
  {   98,   -2 }, /* (66) var_type_spec ::= COLON type_qualified */
  {   98,    0 }, /* (67) var_type_spec ::= */
  {  102,   -3 }, /* (68) expression ::= PAR_BEGIN expression PAR_END */
  {  102,   -1 }, /* (69) expression ::= literal */
  {  102,   -1 }, /* (70) expression ::= unaryexp */
  {  102,   -1 }, /* (71) expression ::= binaryexp */
  {  102,   -1 }, /* (72) expression ::= struct_init */
  {  102,   -3 }, /* (73) expression ::= expression AS type_qualified */
  {  102,   -1 }, /* (74) expression ::= identifier_or_self */
  {  102,   -3 }, /* (75) expression ::= expression DOT identifier */
  {  102,   -4 }, /* (76) expression ::= identifier PAR_BEGIN function_call_args PAR_END */
  {  102,   -6 }, /* (77) expression ::= expression DOT identifier_simple PAR_BEGIN function_call_args PAR_END */
  {  104,   -3 }, /* (78) binaryexp ::= expression AND expression */
  {  104,   -3 }, /* (79) binaryexp ::= expression OR expression */
  {  104,   -3 }, /* (80) binaryexp ::= expression PLUS expression */
  {  104,   -3 }, /* (81) binaryexp ::= expression MINUS expression */
  {  104,   -3 }, /* (82) binaryexp ::= expression MULT expression */
  {  104,   -3 }, /* (83) binaryexp ::= expression DIV expression */
  {  104,   -3 }, /* (84) binaryexp ::= expression MOD expression */
  {  104,   -3 }, /* (85) binaryexp ::= expression EQ expression */
  {  104,   -3 }, /* (86) binaryexp ::= expression NE expression */
  {  104,   -3 }, /* (87) binaryexp ::= expression LE expression */
  {  104,   -3 }, /* (88) binaryexp ::= expression LT expression */
  {  104,   -3 }, /* (89) binaryexp ::= expression GE expression */
  {  104,   -3 }, /* (90) binaryexp ::= expression GT expression */
  {  103,   -2 }, /* (91) unaryexp ::= NOT expression */
  {  103,   -2 }, /* (92) unaryexp ::= BIT_NOT expression */
  {  103,   -2 }, /* (93) unaryexp ::= MINUS expression */
  {  103,   -2 }, /* (94) unaryexp ::= REFERENCE expression */
  {  103,   -2 }, /* (95) unaryexp ::= MULT expression */
  {  106,   -3 }, /* (96) function_call_args ::= function_call_args COMMA expression */
  {  106,   -1 }, /* (97) function_call_args ::= expression */
  {  106,    0 }, /* (98) function_call_args ::= */
  {  107,   -4 }, /* (99) struct_init ::= type_specifier SCOPE_BEGIN struct_member_init_list SCOPE_END */
  {  108,    0 }, /* (100) struct_member_init_list ::= */
  {  108,   -3 }, /* (101) struct_member_init_list ::= struct_member_init_list COMMA struct_member_init */
  {  108,   -1 }, /* (102) struct_member_init_list ::= struct_member_init */
  {  109,   -3 }, /* (103) struct_member_init ::= identifier_simple COLON expression */
  {  105,   -1 }, /* (104) literal ::= INTEGER_LITERAL */
  {  105,   -1 }, /* (105) literal ::= INTEGER_LITERAL_I8 */
  {  105,   -1 }, /* (106) literal ::= INTEGER_LITERAL_I16 */
  {  105,   -1 }, /* (107) literal ::= INTEGER_LITERAL_I32 */
  {  105,   -1 }, /* (108) literal ::= INTEGER_LITERAL_I64 */
  {  105,   -1 }, /* (109) literal ::= INTEGER_LITERAL_U8 */
  {  105,   -1 }, /* (110) literal ::= INTEGER_LITERAL_U16 */
  {  105,   -1 }, /* (111) literal ::= INTEGER_LITERAL_U32 */
  {  105,   -1 }, /* (112) literal ::= INTEGER_LITERAL_U64 */
  {  105,   -1 }, /* (113) literal ::= FLOAT_LITERAL */
  {  105,   -1 }, /* (114) literal ::= FLOAT_LITERAL_32 */
  {  105,   -1 }, /* (115) literal ::= FLOAT_LITERAL_64 */
  {  105,   -1 }, /* (116) literal ::= BOOL_LITERAL */
  {   75,   -2 }, /* (117) module ::= decls END */
  {  112,   -3 }, /* (118) type_array ::= type_builtin ARRAY_BEGIN ARRAY_END */
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
**
** The yyLookahead and yyLookaheadToken parameters provide reduce actions
** access to the lookahead token (if any).  The yyLookahead will be YYNOCODE
** if the lookahead token has already been consumed.  As this procedure is
** only called from one place, optimizing compilers will in-line it, which
** means that the extra parameters have no performance impact.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  unsigned int yyruleno,       /* Number of the rule by which to reduce */
  int yyLookahead,             /* Lookahead token, or YYNOCODE if none */
  YALSTParserTOKENTYPE yyLookaheadToken  /* Value of the lookahead token */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  YALSTParserARG_FETCH;
  (void)yyLookahead;
  (void)yyLookaheadToken;
  yymsp = yypParser->yytos;
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    yysize = yyRuleInfo[yyruleno].nrhs;
    if( yysize ){
      fprintf(yyTraceFILE, "%sReduce %d [%s], go to state %d.\n",
        yyTracePrompt,
        yyruleno, yyRuleName[yyruleno], yymsp[yysize].stateno);
    }else{
      fprintf(yyTraceFILE, "%sReduce %d [%s].\n",
        yyTracePrompt, yyruleno, yyRuleName[yyruleno]);
    }
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
#line 128 "stparserimpl.lemon"
{
    yylhsminor.yy16 =yymsp[-1].minor.yy16; yymsp[-1].minor.yy16->addDecl(yymsp[0].minor.yy78);
}
#line 1525 "stparserimpl.c"
  yymsp[-1].minor.yy16 = yylhsminor.yy16;
        break;
      case 1: /* decls ::= */
#line 131 "stparserimpl.lemon"
{
        yymsp[1].minor.yy16 =  pParser->getDeclModule();
}
#line 1533 "stparserimpl.c"
        break;
      case 2: /* decl ::= decl_alias */
#line 135 "stparserimpl.lemon"
{
    yylhsminor.yy78 = yymsp[0].minor.yy210;
}
#line 1540 "stparserimpl.c"
  yymsp[0].minor.yy78 = yylhsminor.yy78;
        break;
      case 3: /* decl ::= decl_struct */
#line 138 "stparserimpl.lemon"
{
    yylhsminor.yy78 = yymsp[0].minor.yy65;
}
#line 1548 "stparserimpl.c"
  yymsp[0].minor.yy78 = yylhsminor.yy78;
        break;
      case 4: /* decl ::= decl_function */
#line 141 "stparserimpl.lemon"
{
    yylhsminor.yy78 = yymsp[0].minor.yy128;
}
#line 1556 "stparserimpl.c"
  yymsp[0].minor.yy78 = yylhsminor.yy78;
        break;
      case 5: /* decl ::= decl_type_functions_scope */
#line 144 "stparserimpl.lemon"
{
    yylhsminor.yy78 = yymsp[0].minor.yy42;
}
#line 1564 "stparserimpl.c"
  yymsp[0].minor.yy78 = yylhsminor.yy78;
        break;
      case 6: /* decl ::= decl_import */
#line 148 "stparserimpl.lemon"
{
    yylhsminor.yy78 = yymsp[0].minor.yy41;
}
#line 1572 "stparserimpl.c"
  yymsp[0].minor.yy78 = yylhsminor.yy78;
        break;
      case 7: /* identifier_complex ::= IDENTIFIER_MULTI */
#line 154 "stparserimpl.lemon"
{
    yylhsminor.yy173 = pParser->createNode<yal::frontend::STIdentifier>(yymsp[0].minor.yy0.tokenStr, true);
    auto srcInfo = CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy173->setSourceInfo(srcInfo);
}
#line 1582 "stparserimpl.c"
  yymsp[0].minor.yy173 = yylhsminor.yy173;
        break;
      case 8: /* identifier_simple ::= IDENTIFIER */
      case 11: /* self ::= SELF */ yytestcase(yyruleno==11);
#line 160 "stparserimpl.lemon"
{
        yylhsminor.yy173 = pParser->createNode<yal::frontend::STIdentifier>(yymsp[0].minor.yy0.tokenStr, false);
        auto srcInfo = CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy173->setSourceInfo(srcInfo);
}
#line 1593 "stparserimpl.c"
  yymsp[0].minor.yy173 = yylhsminor.yy173;
        break;
      case 9: /* identifier ::= identifier_simple */
      case 10: /* identifier ::= identifier_complex */ yytestcase(yyruleno==10);
#line 166 "stparserimpl.lemon"
{
        yylhsminor.yy173 = yymsp[0].minor.yy173;
}
#line 1602 "stparserimpl.c"
  yymsp[0].minor.yy173 = yylhsminor.yy173;
        break;
      case 12: /* identifier_or_self ::= identifier_simple */
      case 13: /* identifier_or_self ::= self */ yytestcase(yyruleno==13);
#line 180 "stparserimpl.lemon"
{yylhsminor.yy173=yymsp[0].minor.yy173;}
#line 1609 "stparserimpl.c"
  yymsp[0].minor.yy173 = yylhsminor.yy173;
        break;
      case 14: /* type_specifier ::= type_builtin */
#line 183 "stparserimpl.lemon"
{
        yylhsminor.yy82=yymsp[0].minor.yy82;
}
#line 1617 "stparserimpl.c"
  yymsp[0].minor.yy82 = yylhsminor.yy82;
        break;
      case 15: /* type_specifier ::= type_array */
#line 187 "stparserimpl.lemon"
{yymsp[0].minor.yy82 = nullptr;}
#line 1623 "stparserimpl.c"
        break;
      case 16: /* type_specifier ::= identifier */
#line 188 "stparserimpl.lemon"
{
    yylhsminor.yy82 = pParser->createNode<yal::frontend::STType>(yymsp[0].minor.yy173);
    yylhsminor.yy82->setSourceInfo(yymsp[0].minor.yy173->getSourceInfo());
}
#line 1631 "stparserimpl.c"
  yymsp[0].minor.yy82 = yylhsminor.yy82;
        break;
      case 17: /* type_builtin ::= TYPE_BOOL */
#line 193 "stparserimpl.lemon"
{
    yylhsminor.yy82 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Bool);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy82->setSourceInfo(srcInfo);
    }
#line 1641 "stparserimpl.c"
  yymsp[0].minor.yy82 = yylhsminor.yy82;
        break;
      case 18: /* type_builtin ::= TYPE_INT8 */
#line 198 "stparserimpl.lemon"
{
        yylhsminor.yy82 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int8);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy82->setSourceInfo(srcInfo);
    }
#line 1651 "stparserimpl.c"
  yymsp[0].minor.yy82 = yylhsminor.yy82;
        break;
      case 19: /* type_builtin ::= TYPE_UINT8 */
#line 203 "stparserimpl.lemon"
{
        yylhsminor.yy82 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt8);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy82->setSourceInfo(srcInfo);
    }
#line 1661 "stparserimpl.c"
  yymsp[0].minor.yy82 = yylhsminor.yy82;
        break;
      case 20: /* type_builtin ::= TYPE_INT16 */
#line 208 "stparserimpl.lemon"
{
        yylhsminor.yy82 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int16);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy82->setSourceInfo(srcInfo);
    }
#line 1671 "stparserimpl.c"
  yymsp[0].minor.yy82 = yylhsminor.yy82;
        break;
      case 21: /* type_builtin ::= TYPE_UINT16 */
#line 213 "stparserimpl.lemon"
{
        yylhsminor.yy82 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt16);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy82->setSourceInfo(srcInfo);
    }
#line 1681 "stparserimpl.c"
  yymsp[0].minor.yy82 = yylhsminor.yy82;
        break;
      case 22: /* type_builtin ::= TYPE_INT32 */
#line 218 "stparserimpl.lemon"
{
        yylhsminor.yy82 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int32);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy82->setSourceInfo(srcInfo);
    }
#line 1691 "stparserimpl.c"
  yymsp[0].minor.yy82 = yylhsminor.yy82;
        break;
      case 23: /* type_builtin ::= TYPE_UINT32 */
#line 223 "stparserimpl.lemon"
{
        yylhsminor.yy82 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt32);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy82->setSourceInfo(srcInfo);
    }
#line 1701 "stparserimpl.c"
  yymsp[0].minor.yy82 = yylhsminor.yy82;
        break;
      case 24: /* type_builtin ::= TYPE_INT64 */
#line 228 "stparserimpl.lemon"
{
        yylhsminor.yy82 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int64);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy82->setSourceInfo(srcInfo);
    }
#line 1711 "stparserimpl.c"
  yymsp[0].minor.yy82 = yylhsminor.yy82;
        break;
      case 25: /* type_builtin ::= TYPE_UINT64 */
#line 233 "stparserimpl.lemon"
{
        yylhsminor.yy82 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt64);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy82->setSourceInfo(srcInfo);
    }
#line 1721 "stparserimpl.c"
  yymsp[0].minor.yy82 = yylhsminor.yy82;
        break;
      case 26: /* type_builtin ::= TYPE_FLOAT */
#line 238 "stparserimpl.lemon"
{
        yylhsminor.yy82 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Float32);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy82->setSourceInfo(srcInfo);
    }
#line 1731 "stparserimpl.c"
  yymsp[0].minor.yy82 = yylhsminor.yy82;
        break;
      case 27: /* type_builtin ::= TYPE_DOUBLE */
#line 243 "stparserimpl.lemon"
{
        yylhsminor.yy82 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Float64);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy82->setSourceInfo(srcInfo);
    }
#line 1741 "stparserimpl.c"
  yymsp[0].minor.yy82 = yylhsminor.yy82;
        break;
      case 28: /* qualifier ::= */
#line 251 "stparserimpl.lemon"
{yymsp[1].minor.yy175 = 0;}
#line 1747 "stparserimpl.c"
        break;
      case 29: /* qualifier ::= MUT */
#line 252 "stparserimpl.lemon"
{yymsp[0].minor.yy175 = yal::frontend::STQualType::Qualifiers::kQualMutable;}
#line 1752 "stparserimpl.c"
        break;
      case 30: /* qualifier ::= REFERENCE */
#line 253 "stparserimpl.lemon"
{yymsp[0].minor.yy175 = yal::frontend::STQualType::Qualifiers::kQualReference;}
#line 1757 "stparserimpl.c"
        break;
      case 31: /* qualifier ::= MUT REFERENCE */
#line 254 "stparserimpl.lemon"
{
        yymsp[-1].minor.yy175 = yal::frontend::STQualType::Qualifiers::kQualReference | yal::frontend::STQualType::Qualifiers::kQualMutable;
 }
#line 1764 "stparserimpl.c"
        break;
      case 32: /* type_qualified ::= qualifier type_specifier */
#line 257 "stparserimpl.lemon"
{
    yylhsminor.yy95 = pParser->createNode<yal::frontend::STQualType>(yymsp[0].minor.yy82, yymsp[-1].minor.yy175);
}
#line 1771 "stparserimpl.c"
  yymsp[-1].minor.yy95 = yylhsminor.yy95;
        break;
      case 33: /* decl_import ::= IMPORT decl_import_module SEMI_COLON */
#line 263 "stparserimpl.lemon"
{
    yylhsminor.yy41 = yymsp[-1].minor.yy41;
    auto srcInfo = CreateSourceInfo(yymsp[-2].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy41->setSourceInfo(srcInfo);
    yylhsminor.yy41->finalize(*pParser);
}
#line 1782 "stparserimpl.c"
  yymsp[-2].minor.yy41 = yylhsminor.yy41;
        break;
      case 34: /* decl_import_module ::= decl_import_module COLON COLON identifier_simple */
#line 270 "stparserimpl.lemon"
{
        yylhsminor.yy41 = yymsp[-3].minor.yy41;
        yylhsminor.yy41->addModuleChunk(yymsp[0].minor.yy173);
}
#line 1791 "stparserimpl.c"
  yymsp[-3].minor.yy41 = yylhsminor.yy41;
        break;
      case 35: /* decl_import_module ::= identifier */
#line 275 "stparserimpl.lemon"
{
    yylhsminor.yy41 = pParser->createNode<yal::frontend::STDeclImport>(*pParser);
    yylhsminor.yy41->addModuleChunk(yymsp[0].minor.yy173);
}
#line 1800 "stparserimpl.c"
  yymsp[0].minor.yy41 = yylhsminor.yy41;
        break;
      case 36: /* decl_alias ::= TYPE identifier_simple ALIAS type_specifier SEMI_COLON */
#line 281 "stparserimpl.lemon"
{
    yylhsminor.yy210 = pParser->createNode<yal::frontend::STDeclAlias>(yymsp[-3].minor.yy173, yymsp[-1].minor.yy82, false);
    auto srcInfo = CreateSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy210->setSourceInfo(srcInfo);
}
#line 1810 "stparserimpl.c"
  yymsp[-4].minor.yy210 = yylhsminor.yy210;
        break;
      case 37: /* decl_alias ::= TYPE identifier_simple FROM type_specifier SEMI_COLON */
#line 287 "stparserimpl.lemon"
{
    yylhsminor.yy210 = pParser->createNode<yal::frontend::STDeclAlias>(yymsp[-3].minor.yy173, yymsp[-1].minor.yy82, true);
    auto srcInfo = CreateSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy210->setSourceInfo(srcInfo);
}
#line 1820 "stparserimpl.c"
  yymsp[-4].minor.yy210 = yylhsminor.yy210;
        break;
      case 38: /* decl_struct ::= TYPE identifier_simple STRUCT SCOPE_BEGIN struct_decl_vars SCOPE_END */
#line 294 "stparserimpl.lemon"
{
    yylhsminor.yy65 = pParser->createNode<yal::frontend::STDeclStruct>(yymsp[-4].minor.yy173, yymsp[-1].minor.yy12, *pParser);
    auto srcInfo = CreateSourceInfo(yymsp[-5].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy65->setSourceInfo(srcInfo);
}
#line 1830 "stparserimpl.c"
  yymsp[-5].minor.yy65 = yylhsminor.yy65;
        break;
      case 39: /* struct_decl_vars ::= struct_decl_vars COMMA struct_decl_var */
#line 300 "stparserimpl.lemon"
{
    auto& list = pParser->getStructMemberList();
    yylhsminor.yy12 = list.add(yymsp[-2].minor.yy12, yymsp[0].minor.yy127);
}
#line 1839 "stparserimpl.c"
  yymsp[-2].minor.yy12 = yylhsminor.yy12;
        break;
      case 40: /* struct_decl_vars ::= struct_decl_var */
#line 304 "stparserimpl.lemon"
{
    auto& list = pParser->getStructMemberList();
    yylhsminor.yy12 = list.add(yymsp[0].minor.yy127);
}
#line 1848 "stparserimpl.c"
  yymsp[0].minor.yy12 = yylhsminor.yy12;
        break;
      case 41: /* struct_decl_var ::= identifier_simple COLON type_qualified */
#line 309 "stparserimpl.lemon"
{
    yylhsminor.yy127 = pParser->createNode<yal::frontend::STStructMember>(yymsp[-2].minor.yy173, yymsp[0].minor.yy95);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy173->getSourceInfo(), yymsp[0].minor.yy95->getSourceInfo(),pParser->getSourceHandle());
    yylhsminor.yy127->setSourceInfo(srcInfo);
}
#line 1858 "stparserimpl.c"
  yymsp[-2].minor.yy127 = yylhsminor.yy127;
        break;
      case 42: /* function_param_list ::= PAR_BEGIN function_args_decl PAR_END */
#line 318 "stparserimpl.lemon"
{
    yymsp[-2].minor.yy68 = yymsp[-1].minor.yy68;
}
#line 1866 "stparserimpl.c"
        break;
      case 43: /* decl_function ::= FUNCTION identifier_simple function_param_list function_return_decl SCOPE_BEGIN statement_list_or_empty SCOPE_END */
#line 323 "stparserimpl.lemon"
{
    yylhsminor.yy128 = pParser->createNode<yal::frontend::STDeclFunction>(yymsp[-5].minor.yy173, yymsp[-3].minor.yy95, yymsp[-4].minor.yy68, yymsp[-1].minor.yy137, *pParser);
    yylhsminor.yy128->setSourceInfo(CreateSourceInfo(yymsp[-6].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle()));
}
#line 1874 "stparserimpl.c"
  yymsp[-6].minor.yy128 = yylhsminor.yy128;
        break;
      case 44: /* decl_type_functions_scope ::= IMPL type_specifier SCOPE_BEGIN decl_type_function_list SCOPE_END */
#line 330 "stparserimpl.lemon"
{
        yylhsminor.yy42 = pParser->createNode<yal::frontend::STDeclTypeFunctions>(yymsp[-3].minor.yy82,yymsp[-1].minor.yy223, *pParser);
        yylhsminor.yy42->setSourceInfo(CreateSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle()));
}
#line 1883 "stparserimpl.c"
  yymsp[-4].minor.yy42 = yylhsminor.yy42;
        break;
      case 45: /* decl_type_function_list ::= decl_type_function_list decl_function */
#line 335 "stparserimpl.lemon"
{
    auto& list = pParser->getDeclList();
    yylhsminor.yy223 = list.add(yymsp[-1].minor.yy223, yymsp[0].minor.yy128);
}
#line 1892 "stparserimpl.c"
  yymsp[-1].minor.yy223 = yylhsminor.yy223;
        break;
      case 46: /* decl_type_function_list ::= decl_function */
#line 340 "stparserimpl.lemon"
{
    auto& list = pParser->getDeclList();
    yylhsminor.yy223 = list.add(yymsp[0].minor.yy128);
}
#line 1901 "stparserimpl.c"
  yymsp[0].minor.yy223 = yylhsminor.yy223;
        break;
      case 47: /* function_args_decl ::= function_args_decl COMMA function_arg_decl */
#line 348 "stparserimpl.lemon"
{
        auto& list = pParser->getDeclParamList();
        yylhsminor.yy68 = list.add(yymsp[-2].minor.yy68, yymsp[0].minor.yy49);
}
#line 1910 "stparserimpl.c"
  yymsp[-2].minor.yy68 = yylhsminor.yy68;
        break;
      case 48: /* function_args_decl ::= function_arg_decl */
#line 353 "stparserimpl.lemon"
{
    auto& list = pParser->getDeclParamList();
    yylhsminor.yy68 = list.add(yymsp[0].minor.yy49);
}
#line 1919 "stparserimpl.c"
  yymsp[0].minor.yy68 = yylhsminor.yy68;
        break;
      case 49: /* function_args_decl ::= */
#line 358 "stparserimpl.lemon"
{
   yymsp[1].minor.yy68 = pParser->getDeclParamList().getRangeEmpty();
}
#line 1927 "stparserimpl.c"
        break;
      case 50: /* function_arg_decl ::= identifier_simple COLON type_qualified */
#line 362 "stparserimpl.lemon"
{
        yylhsminor.yy49 = pParser->createNode<yal::frontend::STDeclParam>(yymsp[-2].minor.yy173, yymsp[0].minor.yy95);
        yylhsminor.yy49->setSourceInfo(yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy173->getSourceInfo(),yymsp[0].minor.yy95->getSourceInfo(), pParser->getSourceHandle()));
}
#line 1935 "stparserimpl.c"
  yymsp[-2].minor.yy49 = yylhsminor.yy49;
        break;
      case 51: /* function_arg_decl ::= qualifier self */
#line 367 "stparserimpl.lemon"
{
        auto qualType = pParser->createNode<yal::frontend::STQualType>(nullptr, yymsp[-1].minor.yy175);
        yylhsminor.yy49 = pParser->createNode<yal::frontend::STDeclParam>(yymsp[0].minor.yy173, qualType);
        yylhsminor.yy49->setSourceInfo(yymsp[0].minor.yy173->getSourceInfo());
}
#line 1945 "stparserimpl.c"
  yymsp[-1].minor.yy49 = yylhsminor.yy49;
        break;
      case 52: /* function_return_decl ::= COLON type_qualified */
#line 373 "stparserimpl.lemon"
{ yymsp[-1].minor.yy95 = yymsp[0].minor.yy95;}
#line 1951 "stparserimpl.c"
        break;
      case 53: /* function_return_decl ::= */
      case 67: /* var_type_spec ::= */ yytestcase(yyruleno==67);
#line 374 "stparserimpl.lemon"
{yymsp[1].minor.yy95 = nullptr;}
#line 1957 "stparserimpl.c"
        break;
      case 54: /* statement_list_or_empty ::= */
#line 378 "stparserimpl.lemon"
{yymsp[1].minor.yy137 = pParser->getStmtList().getRangeEmpty();}
#line 1962 "stparserimpl.c"
        break;
      case 55: /* statement_list_or_empty ::= statement_list */
#line 379 "stparserimpl.lemon"
{yylhsminor.yy137 = yymsp[0].minor.yy137;}
#line 1967 "stparserimpl.c"
  yymsp[0].minor.yy137 = yylhsminor.yy137;
        break;
      case 56: /* statement_list ::= statement_list statement */
#line 382 "stparserimpl.lemon"
{
    auto& list = pParser->getStmtList();
    yylhsminor.yy137 = list.add(yymsp[-1].minor.yy137,yymsp[0].minor.yy11);
}
#line 1976 "stparserimpl.c"
  yymsp[-1].minor.yy137 = yylhsminor.yy137;
        break;
      case 57: /* statement_list ::= statement */
#line 386 "stparserimpl.lemon"
{
    auto& list = pParser->getStmtList();
    yylhsminor.yy137 = list.add(yymsp[0].minor.yy11);
}
#line 1985 "stparserimpl.c"
  yymsp[0].minor.yy137 = yylhsminor.yy137;
        break;
      case 58: /* statement ::= expression ASSIGN expression SEMI_COLON */
#line 392 "stparserimpl.lemon"
{
   yylhsminor.yy11 = pParser->createNode<yal::frontend::STStmtAssign>(yymsp[-3].minor.yy178,yymsp[-1].minor.yy178);
   yylhsminor.yy11->setSourceInfo(yal::frontend::CreateSourceInfo(yymsp[-3].minor.yy178->getSourceInfo(), yymsp[-1].minor.yy178->getSourceInfo(), pParser->getSourceHandle()));
}
#line 1994 "stparserimpl.c"
  yymsp[-3].minor.yy11 = yylhsminor.yy11;
        break;
      case 59: /* statement ::= decl_var SEMI_COLON */
#line 396 "stparserimpl.lemon"
{
    yylhsminor.yy11 = pParser->createNode<yal::frontend::STStmtDecl>(yymsp[-1].minor.yy67);
    yylhsminor.yy11->setSourceInfo(yymsp[-1].minor.yy67->getSourceInfo());
}
#line 2003 "stparserimpl.c"
  yymsp[-1].minor.yy11 = yylhsminor.yy11;
        break;
      case 60: /* statement ::= expression SEMI_COLON */
#line 401 "stparserimpl.lemon"
{
    yylhsminor.yy11 = pParser->createNode<yal::frontend::STStmtExpression>(yymsp[-1].minor.yy178);
    yylhsminor.yy11->setSourceInfo(yymsp[-1].minor.yy178->getSourceInfo());
}
#line 2012 "stparserimpl.c"
  yymsp[-1].minor.yy11 = yylhsminor.yy11;
        break;
      case 61: /* statement ::= RETURN expression SEMI_COLON */
#line 406 "stparserimpl.lemon"
{
    yylhsminor.yy11 = pParser->createNode<yal::frontend::STStmtReturn>(yymsp[-1].minor.yy178);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy0, yymsp[-1].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy11->setSourceInfo(srcInfo);
}
#line 2022 "stparserimpl.c"
  yymsp[-2].minor.yy11 = yylhsminor.yy11;
        break;
      case 62: /* statement ::= RETURN SEMI_COLON */
#line 412 "stparserimpl.lemon"
{
    yylhsminor.yy11 = pParser->createNode<yal::frontend::STStmtReturn>(nullptr);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[-1].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy11->setSourceInfo(srcInfo);
}
#line 2032 "stparserimpl.c"
  yymsp[-1].minor.yy11 = yylhsminor.yy11;
        break;
      case 63: /* statement ::= SCOPE_BEGIN statement_list_or_empty SCOPE_END */
#line 418 "stparserimpl.lemon"
{
    yylhsminor.yy11 = pParser->createNode<yal::frontend::STStmtListScoped>(yymsp[-1].minor.yy137, *pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy11->setSourceInfo(srcInfo);
}
#line 2042 "stparserimpl.c"
  yymsp[-2].minor.yy11 = yylhsminor.yy11;
        break;
      case 64: /* decl_var ::= VAR identifier_simple var_type_spec ASSIGN expression */
#line 426 "stparserimpl.lemon"
{
    yylhsminor.yy67 = pParser->createNode<yal::frontend::STDeclVar>(yymsp[-3].minor.yy173, yymsp[-2].minor.yy95, yymsp[0].minor.yy178, false);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy67->setSourceInfo(srcInfo);
}
#line 2052 "stparserimpl.c"
  yymsp[-4].minor.yy67 = yylhsminor.yy67;
        break;
      case 65: /* decl_var ::= LET identifier_simple var_type_spec ASSIGN expression */
#line 432 "stparserimpl.lemon"
{
    yylhsminor.yy67 = pParser->createNode<yal::frontend::STDeclVar>(yymsp[-3].minor.yy173, yymsp[-2].minor.yy95, yymsp[0].minor.yy178, true);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy67->setSourceInfo(srcInfo);
}
#line 2062 "stparserimpl.c"
  yymsp[-4].minor.yy67 = yylhsminor.yy67;
        break;
      case 66: /* var_type_spec ::= COLON type_qualified */
#line 438 "stparserimpl.lemon"
{yymsp[-1].minor.yy95 = yymsp[0].minor.yy95;}
#line 2068 "stparserimpl.c"
        break;
      case 68: /* expression ::= PAR_BEGIN expression PAR_END */
#line 444 "stparserimpl.lemon"
{yymsp[-2].minor.yy178 = yymsp[-1].minor.yy178;}
#line 2073 "stparserimpl.c"
        break;
      case 69: /* expression ::= literal */
#line 445 "stparserimpl.lemon"
{yylhsminor.yy178 = yymsp[0].minor.yy178;}
#line 2078 "stparserimpl.c"
  yymsp[0].minor.yy178 = yylhsminor.yy178;
        break;
      case 70: /* expression ::= unaryexp */
#line 446 "stparserimpl.lemon"
{yylhsminor.yy178 = yymsp[0].minor.yy36;}
#line 2084 "stparserimpl.c"
  yymsp[0].minor.yy178 = yylhsminor.yy178;
        break;
      case 71: /* expression ::= binaryexp */
#line 447 "stparserimpl.lemon"
{yylhsminor.yy178 = yymsp[0].minor.yy38;}
#line 2090 "stparserimpl.c"
  yymsp[0].minor.yy178 = yylhsminor.yy178;
        break;
      case 72: /* expression ::= struct_init */
#line 448 "stparserimpl.lemon"
{ yylhsminor.yy178 = yymsp[0].minor.yy140;}
#line 2096 "stparserimpl.c"
  yymsp[0].minor.yy178 = yylhsminor.yy178;
        break;
      case 73: /* expression ::= expression AS type_qualified */
#line 450 "stparserimpl.lemon"
{
    yylhsminor.yy178 = pParser->createNode<yal::frontend::STExprCast>(yymsp[-2].minor.yy178, yymsp[0].minor.yy95);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy178->getSourceInfo(),
                                                   yymsp[0].minor.yy95->getSourceInfo(),
                                                   pParser->getSourceHandle());
    yylhsminor.yy178->setSourceInfo(srcInfo);
}
#line 2108 "stparserimpl.c"
  yymsp[-2].minor.yy178 = yylhsminor.yy178;
        break;
      case 74: /* expression ::= identifier_or_self */
#line 458 "stparserimpl.lemon"
{
    yylhsminor.yy178 = pParser->createNode<yal::frontend::STExprVarRef>(yymsp[0].minor.yy173);
    yylhsminor.yy178->setSourceInfo(yymsp[0].minor.yy173->getSourceInfo());
}
#line 2117 "stparserimpl.c"
  yymsp[0].minor.yy178 = yylhsminor.yy178;
        break;
      case 75: /* expression ::= expression DOT identifier */
#line 463 "stparserimpl.lemon"
{
    yylhsminor.yy178 = pParser->createNode<yal::frontend::STExprStructVarRef>(yymsp[-2].minor.yy178, yymsp[0].minor.yy173);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy178->getSourceInfo(), yymsp[0].minor.yy173->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy178->setSourceInfo(srcInfo);
}
#line 2127 "stparserimpl.c"
  yymsp[-2].minor.yy178 = yylhsminor.yy178;
        break;
      case 76: /* expression ::= identifier PAR_BEGIN function_call_args PAR_END */
#line 469 "stparserimpl.lemon"
{
    yylhsminor.yy178 = pParser->createNode<yal::frontend::STExprFnCall>(yymsp[-3].minor.yy173,yymsp[-1].minor.yy90,*pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-3].minor.yy173->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy178->setSourceInfo(srcInfo);
}
#line 2137 "stparserimpl.c"
  yymsp[-3].minor.yy178 = yylhsminor.yy178;
        break;
      case 77: /* expression ::= expression DOT identifier_simple PAR_BEGIN function_call_args PAR_END */
#line 475 "stparserimpl.lemon"
{
    yylhsminor.yy178 = pParser->createNode<yal::frontend::STExprFnCall>(yymsp[-3].minor.yy173,yymsp[-5].minor.yy178,yymsp[-1].minor.yy90,*pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-5].minor.yy178->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy178->setSourceInfo(srcInfo);
}
#line 2147 "stparserimpl.c"
  yymsp[-5].minor.yy178 = yylhsminor.yy178;
        break;
      case 78: /* binaryexp ::= expression AND expression */
#line 489 "stparserimpl.lemon"
{
        yylhsminor.yy38 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::And,
                                                        yymsp[-2].minor.yy178, yymsp[0].minor.yy178);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy178->getSourceInfo(), yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy38->setSourceInfo(srcInfo);
}
#line 2158 "stparserimpl.c"
  yymsp[-2].minor.yy38 = yylhsminor.yy38;
        break;
      case 79: /* binaryexp ::= expression OR expression */
#line 495 "stparserimpl.lemon"
{
        yylhsminor.yy38 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Or,
                                                        yymsp[-2].minor.yy178, yymsp[0].minor.yy178);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy178->getSourceInfo(), yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy38->setSourceInfo(srcInfo);
}
#line 2169 "stparserimpl.c"
  yymsp[-2].minor.yy38 = yylhsminor.yy38;
        break;
      case 80: /* binaryexp ::= expression PLUS expression */
#line 501 "stparserimpl.lemon"
{
        yylhsminor.yy38 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Plus,
                                                        yymsp[-2].minor.yy178, yymsp[0].minor.yy178);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy178->getSourceInfo(), yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy38->setSourceInfo(srcInfo);
}
#line 2180 "stparserimpl.c"
  yymsp[-2].minor.yy38 = yylhsminor.yy38;
        break;
      case 81: /* binaryexp ::= expression MINUS expression */
#line 507 "stparserimpl.lemon"
{
        yylhsminor.yy38 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Minus,
                                                        yymsp[-2].minor.yy178, yymsp[0].minor.yy178);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy178->getSourceInfo(), yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy38->setSourceInfo(srcInfo);
}
#line 2191 "stparserimpl.c"
  yymsp[-2].minor.yy38 = yylhsminor.yy38;
        break;
      case 82: /* binaryexp ::= expression MULT expression */
#line 513 "stparserimpl.lemon"
{
        yylhsminor.yy38 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Mult,
                                                        yymsp[-2].minor.yy178, yymsp[0].minor.yy178);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy178->getSourceInfo(), yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy38->setSourceInfo(srcInfo);
}
#line 2202 "stparserimpl.c"
  yymsp[-2].minor.yy38 = yylhsminor.yy38;
        break;
      case 83: /* binaryexp ::= expression DIV expression */
#line 519 "stparserimpl.lemon"
{
        yylhsminor.yy38 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Div,
                                                        yymsp[-2].minor.yy178, yymsp[0].minor.yy178);
}
#line 2211 "stparserimpl.c"
  yymsp[-2].minor.yy38 = yylhsminor.yy38;
        break;
      case 84: /* binaryexp ::= expression MOD expression */
#line 523 "stparserimpl.lemon"
{
        yylhsminor.yy38 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Mod,
                                                        yymsp[-2].minor.yy178, yymsp[0].minor.yy178);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy178->getSourceInfo(), yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy38->setSourceInfo(srcInfo);
}
#line 2222 "stparserimpl.c"
  yymsp[-2].minor.yy38 = yylhsminor.yy38;
        break;
      case 85: /* binaryexp ::= expression EQ expression */
#line 530 "stparserimpl.lemon"
{
        yylhsminor.yy38 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Eq,
                                                        yymsp[-2].minor.yy178, yymsp[0].minor.yy178);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy178->getSourceInfo(), yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy38->setSourceInfo(srcInfo);
}
#line 2233 "stparserimpl.c"
  yymsp[-2].minor.yy38 = yylhsminor.yy38;
        break;
      case 86: /* binaryexp ::= expression NE expression */
#line 537 "stparserimpl.lemon"
{
        yylhsminor.yy38 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Ne,
                                                        yymsp[-2].minor.yy178, yymsp[0].minor.yy178);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy178->getSourceInfo(), yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy38->setSourceInfo(srcInfo);
}
#line 2244 "stparserimpl.c"
  yymsp[-2].minor.yy38 = yylhsminor.yy38;
        break;
      case 87: /* binaryexp ::= expression LE expression */
#line 544 "stparserimpl.lemon"
{
        yylhsminor.yy38 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Le,
                                                        yymsp[-2].minor.yy178, yymsp[0].minor.yy178);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy178->getSourceInfo(), yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy38->setSourceInfo(srcInfo);
}
#line 2255 "stparserimpl.c"
  yymsp[-2].minor.yy38 = yylhsminor.yy38;
        break;
      case 88: /* binaryexp ::= expression LT expression */
#line 551 "stparserimpl.lemon"
{
        yylhsminor.yy38 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Lt,
                                                        yymsp[-2].minor.yy178, yymsp[0].minor.yy178);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy178->getSourceInfo(), yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy38->setSourceInfo(srcInfo);
}
#line 2266 "stparserimpl.c"
  yymsp[-2].minor.yy38 = yylhsminor.yy38;
        break;
      case 89: /* binaryexp ::= expression GE expression */
#line 558 "stparserimpl.lemon"
{
        yylhsminor.yy38 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Ge,
                                                        yymsp[-2].minor.yy178, yymsp[0].minor.yy178);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy178->getSourceInfo(), yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy38->setSourceInfo(srcInfo);
}
#line 2277 "stparserimpl.c"
  yymsp[-2].minor.yy38 = yylhsminor.yy38;
        break;
      case 90: /* binaryexp ::= expression GT expression */
#line 565 "stparserimpl.lemon"
{
        yylhsminor.yy38 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Gt,
                                                        yymsp[-2].minor.yy178, yymsp[0].minor.yy178);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy178->getSourceInfo(), yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy38->setSourceInfo(srcInfo);
}
#line 2288 "stparserimpl.c"
  yymsp[-2].minor.yy38 = yylhsminor.yy38;
        break;
      case 91: /* unaryexp ::= NOT expression */
#line 574 "stparserimpl.lemon"
{
    yylhsminor.yy36 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Not,
                                               yymsp[0].minor.yy178);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy36->setSourceInfo(srcInfo);
}
#line 2299 "stparserimpl.c"
  yymsp[-1].minor.yy36 = yylhsminor.yy36;
        break;
      case 92: /* unaryexp ::= BIT_NOT expression */
#line 580 "stparserimpl.lemon"
{
    yylhsminor.yy36 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::BitNot,
                                               yymsp[0].minor.yy178);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy36->setSourceInfo(srcInfo);
}
#line 2310 "stparserimpl.c"
  yymsp[-1].minor.yy36 = yylhsminor.yy36;
        break;
      case 93: /* unaryexp ::= MINUS expression */
#line 586 "stparserimpl.lemon"
{
    yylhsminor.yy36 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Negate,
                                               yymsp[0].minor.yy178);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy36->setSourceInfo(srcInfo);
}
#line 2321 "stparserimpl.c"
  yymsp[-1].minor.yy36 = yylhsminor.yy36;
        break;
      case 94: /* unaryexp ::= REFERENCE expression */
#line 593 "stparserimpl.lemon"
{
    yylhsminor.yy36 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Reference,
                                               yymsp[0].minor.yy178);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy36->setSourceInfo(srcInfo);
}
#line 2332 "stparserimpl.c"
  yymsp[-1].minor.yy36 = yylhsminor.yy36;
        break;
      case 95: /* unaryexp ::= MULT expression */
#line 600 "stparserimpl.lemon"
{
    yylhsminor.yy36 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Derefence,
                                               yymsp[0].minor.yy178);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy36->setSourceInfo(srcInfo);
}
#line 2343 "stparserimpl.c"
  yymsp[-1].minor.yy36 = yylhsminor.yy36;
        break;
      case 96: /* function_call_args ::= function_call_args COMMA expression */
#line 610 "stparserimpl.lemon"
{
    auto& list = pParser->getExprList();
    yylhsminor.yy90 = list.add(yymsp[-2].minor.yy90, yymsp[0].minor.yy178);
}
#line 2352 "stparserimpl.c"
  yymsp[-2].minor.yy90 = yylhsminor.yy90;
        break;
      case 97: /* function_call_args ::= expression */
#line 614 "stparserimpl.lemon"
{
    auto& list = pParser->getExprList();
    yylhsminor.yy90 = list.add(yymsp[0].minor.yy178);
}
#line 2361 "stparserimpl.c"
  yymsp[0].minor.yy90 = yylhsminor.yy90;
        break;
      case 98: /* function_call_args ::= */
#line 618 "stparserimpl.lemon"
{
    yymsp[1].minor.yy90= pParser->getExprList().getRangeEmpty();
}
#line 2369 "stparserimpl.c"
        break;
      case 99: /* struct_init ::= type_specifier SCOPE_BEGIN struct_member_init_list SCOPE_END */
#line 625 "stparserimpl.lemon"
{
    yylhsminor.yy140 = pParser->createNode<yal::frontend::STExprStructInit>(yymsp[-3].minor.yy82, yymsp[-1].minor.yy91, *pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-3].minor.yy82->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy140->setSourceInfo(srcInfo);
}
#line 2378 "stparserimpl.c"
  yymsp[-3].minor.yy140 = yylhsminor.yy140;
        break;
      case 100: /* struct_member_init_list ::= */
#line 631 "stparserimpl.lemon"
{yymsp[1].minor.yy91 = pParser->getStructInitList().getRangeEmpty();}
#line 2384 "stparserimpl.c"
        break;
      case 101: /* struct_member_init_list ::= struct_member_init_list COMMA struct_member_init */
#line 632 "stparserimpl.lemon"
{
     auto& list = pParser->getStructInitList();
     yylhsminor.yy91 = list.add(yymsp[-2].minor.yy91, yymsp[0].minor.yy209);
}
#line 2392 "stparserimpl.c"
  yymsp[-2].minor.yy91 = yylhsminor.yy91;
        break;
      case 102: /* struct_member_init_list ::= struct_member_init */
#line 636 "stparserimpl.lemon"
{
    auto& list = pParser->getStructInitList();
    yylhsminor.yy91 = list.add(yymsp[0].minor.yy209);
}
#line 2401 "stparserimpl.c"
  yymsp[0].minor.yy91 = yylhsminor.yy91;
        break;
      case 103: /* struct_member_init ::= identifier_simple COLON expression */
#line 641 "stparserimpl.lemon"
{
    yylhsminor.yy209 = pParser->createNode<yal::frontend::STStructMemberInit>(yymsp[-2].minor.yy173,yymsp[0].minor.yy178);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy173->getSourceInfo(), yymsp[0].minor.yy178->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy209->setSourceInfo(srcInfo);
}
#line 2411 "stparserimpl.c"
  yymsp[-2].minor.yy209 = yylhsminor.yy209;
        break;
      case 104: /* literal ::= INTEGER_LITERAL */
      case 105: /* literal ::= INTEGER_LITERAL_I8 */ yytestcase(yyruleno==105);
      case 106: /* literal ::= INTEGER_LITERAL_I16 */ yytestcase(yyruleno==106);
      case 107: /* literal ::= INTEGER_LITERAL_I32 */ yytestcase(yyruleno==107);
      case 108: /* literal ::= INTEGER_LITERAL_I64 */ yytestcase(yyruleno==108);
      case 109: /* literal ::= INTEGER_LITERAL_U8 */ yytestcase(yyruleno==109);
      case 110: /* literal ::= INTEGER_LITERAL_U16 */ yytestcase(yyruleno==110);
      case 111: /* literal ::= INTEGER_LITERAL_U32 */ yytestcase(yyruleno==111);
      case 112: /* literal ::= INTEGER_LITERAL_U64 */ yytestcase(yyruleno==112);
#line 649 "stparserimpl.lemon"
{
        yylhsminor.yy178 = pParser->createNode<yal::frontend::STExprIntegerLiteral>(yymsp[0].minor.yy0);
        auto srcInfo =yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy178->setSourceInfo(srcInfo);
   }
#line 2429 "stparserimpl.c"
  yymsp[0].minor.yy178 = yylhsminor.yy178;
        break;
      case 113: /* literal ::= FLOAT_LITERAL */
      case 114: /* literal ::= FLOAT_LITERAL_32 */ yytestcase(yyruleno==114);
      case 115: /* literal ::= FLOAT_LITERAL_64 */ yytestcase(yyruleno==115);
#line 694 "stparserimpl.lemon"
{
        yylhsminor.yy178 = pParser->createNode<yal::frontend::STExprFloatLiteral>(yymsp[0].minor.yy0);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy178->setSourceInfo(srcInfo);
}
#line 2441 "stparserimpl.c"
  yymsp[0].minor.yy178 = yylhsminor.yy178;
        break;
      case 116: /* literal ::= BOOL_LITERAL */
#line 711 "stparserimpl.lemon"
{
        yylhsminor.yy178 = pParser->createNode<yal::frontend::STExprBoolLiteral>(yymsp[0].minor.yy0.tokenStr);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy178->setSourceInfo(srcInfo);
}
#line 2451 "stparserimpl.c"
  yymsp[0].minor.yy178 = yylhsminor.yy178;
        break;
      default:
      /* (117) module ::= decls END */ yytestcase(yyruleno==117);
      /* (118) type_array ::= type_builtin ARRAY_BEGIN ARRAY_END */ yytestcase(yyruleno==118);
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

  yymsp += yysize+1;
  yypParser->yytos = yymsp;
  yymsp->stateno = (YYACTIONTYPE)yyact;
  yymsp->major = (YYCODETYPE)yygoto;
  yyTraceShift(yypParser, yyact, "... then shift");
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
#line 2530 "stparserimpl.c"
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
    int stateno = yypParser->yytos->stateno;
    if( stateno < YY_MIN_REDUCE ){
      fprintf(yyTraceFILE,"%sInput '%s' in state %d\n",
              yyTracePrompt,yyTokenName[yymajor],stateno);
    }else{
      fprintf(yyTraceFILE,"%sInput '%s' with pending reduce %d\n",
              yyTracePrompt,yyTokenName[yymajor],stateno-YY_MIN_REDUCE);
    }
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,(YYCODETYPE)yymajor);
    if( yyact >= YY_MIN_REDUCE ){
      yy_reduce(yypParser,yyact-YY_MIN_REDUCE,yymajor,yyminor);
    }else if( yyact <= YY_MAX_SHIFTREDUCE ){
      yy_shift(yypParser,yyact,yymajor,yyminor);
#ifndef YYNOERRORRECOVERY
      yypParser->yyerrcnt--;
#endif
      yymajor = YYNOCODE;
    }else if( yyact==YY_ACCEPT_ACTION ){
      yypParser->yytos--;
      yy_accept(yypParser);
      return;
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
