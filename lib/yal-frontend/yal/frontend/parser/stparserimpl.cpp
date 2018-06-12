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
  yal::frontend::STStructMemberInit* yy33;
  yal::frontend::STIdentifier* yy57;
  yal::frontend::STDeclModule* yy66;
  yal::frontend::STDeclFunction* yy68;
  yal::frontend::STExprFnCall::ParamList* yy70;
  yal::frontend::STQualType* yy71;
  uint32_t yy72;
  yal::frontend::STType* yy74;
  yal::frontend::STDeclStruct* yy77;
  yal::frontend::STExprStructInit* yy100;
  yal::frontend::STDecl* yy108;
  yal::frontend::STStatement* yy129;
  yal::frontend::STDeclFunction::Params* yy134;
  yal::frontend::STExprStructInit::MemberInitList* yy140;
  yal::frontend::STExpression* yy162;
  yal::frontend::STDeclVar* yy167;
  yal::frontend::STStatementList* yy187;
  yal::frontend::STDeclStruct::Members* yy198;
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
#define YYNSTATE             123
#define YYNRULE              108
#define YY_MAX_SHIFT         122
#define YY_MIN_SHIFTREDUCE   195
#define YY_MAX_SHIFTREDUCE   302
#define YY_MIN_REDUCE        303
#define YY_MAX_REDUCE        410
#define YY_ERROR_ACTION      411
#define YY_ACCEPT_ACTION     412
#define YY_NO_ACTION         413
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
#define YY_ACTTAB_COUNT (1031)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    14,  120,  120,  118,  329,   15,   16,  199,  325,   87,
 /*    10 */   200,  206,  207,  208,  209,  210,  211,  212,  213,  214,
 /*    20 */   215,  216,  115,  219,  120,   13,   92,  344,   59,   37,
 /*    30 */     2,  312,  312,   32,   35,  412,    8,   54,   52,  110,
 /*    40 */   288,  289,  290,  291,  292,  293,  294,  295,  296,  297,
 /*    50 */   298,  299,  300,   14,  312,   26,   25,   24,   15,   16,
 /*    60 */   199,   56,   87,  200,  206,  207,  208,  209,  210,  211,
 /*    70 */   212,  213,  214,  215,  216,  303,  304,  305,   13,   58,
 /*    80 */   114,  246,  331,  358,   98,   37,   32,   41,   86,  340,
 /*    90 */   115,  219,  110,  288,  289,  290,  291,  292,  293,  294,
 /*   100 */   295,  296,  297,  298,  299,  300,   98,   14,   83,  394,
 /*   110 */    90,  122,   15,   16,  283,   46,   87,  200,  206,  207,
 /*   120 */   208,  209,  210,  211,  212,  213,  214,  215,  216,   60,
 /*   130 */    34,  393,   13,   58,  114,  109,   49,   37,   33,  342,
 /*   140 */    32,   37,  335,  339,   37,   48,  110,  288,  289,  290,
 /*   150 */   291,  292,  293,  294,  295,  296,  297,  298,  299,  300,
 /*   160 */    11,   29,   30,   17,   42,  260,   23,   22,   18,   20,
 /*   170 */    19,   21,   28,   27,   26,   25,   24,  199,   93,  116,
 /*   180 */    56,   51,  116,  120,   17,   85,  261,  112,  310,  366,
 /*   190 */    96,  333,  115,  219,  334,   88,   17,  104,  262,   94,
 /*   200 */    39,  244,    4,   89,  349,   61,  362,  363,  361,   38,
 /*   210 */   364,  228,  313,  312,   53,  313,  103,  120,  111,   85,
 /*   220 */   113,  112,  310,  366,   96,  224,   47,  200,  343,   57,
 /*   230 */   230,  121,    1,   50,   91,    9,    4,   95,  349,   61,
 /*   240 */   362,  363,  361,   10,  364,  247,  120,  312,   85,  243,
 /*   250 */   112,  310,  366,   96,  229,   12,   56,    5,   55,   31,
 /*   260 */   108,  220,   43,    6,    3,    4,   97,  349,   61,  362,
 /*   270 */   363,  361,    7,  364,   29,   30,  312,   44,   45,   23,
 /*   280 */    22,   18,   20,   19,   21,   28,   27,   26,   25,   24,
 /*   290 */    29,   30,   40,   56,  302,   23,   22,   18,   20,   19,
 /*   300 */    21,   28,   27,   26,   25,   24,  222,  119,   36,   56,
 /*   310 */   305,  305,   29,   30,  245,  409,  305,   23,   22,   18,
 /*   320 */    20,   19,   21,   28,   27,   26,   25,   24,   29,   30,
 /*   330 */   242,   56,  305,   23,   22,   18,   20,   19,   21,   28,
 /*   340 */    27,   26,   25,   24,  305,  305,  120,   56,   85,  305,
 /*   350 */   112,  310,  366,   96,  305,  305,  305,  305,  257,  305,
 /*   360 */   305,  305,  305,  305,  305,  305,  305,  348,   61,  362,
 /*   370 */   363,  361,  305,  364,  252,  305,  312,   29,   30,  305,
 /*   380 */   305,  305,   23,   22,   18,   20,   19,   21,   28,   27,
 /*   390 */    26,   25,   24,  305,  305,  205,   56,  206,  207,  208,
 /*   400 */   209,  210,  211,  212,  213,  214,  215,  216,  305,  305,
 /*   410 */   305,  305,  305,  305,  305,  117,  305,  305,  305,   30,
 /*   420 */   305,  305,  305,   23,   22,   18,   20,   19,   21,   28,
 /*   430 */    27,   26,   25,   24,  305,  305,  305,   56,  120,  305,
 /*   440 */    85,  305,  112,  310,  366,  305,  305,  120,  305,   85,
 /*   450 */   305,  112,  310,  366,  305,  305,  305,  305,  305,  305,
 /*   460 */    70,  362,  363,  361,   84,  364,  305,  305,  312,   70,
 /*   470 */   362,  363,  361,   82,  364,  305,  120,  312,   85,  305,
 /*   480 */   112,  310,  366,  305,   87,  305,  206,  207,  208,  209,
 /*   490 */   210,  211,  212,  213,  214,  215,  216,  305,   70,  362,
 /*   500 */   363,  361,   81,  364,  305,  205,  312,  206,  207,  208,
 /*   510 */   209,  210,  211,  212,  213,  214,  215,  216,  411,  411,
 /*   520 */   411,  411,  411,  411,   28,   27,   26,   25,   24,  305,
 /*   530 */   305,  305,   56,   23,   22,   18,   20,   19,   21,   28,
 /*   540 */    27,   26,   25,   24,  120,  305,   85,   56,  112,  310,
 /*   550 */   366,  305,  305,  120,  305,   85,  305,  112,  310,  366,
 /*   560 */   305,  305,  305,  305,  305,  305,   62,  362,  363,  361,
 /*   570 */   305,  364,  305,  305,  312,   66,  362,  363,  361,  305,
 /*   580 */   364,  305,  120,  312,   85,  305,  112,  310,  366,  305,
 /*   590 */   305,  120,  305,   85,  305,  112,  310,  366,  305,  305,
 /*   600 */   305,  305,  305,  305,   67,  362,  363,  361,  305,  364,
 /*   610 */   305,  305,  312,   63,  362,  363,  361,  305,  364,  305,
 /*   620 */   120,  312,   85,  305,  112,  310,  366,  305,  305,  120,
 /*   630 */   305,   85,  305,  112,  310,  366,  305,  305,  305,  305,
 /*   640 */   305,  305,   68,  362,  363,  361,  305,  364,  305,  305,
 /*   650 */   312,   99,  362,  363,  361,  305,  364,  305,  120,  312,
 /*   660 */    85,  305,  112,  310,  366,  305,  305,  120,  305,   85,
 /*   670 */   305,  112,  310,  366,  305,  305,  305,  305,  305,  305,
 /*   680 */   100,  362,  363,  361,  305,  364,  305,  305,  312,  101,
 /*   690 */   362,  363,  361,  305,  364,  305,  120,  312,   85,  305,
 /*   700 */   112,  310,  366,  305,  305,  120,  305,   85,  305,  112,
 /*   710 */   310,  366,  305,  305,  305,  305,  305,  305,  102,  362,
 /*   720 */   363,  361,  305,  364,  305,  305,  312,   69,  362,  363,
 /*   730 */   361,  305,  364,  305,  120,  312,   85,  305,  112,  310,
 /*   740 */   366,  305,  305,  120,  305,   85,  305,  112,  310,  366,
 /*   750 */   305,  305,  305,  305,  305,  305,   72,  362,  363,  361,
 /*   760 */   305,  364,  305,  305,  312,   73,  362,  363,  361,  305,
 /*   770 */   364,  305,  120,  312,   85,  305,  112,  310,  366,  305,
 /*   780 */   305,  120,  305,   85,  305,  112,  310,  366,  305,  305,
 /*   790 */   305,  305,  305,  305,   74,  362,  363,  361,  305,  364,
 /*   800 */   305,  305,  312,   75,  362,  363,  361,  305,  364,  305,
 /*   810 */   120,  312,   85,  305,  112,  310,  366,  305,  305,  120,
 /*   820 */   305,   85,  305,  112,  310,  366,  305,  305,  305,  305,
 /*   830 */   305,  305,   76,  362,  363,  361,  305,  364,  305,  305,
 /*   840 */   312,   77,  362,  363,  361,  305,  364,  305,  120,  312,
 /*   850 */    85,  305,  112,  310,  366,  305,  305,  120,  305,   85,
 /*   860 */   305,  112,  310,  366,  305,  305,  305,  305,  305,  305,
 /*   870 */   105,  362,  363,  361,  305,  364,  305,  305,  312,  106,
 /*   880 */   362,  363,  361,  305,  364,  305,  120,  312,   85,  305,
 /*   890 */   112,  310,  366,  305,  305,  120,  305,   85,  305,  112,
 /*   900 */   310,  366,  305,  305,  305,  305,  305,  305,  107,  362,
 /*   910 */   363,  361,  305,  364,  305,  305,  312,   79,  362,  363,
 /*   920 */   361,  305,  364,  305,  120,  312,   85,  305,  112,  310,
 /*   930 */   366,  305,  305,  120,  305,   85,  305,  112,  310,  366,
 /*   940 */   305,  305,  305,  305,  305,  305,   80,  362,  363,  361,
 /*   950 */   305,  364,  305,  305,  312,   71,  362,  363,  361,  305,
 /*   960 */   364,  305,  120,  312,   85,  305,  112,  310,  366,  305,
 /*   970 */   305,  120,  305,   85,  305,  112,  310,  366,  305,  305,
 /*   980 */   305,  305,  305,  305,   78,  362,  363,  361,  305,  364,
 /*   990 */   305,  305,  312,   64,  362,  363,  361,  305,  364,  305,
 /*  1000 */   120,  312,   85,  305,  112,  310,  366,  305,  305,  305,
 /*  1010 */   305,  305,  305,  305,  305,  305,  305,  305,  305,  305,
 /*  1020 */   305,  305,   65,  362,  363,  361,  305,  364,  305,  305,
 /*  1030 */   312,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    14,   70,   70,   72,   72,   19,   20,   23,   24,   23,
 /*    10 */    24,   25,   26,   27,   28,   29,   30,   31,   32,   33,
 /*    20 */    34,   35,   38,   39,   70,   39,   72,   71,   74,   73,
 /*    30 */    44,  100,  100,   47,   68,   69,   50,   51,   52,   53,
 /*    40 */    54,   55,   56,   57,   58,   59,   60,   61,   62,   63,
 /*    50 */    64,   65,   66,   14,  100,   15,   16,   17,   19,   20,
 /*    60 */    23,   21,   23,   24,   25,   26,   27,   28,   29,   30,
 /*    70 */    31,   32,   33,   34,   35,   78,   79,   80,   39,   73,
 /*    80 */    74,   42,   85,   71,   74,   73,   47,   41,   82,   83,
 /*    90 */    38,   39,   53,   54,   55,   56,   57,   58,   59,   60,
 /*   100 */    61,   62,   63,   64,   65,   66,   74,   14,   98,   99,
 /*   110 */    81,   22,   19,   20,   45,   46,   23,   24,   25,   26,
 /*   120 */    27,   28,   29,   30,   31,   32,   33,   34,   35,   40,
 /*   130 */    47,   99,   39,   73,   74,   71,   74,   73,   49,   71,
 /*   140 */    47,   73,   71,   83,   73,   84,   53,   54,   55,   56,
 /*   150 */    57,   58,   59,   60,   61,   62,   63,   64,   65,   66,
 /*   160 */     1,    2,    3,   46,   41,   48,    7,    8,    9,   10,
 /*   170 */    11,   12,   13,   14,   15,   16,   17,   23,   88,   74,
 /*   180 */    21,   74,   74,   70,   46,   72,   48,   74,   75,   76,
 /*   190 */    77,   86,   38,   39,   86,   87,   46,   41,   48,   88,
 /*   200 */    44,   42,   89,   90,   91,   92,   93,   94,   95,   46,
 /*   210 */    97,   48,   41,  100,   74,   44,   74,   70,   74,   72,
 /*   220 */    81,   74,   75,   76,   77,   45,   46,   24,   75,   84,
 /*   230 */    45,   74,   44,   41,   41,    1,   89,   90,   91,   92,
 /*   240 */    93,   94,   95,    1,   97,   45,   70,  100,   72,   42,
 /*   250 */    74,   75,   76,   77,   45,   41,   21,   47,   41,   47,
 /*   260 */     9,   39,   10,   47,   44,   89,   90,   91,   92,   93,
 /*   270 */    94,   95,   47,   97,    2,    3,  100,   41,   41,    7,
 /*   280 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*   290 */     2,    3,   44,   21,   37,    7,    8,    9,   10,   11,
 /*   300 */    12,   13,   14,   15,   16,   17,   42,   36,   41,   21,
 /*   310 */   101,  101,    2,    3,   42,    0,  101,    7,    8,    9,
 /*   320 */    10,   11,   12,   13,   14,   15,   16,   17,    2,    3,
 /*   330 */    42,   21,  101,    7,    8,    9,   10,   11,   12,   13,
 /*   340 */    14,   15,   16,   17,  101,  101,   70,   21,   72,  101,
 /*   350 */    74,   75,   76,   77,  101,  101,  101,  101,   48,  101,
 /*   360 */   101,  101,  101,  101,  101,  101,  101,   91,   92,   93,
 /*   370 */    94,   95,  101,   97,   48,  101,  100,    2,    3,  101,
 /*   380 */   101,  101,    7,    8,    9,   10,   11,   12,   13,   14,
 /*   390 */    15,   16,   17,  101,  101,   23,   21,   25,   26,   27,
 /*   400 */    28,   29,   30,   31,   32,   33,   34,   35,  101,  101,
 /*   410 */   101,  101,  101,  101,  101,   43,  101,  101,  101,    3,
 /*   420 */   101,  101,  101,    7,    8,    9,   10,   11,   12,   13,
 /*   430 */    14,   15,   16,   17,  101,  101,  101,   21,   70,  101,
 /*   440 */    72,  101,   74,   75,   76,  101,  101,   70,  101,   72,
 /*   450 */   101,   74,   75,   76,  101,  101,  101,  101,  101,  101,
 /*   460 */    92,   93,   94,   95,   96,   97,  101,  101,  100,   92,
 /*   470 */    93,   94,   95,   96,   97,  101,   70,  100,   72,  101,
 /*   480 */    74,   75,   76,  101,   23,  101,   25,   26,   27,   28,
 /*   490 */    29,   30,   31,   32,   33,   34,   35,  101,   92,   93,
 /*   500 */    94,   95,   96,   97,  101,   23,  100,   25,   26,   27,
 /*   510 */    28,   29,   30,   31,   32,   33,   34,   35,    7,    8,
 /*   520 */     9,   10,   11,   12,   13,   14,   15,   16,   17,  101,
 /*   530 */   101,  101,   21,    7,    8,    9,   10,   11,   12,   13,
 /*   540 */    14,   15,   16,   17,   70,  101,   72,   21,   74,   75,
 /*   550 */    76,  101,  101,   70,  101,   72,  101,   74,   75,   76,
 /*   560 */   101,  101,  101,  101,  101,  101,   92,   93,   94,   95,
 /*   570 */   101,   97,  101,  101,  100,   92,   93,   94,   95,  101,
 /*   580 */    97,  101,   70,  100,   72,  101,   74,   75,   76,  101,
 /*   590 */   101,   70,  101,   72,  101,   74,   75,   76,  101,  101,
 /*   600 */   101,  101,  101,  101,   92,   93,   94,   95,  101,   97,
 /*   610 */   101,  101,  100,   92,   93,   94,   95,  101,   97,  101,
 /*   620 */    70,  100,   72,  101,   74,   75,   76,  101,  101,   70,
 /*   630 */   101,   72,  101,   74,   75,   76,  101,  101,  101,  101,
 /*   640 */   101,  101,   92,   93,   94,   95,  101,   97,  101,  101,
 /*   650 */   100,   92,   93,   94,   95,  101,   97,  101,   70,  100,
 /*   660 */    72,  101,   74,   75,   76,  101,  101,   70,  101,   72,
 /*   670 */   101,   74,   75,   76,  101,  101,  101,  101,  101,  101,
 /*   680 */    92,   93,   94,   95,  101,   97,  101,  101,  100,   92,
 /*   690 */    93,   94,   95,  101,   97,  101,   70,  100,   72,  101,
 /*   700 */    74,   75,   76,  101,  101,   70,  101,   72,  101,   74,
 /*   710 */    75,   76,  101,  101,  101,  101,  101,  101,   92,   93,
 /*   720 */    94,   95,  101,   97,  101,  101,  100,   92,   93,   94,
 /*   730 */    95,  101,   97,  101,   70,  100,   72,  101,   74,   75,
 /*   740 */    76,  101,  101,   70,  101,   72,  101,   74,   75,   76,
 /*   750 */   101,  101,  101,  101,  101,  101,   92,   93,   94,   95,
 /*   760 */   101,   97,  101,  101,  100,   92,   93,   94,   95,  101,
 /*   770 */    97,  101,   70,  100,   72,  101,   74,   75,   76,  101,
 /*   780 */   101,   70,  101,   72,  101,   74,   75,   76,  101,  101,
 /*   790 */   101,  101,  101,  101,   92,   93,   94,   95,  101,   97,
 /*   800 */   101,  101,  100,   92,   93,   94,   95,  101,   97,  101,
 /*   810 */    70,  100,   72,  101,   74,   75,   76,  101,  101,   70,
 /*   820 */   101,   72,  101,   74,   75,   76,  101,  101,  101,  101,
 /*   830 */   101,  101,   92,   93,   94,   95,  101,   97,  101,  101,
 /*   840 */   100,   92,   93,   94,   95,  101,   97,  101,   70,  100,
 /*   850 */    72,  101,   74,   75,   76,  101,  101,   70,  101,   72,
 /*   860 */   101,   74,   75,   76,  101,  101,  101,  101,  101,  101,
 /*   870 */    92,   93,   94,   95,  101,   97,  101,  101,  100,   92,
 /*   880 */    93,   94,   95,  101,   97,  101,   70,  100,   72,  101,
 /*   890 */    74,   75,   76,  101,  101,   70,  101,   72,  101,   74,
 /*   900 */    75,   76,  101,  101,  101,  101,  101,  101,   92,   93,
 /*   910 */    94,   95,  101,   97,  101,  101,  100,   92,   93,   94,
 /*   920 */    95,  101,   97,  101,   70,  100,   72,  101,   74,   75,
 /*   930 */    76,  101,  101,   70,  101,   72,  101,   74,   75,   76,
 /*   940 */   101,  101,  101,  101,  101,  101,   92,   93,   94,   95,
 /*   950 */   101,   97,  101,  101,  100,   92,   93,   94,   95,  101,
 /*   960 */    97,  101,   70,  100,   72,  101,   74,   75,   76,  101,
 /*   970 */   101,   70,  101,   72,  101,   74,   75,   76,  101,  101,
 /*   980 */   101,  101,  101,  101,   92,   93,   94,   95,  101,   97,
 /*   990 */   101,  101,  100,   92,   93,   94,   95,  101,   97,  101,
 /*  1000 */    70,  100,   72,  101,   74,   75,   76,  101,  101,  101,
 /*  1010 */   101,  101,  101,  101,  101,  101,  101,  101,  101,  101,
 /*  1020 */   101,  101,   92,   93,   94,   95,  101,   97,  101,  101,
 /*  1030 */   100,
};
#define YY_SHIFT_USE_DFLT (1031)
#define YY_SHIFT_COUNT    (122)
#define YY_SHIFT_MIN      (-16)
#define YY_SHIFT_MAX      (526)
static const short yy_shift_ofst[] = {
 /*     0 */  1031,  -14,  -14,  -14,  -14,   93,   93,   93,   39,   93,
 /*    10 */    93,   93,   93,   93,   93,   93,   93,   93,   93,   93,
 /*    20 */    93,   93,   93,   93,   93,   93,   93,   93,   93,   93,
 /*    30 */    93,   93,   93,  461,  -16,   89,  372,  482,  154,   37,
 /*    40 */    37,   52,   52,   52,   52,   52,   37,   37,   46,   83,
 /*    50 */    37,  123,   37,  123,   37,   37,   37,   46,  203,   83,
 /*    60 */    37,  159,  272,  288,  310,  326,  375,  375,  375,  375,
 /*    70 */   375,  416,  511,  511,  511,  511,  511,  511,  526,   40,
 /*    80 */    40,  117,  138,   69,  150,  156,  163,  171,  180,  185,
 /*    90 */   188,  192,  193,  234,  242,  200,  207,  209,  214,  235,
 /*   100 */   235,  235,  235,  210,  217,  235,  235,  235,  212,  251,
 /*   110 */   252,  216,  225,  220,  236,  222,  237,  248,  264,  257,
 /*   120 */   271,  267,  315,
};
#define YY_REDUCE_USE_DFLT (-70)
#define YY_REDUCE_COUNT (60)
#define YY_REDUCE_MIN   (-69)
#define YY_REDUCE_MAX   (930)
static const short yy_reduce_ofst[] = {
 /*     0 */   -34,  113,  147,  176,  276,  368,  377,  406,  474,  483,
 /*    10 */   512,  521,  550,  559,  588,  597,  626,  635,  664,  673,
 /*    20 */   702,  711,  740,  749,  778,  787,  816,  825,  854,  863,
 /*    30 */   892,  901,  930,  -46,    6,   -3,  -69,  -68,   60,   10,
 /*    40 */   108,  -44,   12,   64,   68,   71,   32,  105,   29,   61,
 /*    50 */    62,   90,  107,  111,  140,  142,  144,  139,  153,  145,
 /*    60 */   157,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   306,  346,  346,  346,  347,  390,  390,  390,  411,  411,
 /*    10 */   411,  411,  411,  411,  411,  411,  411,  411,  411,  411,
 /*    20 */   411,  411,  411,  411,  411,  411,  411,  411,  411,  411,
 /*    30 */   411,  411,  411,  411,  341,  411,  411,  411,  325,  392,
 /*    40 */   411,  325,  325,  325,  325,  325,  411,  411,  345,  411,
 /*    50 */   411,  359,  411,  359,  411,  411,  411,  345,  411,  411,
 /*    60 */   411,  411,  411,  411,  411,  411,  357,  356,  395,  388,
 /*    70 */   389,  372,  383,  382,  381,  380,  379,  378,  371,  374,
 /*    80 */   373,  411,  411,  411,  411,  411,  411,  307,  411,  411,
 /*    90 */   411,  411,  411,  411,  411,  411,  411,  411,  411,  387,
 /*   100 */   386,  385,  384,  411,  411,  377,  376,  375,  411,  411,
 /*   110 */   411,  367,  309,  411,  411,  326,  411,  411,  411,  411,
 /*   120 */   311,  411,  411,
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
 /*  62 */ "expression ::= RANGE_CAST LT type_qualified GT PAR_BEGIN expression PAR_END",
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
  { 92, -7 },
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
#line 124 "stparserimpl.lemon"
{
    yylhsminor.yy66 =yymsp[-1].minor.yy66; yymsp[-1].minor.yy66->addDecl(yymsp[0].minor.yy108);
}
#line 1341 "stparserimpl.c"
  yymsp[-1].minor.yy66 = yylhsminor.yy66;
        break;
      case 1: /* decls ::= decls decl_function */
      case 2: /* decls ::= decls decl_type_function */ yytestcase(yyruleno==2);
#line 127 "stparserimpl.lemon"
{
        yylhsminor.yy66 =yymsp[-1].minor.yy66; yymsp[-1].minor.yy66->addDecl(yymsp[0].minor.yy68);
}
#line 1350 "stparserimpl.c"
  yymsp[-1].minor.yy66 = yylhsminor.yy66;
        break;
      case 3: /* decls ::= */
#line 133 "stparserimpl.lemon"
{
        yymsp[1].minor.yy66 =  pParser->getDeclModule();
     }
#line 1358 "stparserimpl.c"
        break;
      case 4: /* identifier ::= IDENTIFIER */
      case 5: /* self ::= SELF */ yytestcase(yyruleno==5);
#line 140 "stparserimpl.lemon"
{
        yylhsminor.yy57 = pParser->createNode<yal::frontend::STIdentifier>(yymsp[0].minor.yy0.tokenStr);
        auto srcInfo = CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy57->setSourceInfo(srcInfo);
}
#line 1368 "stparserimpl.c"
  yymsp[0].minor.yy57 = yylhsminor.yy57;
        break;
      case 6: /* identifier_or_self ::= identifier */
      case 7: /* identifier_or_self ::= self */ yytestcase(yyruleno==7);
#line 152 "stparserimpl.lemon"
{yylhsminor.yy57=yymsp[0].minor.yy57;}
#line 1375 "stparserimpl.c"
  yymsp[0].minor.yy57 = yylhsminor.yy57;
        break;
      case 8: /* type_specifier ::= type_builtin */
#line 155 "stparserimpl.lemon"
{
        yylhsminor.yy74=yymsp[0].minor.yy74;
}
#line 1383 "stparserimpl.c"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 9: /* type_specifier ::= type_array */
#line 159 "stparserimpl.lemon"
{yymsp[0].minor.yy74 = nullptr;}
#line 1389 "stparserimpl.c"
        break;
      case 10: /* type_specifier ::= IDENTIFIER */
#line 160 "stparserimpl.lemon"
{
    yylhsminor.yy74 = pParser->createNode<yal::frontend::STType>(yymsp[0].minor.yy0.tokenStr);
    yylhsminor.yy74->setSourceInfo(yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0,yymsp[0].minor.yy0, pParser->getSourceHandle()));
}
#line 1397 "stparserimpl.c"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 11: /* type_builtin ::= TYPE_BOOL */
#line 165 "stparserimpl.lemon"
{
    yylhsminor.yy74 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Bool);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy74->setSourceInfo(srcInfo);
    }
#line 1407 "stparserimpl.c"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 12: /* type_builtin ::= TYPE_INT8 */
#line 170 "stparserimpl.lemon"
{
        yylhsminor.yy74 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int8);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy74->setSourceInfo(srcInfo);
    }
#line 1417 "stparserimpl.c"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 13: /* type_builtin ::= TYPE_UINT8 */
#line 175 "stparserimpl.lemon"
{
        yylhsminor.yy74 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt8);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy74->setSourceInfo(srcInfo);
    }
#line 1427 "stparserimpl.c"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 14: /* type_builtin ::= TYPE_INT16 */
#line 180 "stparserimpl.lemon"
{
        yylhsminor.yy74 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int16);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy74->setSourceInfo(srcInfo);
    }
#line 1437 "stparserimpl.c"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 15: /* type_builtin ::= TYPE_UINT16 */
#line 185 "stparserimpl.lemon"
{
        yylhsminor.yy74 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt16);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy74->setSourceInfo(srcInfo);
    }
#line 1447 "stparserimpl.c"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 16: /* type_builtin ::= TYPE_INT32 */
#line 190 "stparserimpl.lemon"
{
        yylhsminor.yy74 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int32);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy74->setSourceInfo(srcInfo);
    }
#line 1457 "stparserimpl.c"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 17: /* type_builtin ::= TYPE_UINT32 */
#line 195 "stparserimpl.lemon"
{
        yylhsminor.yy74 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt32);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy74->setSourceInfo(srcInfo);
    }
#line 1467 "stparserimpl.c"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 18: /* type_builtin ::= TYPE_INT64 */
#line 200 "stparserimpl.lemon"
{
        yylhsminor.yy74 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int64);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy74->setSourceInfo(srcInfo);
    }
#line 1477 "stparserimpl.c"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 19: /* type_builtin ::= TYPE_UINT64 */
#line 205 "stparserimpl.lemon"
{
        yylhsminor.yy74 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt64);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy74->setSourceInfo(srcInfo);
    }
#line 1487 "stparserimpl.c"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 20: /* type_builtin ::= TYPE_FLOAT */
#line 210 "stparserimpl.lemon"
{
        yylhsminor.yy74 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Float32);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy74->setSourceInfo(srcInfo);
    }
#line 1497 "stparserimpl.c"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 21: /* type_builtin ::= TYPE_DOUBLE */
#line 215 "stparserimpl.lemon"
{
        yylhsminor.yy74 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Float64);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy74->setSourceInfo(srcInfo);
    }
#line 1507 "stparserimpl.c"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 22: /* qualifier ::= */
#line 223 "stparserimpl.lemon"
{yymsp[1].minor.yy72 = 0;}
#line 1513 "stparserimpl.c"
        break;
      case 23: /* qualifier ::= MUT */
#line 224 "stparserimpl.lemon"
{yymsp[0].minor.yy72 = yal::frontend::STQualType::Qualifiers::kQualMutable;}
#line 1518 "stparserimpl.c"
        break;
      case 24: /* qualifier ::= REFERENCE */
#line 225 "stparserimpl.lemon"
{yymsp[0].minor.yy72 = yal::frontend::STQualType::Qualifiers::kQualReference;}
#line 1523 "stparserimpl.c"
        break;
      case 25: /* qualifier ::= MUT REFERENCE */
#line 226 "stparserimpl.lemon"
{
        yymsp[-1].minor.yy72 = yal::frontend::STQualType::Qualifiers::kQualReference | yal::frontend::STQualType::Qualifiers::kQualMutable;
 }
#line 1530 "stparserimpl.c"
        break;
      case 26: /* type_qualified ::= qualifier type_specifier */
#line 229 "stparserimpl.lemon"
{
    yylhsminor.yy71 = pParser->createNode<yal::frontend::STQualType>(yymsp[0].minor.yy74, yymsp[-1].minor.yy72);
}
#line 1537 "stparserimpl.c"
  yymsp[-1].minor.yy71 = yylhsminor.yy71;
        break;
      case 27: /* decl_type ::= TYPE identifier COLON type_specifier SEMI_COLON */
#line 235 "stparserimpl.lemon"
{
    yymsp[-4].minor.yy108 = nullptr;
}
#line 1545 "stparserimpl.c"
        break;
      case 28: /* decl_type ::= decl_struct */
#line 239 "stparserimpl.lemon"
{ yylhsminor.yy108 = yymsp[0].minor.yy77;}
#line 1550 "stparserimpl.c"
  yymsp[0].minor.yy108 = yylhsminor.yy108;
        break;
      case 29: /* decl_struct ::= TYPE identifier COLON STRUCT SCOPE_BEGIN struct_decl_vars SCOPE_END */
#line 244 "stparserimpl.lemon"
{
    yylhsminor.yy77 = pParser->createNode<yal::frontend::STDeclStruct>(yymsp[-5].minor.yy57, yymsp[-1].minor.yy198);
    auto srcInfo = CreateSourceInfo(yymsp[-6].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy77->setSourceInfo(srcInfo);
}
#line 1560 "stparserimpl.c"
  yymsp[-6].minor.yy77 = yylhsminor.yy77;
        break;
      case 30: /* struct_decl_vars ::= struct_decl_vars COMMA struct_decl_var */
#line 250 "stparserimpl.lemon"
{
    yylhsminor.yy198 = yymsp[-2].minor.yy198;
    yylhsminor.yy198->push_back(yymsp[0].minor.yy25);
}
#line 1569 "stparserimpl.c"
  yymsp[-2].minor.yy198 = yylhsminor.yy198;
        break;
      case 31: /* struct_decl_vars ::= struct_decl_var */
#line 254 "stparserimpl.lemon"
{
    yylhsminor.yy198 = pParser->createVector<const yal::frontend::STStructMember*>();
    yylhsminor.yy198->push_back(yymsp[0].minor.yy25);
}
#line 1578 "stparserimpl.c"
  yymsp[0].minor.yy198 = yylhsminor.yy198;
        break;
      case 32: /* struct_decl_var ::= identifier COLON type_qualified */
#line 259 "stparserimpl.lemon"
{
    yylhsminor.yy25 = pParser->createNode<yal::frontend::STStructMember>(yymsp[-2].minor.yy57, yymsp[0].minor.yy71);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy57->getSourceInfo(), yymsp[0].minor.yy71->getSourceInfo(),pParser->getSourceHandle());
    yylhsminor.yy25->setSourceInfo(srcInfo);
}
#line 1588 "stparserimpl.c"
  yymsp[-2].minor.yy25 = yylhsminor.yy25;
        break;
      case 33: /* function_param_list ::= PAR_BEGIN function_args_decl PAR_END */
#line 268 "stparserimpl.lemon"
{
    yymsp[-2].minor.yy134 = yymsp[-1].minor.yy134;
}
#line 1596 "stparserimpl.c"
        break;
      case 34: /* decl_function ::= FUNCTION identifier function_param_list function_return_decl SCOPE_BEGIN statement_list_or_empty SCOPE_END */
#line 273 "stparserimpl.lemon"
{
    yylhsminor.yy68 = pParser->createNode<yal::frontend::STDeclFunction>(yymsp[-5].minor.yy57, nullptr, yymsp[-3].minor.yy71, yymsp[-4].minor.yy134, yymsp[-1].minor.yy187);
    yylhsminor.yy68->setSourceInfo(CreateSourceInfo(yymsp[-6].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle()));
}
#line 1604 "stparserimpl.c"
  yymsp[-6].minor.yy68 = yylhsminor.yy68;
        break;
      case 35: /* decl_type_function ::= FUNCTION type_specifier COLON COLON identifier function_param_list function_return_decl SCOPE_BEGIN statement_list_or_empty SCOPE_END */
#line 281 "stparserimpl.lemon"
{
    yylhsminor.yy68 = pParser->createNode<yal::frontend::STDeclFunction>(yymsp[-5].minor.yy57, yymsp[-8].minor.yy74, yymsp[-3].minor.yy71, yymsp[-4].minor.yy134, yymsp[-1].minor.yy187);
    yylhsminor.yy68->setSourceInfo(CreateSourceInfo(yymsp[-9].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle()));
}
#line 1613 "stparserimpl.c"
  yymsp[-9].minor.yy68 = yylhsminor.yy68;
        break;
      case 36: /* function_args_decl ::= function_args_decl COMMA function_arg_decl */
#line 288 "stparserimpl.lemon"
{
        yymsp[-2].minor.yy134->push_back(yymsp[0].minor.yy3);
        yylhsminor.yy134 = yymsp[-2].minor.yy134;
}
#line 1622 "stparserimpl.c"
  yymsp[-2].minor.yy134 = yylhsminor.yy134;
        break;
      case 37: /* function_args_decl ::= function_arg_decl */
#line 293 "stparserimpl.lemon"
{
    yylhsminor.yy134 = pParser->createVector<const yal::frontend::STDeclParam*>();
    yylhsminor.yy134->push_back(yymsp[0].minor.yy3);
}
#line 1631 "stparserimpl.c"
  yymsp[0].minor.yy134 = yylhsminor.yy134;
        break;
      case 38: /* function_args_decl ::= */
#line 298 "stparserimpl.lemon"
{
        yymsp[1].minor.yy134 = nullptr;
}
#line 1639 "stparserimpl.c"
        break;
      case 39: /* function_arg_decl ::= identifier COLON type_qualified */
#line 302 "stparserimpl.lemon"
{
        yylhsminor.yy3 = pParser->createNode<yal::frontend::STDeclParam>(yymsp[-2].minor.yy57, yymsp[0].minor.yy71);
        yylhsminor.yy3->setSourceInfo(yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy57->getSourceInfo(),yymsp[0].minor.yy71->getSourceInfo(), pParser->getSourceHandle()));
}
#line 1647 "stparserimpl.c"
  yymsp[-2].minor.yy3 = yylhsminor.yy3;
        break;
      case 40: /* function_arg_decl ::= qualifier self */
#line 307 "stparserimpl.lemon"
{
        auto qualType = pParser->createNode<yal::frontend::STQualType>(nullptr, yymsp[-1].minor.yy72);
        yylhsminor.yy3 = pParser->createNode<yal::frontend::STDeclParam>(yymsp[0].minor.yy57, qualType);
        yylhsminor.yy3->setSourceInfo(yymsp[0].minor.yy57->getSourceInfo());
}
#line 1657 "stparserimpl.c"
  yymsp[-1].minor.yy3 = yylhsminor.yy3;
        break;
      case 41: /* function_return_decl ::= COLON type_qualified */
#line 313 "stparserimpl.lemon"
{ yymsp[-1].minor.yy71 = yymsp[0].minor.yy71;}
#line 1663 "stparserimpl.c"
        break;
      case 42: /* function_return_decl ::= */
      case 56: /* var_type_spec ::= */ yytestcase(yyruleno==56);
#line 314 "stparserimpl.lemon"
{yymsp[1].minor.yy71 = nullptr;}
#line 1669 "stparserimpl.c"
        break;
      case 43: /* statement_list_or_empty ::= */
#line 318 "stparserimpl.lemon"
{yymsp[1].minor.yy187 = nullptr;}
#line 1674 "stparserimpl.c"
        break;
      case 44: /* statement_list_or_empty ::= statement_list */
#line 319 "stparserimpl.lemon"
{yylhsminor.yy187 = yymsp[0].minor.yy187;}
#line 1679 "stparserimpl.c"
  yymsp[0].minor.yy187 = yylhsminor.yy187;
        break;
      case 45: /* statement_list ::= statement_list statement */
#line 322 "stparserimpl.lemon"
{
    yymsp[-1].minor.yy187->push_back(yymsp[0].minor.yy129);
    yylhsminor.yy187=yymsp[-1].minor.yy187;
}
#line 1688 "stparserimpl.c"
  yymsp[-1].minor.yy187 = yylhsminor.yy187;
        break;
      case 46: /* statement_list ::= statement */
#line 326 "stparserimpl.lemon"
{
    yylhsminor.yy187 = pParser->createVector<const yal::frontend::STStatement*>();
    yylhsminor.yy187->push_back(yymsp[0].minor.yy129);
}
#line 1697 "stparserimpl.c"
  yymsp[0].minor.yy187 = yylhsminor.yy187;
        break;
      case 47: /* statement ::= expression ASSIGN expression SEMI_COLON */
#line 332 "stparserimpl.lemon"
{
   yylhsminor.yy129 = pParser->createNode<yal::frontend::STStmtAssign>(yymsp[-3].minor.yy162,yymsp[-1].minor.yy162);
   yylhsminor.yy129->setSourceInfo(yal::frontend::CreateSourceInfo(yymsp[-3].minor.yy162->getSourceInfo(), yymsp[-1].minor.yy162->getSourceInfo(), pParser->getSourceHandle()));
}
#line 1706 "stparserimpl.c"
  yymsp[-3].minor.yy129 = yylhsminor.yy129;
        break;
      case 48: /* statement ::= decl_var SEMI_COLON */
#line 336 "stparserimpl.lemon"
{
    yylhsminor.yy129 = pParser->createNode<yal::frontend::STStmtDecl>(yymsp[-1].minor.yy167);
    yylhsminor.yy129->setSourceInfo(yymsp[-1].minor.yy167->getSourceInfo());
}
#line 1715 "stparserimpl.c"
  yymsp[-1].minor.yy129 = yylhsminor.yy129;
        break;
      case 49: /* statement ::= expression SEMI_COLON */
#line 341 "stparserimpl.lemon"
{
    yylhsminor.yy129 = pParser->createNode<yal::frontend::STStmtExpression>(yymsp[-1].minor.yy162);
    yylhsminor.yy129->setSourceInfo(yymsp[-1].minor.yy162->getSourceInfo());
}
#line 1724 "stparserimpl.c"
  yymsp[-1].minor.yy129 = yylhsminor.yy129;
        break;
      case 50: /* statement ::= RETURN expression SEMI_COLON */
#line 346 "stparserimpl.lemon"
{
    yylhsminor.yy129 = pParser->createNode<yal::frontend::STStmtReturn>(yymsp[-1].minor.yy162);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy0, yymsp[-1].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy129->setSourceInfo(srcInfo);
}
#line 1734 "stparserimpl.c"
  yymsp[-2].minor.yy129 = yylhsminor.yy129;
        break;
      case 51: /* statement ::= RETURN SEMI_COLON */
#line 352 "stparserimpl.lemon"
{
    yylhsminor.yy129 = pParser->createNode<yal::frontend::STStmtReturn>(nullptr);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[-1].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy129->setSourceInfo(srcInfo);
}
#line 1744 "stparserimpl.c"
  yymsp[-1].minor.yy129 = yylhsminor.yy129;
        break;
      case 52: /* statement ::= SCOPE_BEGIN statement_list_or_empty SCOPE_END */
#line 358 "stparserimpl.lemon"
{
    yylhsminor.yy129 = pParser->createNode<yal::frontend::STStmtListScoped>(yymsp[-1].minor.yy187);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy129->setSourceInfo(srcInfo);
}
#line 1754 "stparserimpl.c"
  yymsp[-2].minor.yy129 = yylhsminor.yy129;
        break;
      case 53: /* decl_var ::= VAR identifier var_type_spec ASSIGN expression */
      case 54: /* decl_var ::= LET identifier var_type_spec ASSIGN expression */ yytestcase(yyruleno==54);
#line 366 "stparserimpl.lemon"
{
    yylhsminor.yy167 = pParser->createNode<yal::frontend::STDeclVar>(yymsp[-3].minor.yy57, yymsp[-2].minor.yy71, yymsp[0].minor.yy162);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy167->setSourceInfo(srcInfo);
}
#line 1765 "stparserimpl.c"
  yymsp[-4].minor.yy167 = yylhsminor.yy167;
        break;
      case 55: /* var_type_spec ::= COLON type_qualified */
#line 378 "stparserimpl.lemon"
{yymsp[-1].minor.yy71 = yymsp[0].minor.yy71;}
#line 1771 "stparserimpl.c"
        break;
      case 57: /* expression ::= PAR_BEGIN expression PAR_END */
#line 384 "stparserimpl.lemon"
{yymsp[-2].minor.yy162 = yymsp[-1].minor.yy162;}
#line 1776 "stparserimpl.c"
        break;
      case 58: /* expression ::= literal */
#line 385 "stparserimpl.lemon"
{yylhsminor.yy162 = yymsp[0].minor.yy162;}
#line 1781 "stparserimpl.c"
  yymsp[0].minor.yy162 = yylhsminor.yy162;
        break;
      case 59: /* expression ::= unaryexp */
#line 386 "stparserimpl.lemon"
{yylhsminor.yy162 = yymsp[0].minor.yy2;}
#line 1787 "stparserimpl.c"
  yymsp[0].minor.yy162 = yylhsminor.yy162;
        break;
      case 60: /* expression ::= binaryexp */
#line 387 "stparserimpl.lemon"
{yylhsminor.yy162 = yymsp[0].minor.yy4;}
#line 1793 "stparserimpl.c"
  yymsp[0].minor.yy162 = yylhsminor.yy162;
        break;
      case 61: /* expression ::= struct_init */
#line 388 "stparserimpl.lemon"
{ yylhsminor.yy162 = yymsp[0].minor.yy100;}
#line 1799 "stparserimpl.c"
  yymsp[0].minor.yy162 = yylhsminor.yy162;
        break;
      case 62: /* expression ::= RANGE_CAST LT type_qualified GT PAR_BEGIN expression PAR_END */
#line 390 "stparserimpl.lemon"
{
    yylhsminor.yy162 = pParser->createNode<yal::frontend::STExprRangeCast>(yymsp[-4].minor.yy71, yymsp[-1].minor.yy162);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-6].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy162->setSourceInfo(srcInfo);
}
#line 1809 "stparserimpl.c"
  yymsp[-6].minor.yy162 = yylhsminor.yy162;
        break;
      case 63: /* expression ::= identifier_or_self */
#line 396 "stparserimpl.lemon"
{
    yylhsminor.yy162 = pParser->createNode<yal::frontend::STExprVarRef>(yymsp[0].minor.yy57);
    yylhsminor.yy162->setSourceInfo(yymsp[0].minor.yy57->getSourceInfo());
}
#line 1818 "stparserimpl.c"
  yymsp[0].minor.yy162 = yylhsminor.yy162;
        break;
      case 64: /* expression ::= expression DOT identifier */
#line 401 "stparserimpl.lemon"
{
    yylhsminor.yy162 = pParser->createNode<yal::frontend::STExprStructVarRef>(yymsp[-2].minor.yy162, yymsp[0].minor.yy57);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy162->getSourceInfo(), yymsp[0].minor.yy57->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy162->setSourceInfo(srcInfo);
}
#line 1828 "stparserimpl.c"
  yymsp[-2].minor.yy162 = yylhsminor.yy162;
        break;
      case 65: /* expression ::= identifier PAR_BEGIN function_call_args PAR_END */
#line 407 "stparserimpl.lemon"
{
    yylhsminor.yy162 = pParser->createNode<yal::frontend::STExprFnCall>(yymsp[-3].minor.yy57,yymsp[-1].minor.yy70);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-3].minor.yy57->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy162->setSourceInfo(srcInfo);
}
#line 1838 "stparserimpl.c"
  yymsp[-3].minor.yy162 = yylhsminor.yy162;
        break;
      case 66: /* expression ::= expression DOT identifier PAR_BEGIN function_call_args PAR_END */
#line 413 "stparserimpl.lemon"
{
    yylhsminor.yy162 = pParser->createNode<yal::frontend::STExprFnCall>(yymsp[-3].minor.yy57,yymsp[-5].minor.yy162,yymsp[-1].minor.yy70);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-5].minor.yy162->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy162->setSourceInfo(srcInfo);
}
#line 1848 "stparserimpl.c"
  yymsp[-5].minor.yy162 = yylhsminor.yy162;
        break;
      case 67: /* expression ::= type_specifier COLON COLON identifier PAR_BEGIN function_call_args PAR_END */
#line 419 "stparserimpl.lemon"
{
    yylhsminor.yy162 = pParser->createNode<yal::frontend::STExprFnCall>(yymsp[-3].minor.yy57,yymsp[-6].minor.yy74,yymsp[-1].minor.yy70);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-6].minor.yy74->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy162->setSourceInfo(srcInfo);
}
#line 1858 "stparserimpl.c"
  yymsp[-6].minor.yy162 = yylhsminor.yy162;
        break;
      case 68: /* binaryexp ::= expression AND expression */
#line 427 "stparserimpl.lemon"
{
        yylhsminor.yy4 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::And,
                                                        yymsp[-2].minor.yy162, yymsp[0].minor.yy162);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy162->getSourceInfo(), yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy4->setSourceInfo(srcInfo);
}
#line 1869 "stparserimpl.c"
  yymsp[-2].minor.yy4 = yylhsminor.yy4;
        break;
      case 69: /* binaryexp ::= expression OR expression */
#line 433 "stparserimpl.lemon"
{
        yylhsminor.yy4 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Or,
                                                        yymsp[-2].minor.yy162, yymsp[0].minor.yy162);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy162->getSourceInfo(), yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy4->setSourceInfo(srcInfo);
}
#line 1880 "stparserimpl.c"
  yymsp[-2].minor.yy4 = yylhsminor.yy4;
        break;
      case 70: /* binaryexp ::= expression PLUS expression */
#line 439 "stparserimpl.lemon"
{
        yylhsminor.yy4 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Plus,
                                                        yymsp[-2].minor.yy162, yymsp[0].minor.yy162);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy162->getSourceInfo(), yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy4->setSourceInfo(srcInfo);
}
#line 1891 "stparserimpl.c"
  yymsp[-2].minor.yy4 = yylhsminor.yy4;
        break;
      case 71: /* binaryexp ::= expression MINUS expression */
#line 445 "stparserimpl.lemon"
{
        yylhsminor.yy4 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Minus,
                                                        yymsp[-2].minor.yy162, yymsp[0].minor.yy162);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy162->getSourceInfo(), yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy4->setSourceInfo(srcInfo);
}
#line 1902 "stparserimpl.c"
  yymsp[-2].minor.yy4 = yylhsminor.yy4;
        break;
      case 72: /* binaryexp ::= expression MULT expression */
#line 451 "stparserimpl.lemon"
{
        yylhsminor.yy4 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Mult,
                                                        yymsp[-2].minor.yy162, yymsp[0].minor.yy162);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy162->getSourceInfo(), yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy4->setSourceInfo(srcInfo);
}
#line 1913 "stparserimpl.c"
  yymsp[-2].minor.yy4 = yylhsminor.yy4;
        break;
      case 73: /* binaryexp ::= expression DIV expression */
#line 457 "stparserimpl.lemon"
{
        yylhsminor.yy4 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Div,
                                                        yymsp[-2].minor.yy162, yymsp[0].minor.yy162);
}
#line 1922 "stparserimpl.c"
  yymsp[-2].minor.yy4 = yylhsminor.yy4;
        break;
      case 74: /* binaryexp ::= expression MOD expression */
#line 461 "stparserimpl.lemon"
{
        yylhsminor.yy4 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Mod,
                                                        yymsp[-2].minor.yy162, yymsp[0].minor.yy162);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy162->getSourceInfo(), yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy4->setSourceInfo(srcInfo);
}
#line 1933 "stparserimpl.c"
  yymsp[-2].minor.yy4 = yylhsminor.yy4;
        break;
      case 75: /* binaryexp ::= expression EQ expression */
#line 468 "stparserimpl.lemon"
{
        yylhsminor.yy4 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Eq,
                                                        yymsp[-2].minor.yy162, yymsp[0].minor.yy162);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy162->getSourceInfo(), yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy4->setSourceInfo(srcInfo);
}
#line 1944 "stparserimpl.c"
  yymsp[-2].minor.yy4 = yylhsminor.yy4;
        break;
      case 76: /* binaryexp ::= expression NE expression */
#line 475 "stparserimpl.lemon"
{
        yylhsminor.yy4 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Ne,
                                                        yymsp[-2].minor.yy162, yymsp[0].minor.yy162);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy162->getSourceInfo(), yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy4->setSourceInfo(srcInfo);
}
#line 1955 "stparserimpl.c"
  yymsp[-2].minor.yy4 = yylhsminor.yy4;
        break;
      case 77: /* binaryexp ::= expression LE expression */
#line 482 "stparserimpl.lemon"
{
        yylhsminor.yy4 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Le,
                                                        yymsp[-2].minor.yy162, yymsp[0].minor.yy162);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy162->getSourceInfo(), yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy4->setSourceInfo(srcInfo);
}
#line 1966 "stparserimpl.c"
  yymsp[-2].minor.yy4 = yylhsminor.yy4;
        break;
      case 78: /* binaryexp ::= expression LT expression */
#line 489 "stparserimpl.lemon"
{
        yylhsminor.yy4 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Lt,
                                                        yymsp[-2].minor.yy162, yymsp[0].minor.yy162);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy162->getSourceInfo(), yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy4->setSourceInfo(srcInfo);
}
#line 1977 "stparserimpl.c"
  yymsp[-2].minor.yy4 = yylhsminor.yy4;
        break;
      case 79: /* binaryexp ::= expression GE expression */
#line 496 "stparserimpl.lemon"
{
        yylhsminor.yy4 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Ge,
                                                        yymsp[-2].minor.yy162, yymsp[0].minor.yy162);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy162->getSourceInfo(), yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy4->setSourceInfo(srcInfo);
}
#line 1988 "stparserimpl.c"
  yymsp[-2].minor.yy4 = yylhsminor.yy4;
        break;
      case 80: /* binaryexp ::= expression GT expression */
#line 503 "stparserimpl.lemon"
{
        yylhsminor.yy4 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Gt,
                                                        yymsp[-2].minor.yy162, yymsp[0].minor.yy162);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy162->getSourceInfo(), yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy4->setSourceInfo(srcInfo);
}
#line 1999 "stparserimpl.c"
  yymsp[-2].minor.yy4 = yylhsminor.yy4;
        break;
      case 81: /* unaryexp ::= NOT expression */
#line 512 "stparserimpl.lemon"
{
    yylhsminor.yy2 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Not,
                                               yymsp[0].minor.yy162);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy2->setSourceInfo(srcInfo);
}
#line 2010 "stparserimpl.c"
  yymsp[-1].minor.yy2 = yylhsminor.yy2;
        break;
      case 82: /* unaryexp ::= BIT_NOT expression */
#line 518 "stparserimpl.lemon"
{
    yylhsminor.yy2 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::BitNot,
                                               yymsp[0].minor.yy162);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy2->setSourceInfo(srcInfo);
}
#line 2021 "stparserimpl.c"
  yymsp[-1].minor.yy2 = yylhsminor.yy2;
        break;
      case 83: /* unaryexp ::= MINUS expression */
#line 524 "stparserimpl.lemon"
{
    yylhsminor.yy2 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Negate,
                                               yymsp[0].minor.yy162);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy2->setSourceInfo(srcInfo);
}
#line 2032 "stparserimpl.c"
  yymsp[-1].minor.yy2 = yylhsminor.yy2;
        break;
      case 84: /* unaryexp ::= REFERENCE expression */
#line 531 "stparserimpl.lemon"
{
    yylhsminor.yy2 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Reference,
                                               yymsp[0].minor.yy162);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy2->setSourceInfo(srcInfo);
}
#line 2043 "stparserimpl.c"
  yymsp[-1].minor.yy2 = yylhsminor.yy2;
        break;
      case 85: /* function_call_args ::= function_call_args COMMA expression */
#line 540 "stparserimpl.lemon"
{
    yylhsminor.yy70 = yymsp[-2].minor.yy70;
    yylhsminor.yy70->push_back(yymsp[0].minor.yy162);
}
#line 2052 "stparserimpl.c"
  yymsp[-2].minor.yy70 = yylhsminor.yy70;
        break;
      case 86: /* function_call_args ::= expression */
#line 544 "stparserimpl.lemon"
{
    yylhsminor.yy70 = pParser->createVector<const yal::frontend::STExpression*>();
    yylhsminor.yy70->push_back(yymsp[0].minor.yy162);
}
#line 2061 "stparserimpl.c"
  yymsp[0].minor.yy70 = yylhsminor.yy70;
        break;
      case 87: /* function_call_args ::= */
#line 548 "stparserimpl.lemon"
{
    yymsp[1].minor.yy70= nullptr;
}
#line 2069 "stparserimpl.c"
        break;
      case 88: /* struct_init ::= type_specifier SCOPE_BEGIN struct_member_init_list SCOPE_END */
#line 555 "stparserimpl.lemon"
{
    yylhsminor.yy100 = pParser->createNode<yal::frontend::STExprStructInit>(yymsp[-3].minor.yy74, yymsp[-1].minor.yy140);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-3].minor.yy74->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy100->setSourceInfo(srcInfo);
}
#line 2078 "stparserimpl.c"
  yymsp[-3].minor.yy100 = yylhsminor.yy100;
        break;
      case 89: /* struct_member_init_list ::= */
#line 561 "stparserimpl.lemon"
{yymsp[1].minor.yy140 = nullptr;}
#line 2084 "stparserimpl.c"
        break;
      case 90: /* struct_member_init_list ::= struct_member_init_list COMMA struct_member_init */
#line 562 "stparserimpl.lemon"
{
     yymsp[-2].minor.yy140->push_back(yymsp[0].minor.yy33);
     yylhsminor.yy140 = yymsp[-2].minor.yy140;
}
#line 2092 "stparserimpl.c"
  yymsp[-2].minor.yy140 = yylhsminor.yy140;
        break;
      case 91: /* struct_member_init_list ::= struct_member_init */
#line 566 "stparserimpl.lemon"
{
        yylhsminor.yy140 = pParser->createVector<const yal::frontend::STStructMemberInit*>();
        yylhsminor.yy140->push_back(yymsp[0].minor.yy33);
}
#line 2101 "stparserimpl.c"
  yymsp[0].minor.yy140 = yylhsminor.yy140;
        break;
      case 92: /* struct_member_init ::= identifier COLON expression */
#line 571 "stparserimpl.lemon"
{
    yylhsminor.yy33 = pParser->createNode<yal::frontend::STStructMemberInit>(yymsp[-2].minor.yy57,yymsp[0].minor.yy162);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy57->getSourceInfo(), yymsp[0].minor.yy162->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy33->setSourceInfo(srcInfo);
}
#line 2111 "stparserimpl.c"
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
#line 579 "stparserimpl.lemon"
{
        yylhsminor.yy162 = pParser->createNode<yal::frontend::STExprIntegerLiteral>(yymsp[0].minor.yy0);
        auto srcInfo =yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy162->setSourceInfo(srcInfo);
   }
#line 2129 "stparserimpl.c"
  yymsp[0].minor.yy162 = yylhsminor.yy162;
        break;
      case 102: /* literal ::= FLOAT_LITERAL */
      case 103: /* literal ::= FLOAT_LITERAL_32 */ yytestcase(yyruleno==103);
      case 104: /* literal ::= FLOAT_LITERAL_64 */ yytestcase(yyruleno==104);
#line 624 "stparserimpl.lemon"
{
        yylhsminor.yy162 = pParser->createNode<yal::frontend::STExprFloatLiteral>(yymsp[0].minor.yy0);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy162->setSourceInfo(srcInfo);
}
#line 2141 "stparserimpl.c"
  yymsp[0].minor.yy162 = yylhsminor.yy162;
        break;
      case 105: /* literal ::= BOOL_LITERAL */
#line 641 "stparserimpl.lemon"
{
        yylhsminor.yy162 = pParser->createNode<yal::frontend::STExprBoolLiteral>(yymsp[0].minor.yy0.tokenStr);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy162->setSourceInfo(srcInfo);
}
#line 2151 "stparserimpl.c"
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
#line 2235 "stparserimpl.c"
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
