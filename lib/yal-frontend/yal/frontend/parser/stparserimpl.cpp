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
#define YAL_TOKEN_IMPL                           51
#define YAL_TOKEN_RETURN                         52
#define YAL_TOKEN_VAR                            53
#define YAL_TOKEN_LET                            54
#define YAL_TOKEN_INTEGER_LITERAL                55
#define YAL_TOKEN_INTEGER_LITERAL_I8             56
#define YAL_TOKEN_INTEGER_LITERAL_I16            57
#define YAL_TOKEN_INTEGER_LITERAL_I32            58
#define YAL_TOKEN_INTEGER_LITERAL_I64            59
#define YAL_TOKEN_INTEGER_LITERAL_U8             60
#define YAL_TOKEN_INTEGER_LITERAL_U16            61
#define YAL_TOKEN_INTEGER_LITERAL_U32            62
#define YAL_TOKEN_INTEGER_LITERAL_U64            63
#define YAL_TOKEN_FLOAT_LITERAL                  64
#define YAL_TOKEN_FLOAT_LITERAL_32               65
#define YAL_TOKEN_FLOAT_LITERAL_64               66
#define YAL_TOKEN_BOOL_LITERAL                   67
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
#define YYNOCODE 104
#define YYACTIONTYPE unsigned short int
#if INTERFACE
#define YALSTParserTOKENTYPE yal::frontend::TokenInfo
#endif
typedef union {
  int yyinit;
  YALSTParserTOKENTYPE yy0;
  uint32_t yy13;
  yal::frontend::ParseListDeclParam::Range yy16;
  yal::frontend::STDeclFunction* yy20;
  yal::frontend::STStructMember* yy25;
  yal::frontend::STStatement* yy29;
  yal::frontend::STIdentifier* yy31;
  yal::frontend::STStructMemberInit* yy32;
  yal::frontend::ParseListStructMember::Range yy34;
  yal::frontend::STQualType* yy35;
  yal::frontend::STDeclParam* yy39;
  yal::frontend::STExpression* yy58;
  yal::frontend::ParseListExpr::Range yy70;
  yal::frontend::STType* yy72;
  yal::frontend::STDeclVar* yy79;
  yal::frontend::STExprBinaryOperator* yy114;
  yal::frontend::ParseListStmt::Range yy121;
  yal::frontend::ParseListDecl::Range yy122;
  yal::frontend::STDeclTypeFunctions* yy135;
  yal::frontend::STExprStructInit* yy136;
  yal::frontend::ParseListStructInit::Range yy140;
  yal::frontend::STDeclStruct* yy143;
  yal::frontend::STDeclModule* yy152;
  yal::frontend::STExprUnaryOperator* yy184;
  yal::frontend::STDecl* yy188;
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
#define YYNSTATE             114
#define YYNRULE              110
#define YY_MAX_SHIFT         113
#define YY_MIN_SHIFTREDUCE   188
#define YY_MAX_SHIFTREDUCE   297
#define YY_MIN_REDUCE        298
#define YY_MAX_REDUCE        407
#define YY_ERROR_ACTION      408
#define YY_ACCEPT_ACTION     409
#define YY_NO_ACTION         410
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
#define YY_ACTTAB_COUNT (1087)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    13,  111,  192,  109,   92,   14,   15,  192,  320,   83,
 /*    10 */   193,  199,  200,  201,  202,  203,  204,  205,  206,  207,
 /*    20 */   208,  209,  106,  212,   57,   12,   55,  105,  390,  111,
 /*    30 */     1,   86,  307,   30,   84,  337,  334,    7,   51,   49,
 /*    40 */   283,  284,  285,  286,  287,  288,  289,  290,  291,  292,
 /*    50 */   293,  294,  295,   13,   40,  298,  299,  113,   14,   15,
 /*    60 */   307,  326,   83,  193,  199,  200,  201,  202,  203,  204,
 /*    70 */   205,  206,  207,  208,  209,   58,  300,  111,   12,  324,
 /*    80 */    87,  241,   32,  409,   57,   34,   30,  106,  212,  335,
 /*    90 */   341,  192,   35,  283,  284,  285,  286,  287,  288,  289,
 /*   100 */   290,  291,  292,  293,  294,  295,  106,  212,  307,   13,
 /*   110 */    47,   25,   24,   23,   14,   15,   48,   53,   83,  193,
 /*   120 */   199,  200,  201,  202,  203,  204,  205,  206,  207,  208,
 /*   130 */   209,   55,  105,  107,   12,  355,  362,   35,   35,   16,
 /*   140 */   336,  255,   30,   92,  329,   85,  339,   88,   35,  283,
 /*   150 */   284,  285,  286,  287,  288,  289,  290,  291,  292,  293,
 /*   160 */   294,  295,   10,   41,   28,   29,   80,  391,   50,   22,
 /*   170 */    21,   17,   19,   18,   20,   27,   26,   25,   24,   23,
 /*   180 */    41,   28,   29,   53,  278,   45,   22,   21,   17,   19,
 /*   190 */    18,   20,   27,   26,   25,   24,   23,  330,  223,   35,
 /*   200 */    53,   97,   57,  198,  239,  199,  200,  201,  202,  203,
 /*   210 */   204,  205,  206,  207,  208,  209,  107,   16,   16,  256,
 /*   220 */   257,  240,  102,  108,   41,   28,   29,  328,  217,   46,
 /*   230 */    22,   21,   17,   19,   18,   20,   27,   26,   25,   24,
 /*   240 */    23,   41,   28,   29,   53,   39,  104,   22,   21,   17,
 /*   250 */    19,   18,   20,   27,   26,   25,   24,   23,  193,   98,
 /*   260 */   308,   53,   37,  308,  340,  237,   36,   54,  221,   31,
 /*   270 */    56,  112,  111,   44,   82,    8,  103,  305,  363,   90,
 /*   280 */     9,  242,  222,  238,   11,   53,    4,   52,  247,    5,
 /*   290 */     3,   89,  346,   59,  359,  360,  358,    6,  361,    2,
 /*   300 */    42,  213,  111,  307,   82,   38,  103,  305,  363,   90,
 /*   310 */    43,  297,  215,  110,   33,  406,  300,  300,  300,  300,
 /*   320 */     3,   91,  346,   59,  359,  360,  358,  300,  361,   41,
 /*   330 */    28,   29,  300,  307,  300,   22,   21,   17,   19,   18,
 /*   340 */    20,   27,   26,   25,   24,   23,  111,  300,   82,   53,
 /*   350 */   103,  305,  363,   90,  300,  300,  300,  300,  300,  300,
 /*   360 */   300,  300,  300,  300,  300,  300,  345,   59,  359,  360,
 /*   370 */   358,  300,  361,  300,   29,  300,  300,  307,   22,   21,
 /*   380 */    17,   19,   18,   20,   27,   26,   25,   24,   23,  300,
 /*   390 */   300,  300,   53,  111,  300,   82,  300,  103,  305,  363,
 /*   400 */   300,  300,  300,  300,  300,  111,  300,   82,  300,  103,
 /*   410 */   305,  363,  300,  300,   67,  359,  360,  358,   81,  361,
 /*   420 */   300,  300,  300,  300,  307,  300,   67,  359,  360,  358,
 /*   430 */    79,  361,  300,  300,  300,  300,  307,  300,  111,  300,
 /*   440 */    82,  300,  103,  305,  363,  300,  300,  300,  300,  300,
 /*   450 */   300,  300,  300,  300,  300,  300,  300,  300,  300,   67,
 /*   460 */   359,  360,  358,   78,  361,  300,  300,  300,  198,  307,
 /*   470 */   199,  200,  201,  202,  203,  204,  205,  206,  207,  208,
 /*   480 */   209,  408,  408,  408,  408,  408,  408,   27,   26,   25,
 /*   490 */    24,   23,  300,  300,  300,   53,   22,   21,   17,   19,
 /*   500 */    18,   20,   27,   26,   25,   24,   23,  300,  300,  300,
 /*   510 */    53,  300,  111,  300,   82,  300,  103,  305,  363,  300,
 /*   520 */   300,  300,  300,  300,  300,  300,  300,  300,  300,  300,
 /*   530 */   300,  300,  300,   60,  359,  360,  358,  300,  361,  300,
 /*   540 */   111,  300,   82,  307,  103,  305,  363,  300,  300,  300,
 /*   550 */   300,  111,  300,   82,  300,  103,  305,  363,  300,  300,
 /*   560 */   300,   63,  359,  360,  358,  300,  361,  300,  300,  300,
 /*   570 */   300,  307,   64,  359,  360,  358,  300,  361,  300,  300,
 /*   580 */   300,  300,  307,  300,  111,  300,   82,  300,  103,  305,
 /*   590 */   363,  300,  300,  300,  300,  300,  300,  300,  300,  300,
 /*   600 */   300,  300,  300,  300,  300,   61,  359,  360,  358,  300,
 /*   610 */   361,  300,  111,  300,   82,  307,  103,  305,  363,  300,
 /*   620 */   300,  300,  300,  111,  300,   82,  300,  103,  305,  363,
 /*   630 */   300,  300,  300,   65,  359,  360,  358,  300,  361,  300,
 /*   640 */   300,  300,  300,  307,   93,  359,  360,  358,  300,  361,
 /*   650 */   300,  300,  300,  300,  307,  300,  111,  300,   82,  300,
 /*   660 */   103,  305,  363,  300,  300,  300,  300,  300,  300,  300,
 /*   670 */   300,  300,  300,  300,  300,  300,  300,   94,  359,  360,
 /*   680 */   358,  300,  361,  300,  111,  300,   82,  307,  103,  305,
 /*   690 */   363,  300,  300,  300,  300,  111,  300,   82,  300,  103,
 /*   700 */   305,  363,  300,  300,  300,   95,  359,  360,  358,  300,
 /*   710 */   361,  300,  300,  300,  300,  307,   96,  359,  360,  358,
 /*   720 */   300,  361,  300,  300,  300,  300,  307,  300,  111,  300,
 /*   730 */    82,  300,  103,  305,  363,  300,  300,  300,  300,  300,
 /*   740 */   300,  300,  300,  300,  300,  300,  300,  300,  300,   66,
 /*   750 */   359,  360,  358,  300,  361,  300,  111,  300,   82,  307,
 /*   760 */   103,  305,  363,  300,  300,  300,  300,  111,  300,   82,
 /*   770 */   300,  103,  305,  363,  300,  300,  300,   69,  359,  360,
 /*   780 */   358,  300,  361,  300,  300,  300,  300,  307,   70,  359,
 /*   790 */   360,  358,  300,  361,  300,  300,  300,  300,  307,  300,
 /*   800 */   111,  300,   82,  300,  103,  305,  363,  300,  300,  300,
 /*   810 */   300,  300,  300,  300,  300,  300,  300,  300,  300,  300,
 /*   820 */   300,   71,  359,  360,  358,  300,  361,  300,  111,  300,
 /*   830 */    82,  307,  103,  305,  363,  300,  300,  300,  300,  111,
 /*   840 */   300,   82,  300,  103,  305,  363,  300,  300,  300,   72,
 /*   850 */   359,  360,  358,  300,  361,  300,  300,  300,  300,  307,
 /*   860 */    73,  359,  360,  358,  300,  361,  300,  300,  300,  300,
 /*   870 */   307,  300,  111,  300,   82,  300,  103,  305,  363,  300,
 /*   880 */   300,  300,  300,  300,  300,  300,  300,  300,  300,  300,
 /*   890 */   300,  300,  300,   74,  359,  360,  358,  300,  361,  300,
 /*   900 */   111,  300,   82,  307,  103,  305,  363,  300,  300,  300,
 /*   910 */   300,  111,  300,   82,  300,  103,  305,  363,  300,  300,
 /*   920 */   300,   99,  359,  360,  358,  300,  361,  300,  300,  300,
 /*   930 */   300,  307,  100,  359,  360,  358,  300,  361,  300,  300,
 /*   940 */   300,  300,  307,  300,  111,  300,   82,  300,  103,  305,
 /*   950 */   363,  300,  300,  300,  300,  300,  300,  300,  300,  300,
 /*   960 */   300,  300,  300,  300,  300,  101,  359,  360,  358,  300,
 /*   970 */   361,  300,  111,  300,   82,  307,  103,  305,  363,  300,
 /*   980 */   300,  300,  300,  111,  300,   82,  300,  103,  305,  363,
 /*   990 */   300,  300,  300,   76,  359,  360,  358,  300,  361,  300,
 /*  1000 */   300,  300,  300,  307,   77,  359,  360,  358,  300,  361,
 /*  1010 */   300,  300,  300,  300,  307,  300,  111,  300,   82,  300,
 /*  1020 */   103,  305,  363,  300,  300,  300,  300,  300,  300,  300,
 /*  1030 */   300,  300,  300,  300,  300,  300,  300,   68,  359,  360,
 /*  1040 */   358,  300,  361,  300,  111,  300,   82,  307,  103,  305,
 /*  1050 */   363,  300,  300,  300,  300,  111,  300,   82,  300,  103,
 /*  1060 */   305,  363,  300,  300,  300,   75,  359,  360,  358,  300,
 /*  1070 */   361,  300,  300,  300,  300,  307,   62,  359,  360,  358,
 /*  1080 */   300,  361,  300,  300,  300,  300,  307,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    15,   71,   24,   73,   75,   20,   21,   24,   25,   24,
 /*    10 */    25,   26,   27,   28,   29,   30,   31,   32,   33,   34,
 /*    20 */    35,   36,   39,   40,   50,   40,   74,   75,   99,   71,
 /*    30 */    45,   73,  102,   48,   82,   83,   80,   52,   53,   54,
 /*    40 */    55,   56,   57,   58,   59,   60,   61,   62,   63,   64,
 /*    50 */    65,   66,   67,   15,   42,   79,   80,   23,   20,   21,
 /*    60 */   102,   85,   24,   25,   26,   27,   28,   29,   30,   31,
 /*    70 */    32,   33,   34,   35,   36,   41,  100,   71,   40,   73,
 /*    80 */    88,   43,   69,   70,   50,   51,   48,   39,   40,   80,
 /*    90 */    72,   24,   74,   55,   56,   57,   58,   59,   60,   61,
 /*   100 */    62,   63,   64,   65,   66,   67,   39,   40,  102,   15,
 /*   110 */   101,   16,   17,   18,   20,   21,   75,   22,   24,   25,
 /*   120 */    26,   27,   28,   29,   30,   31,   32,   33,   34,   35,
 /*   130 */    36,   74,   75,   75,   40,   72,   72,   74,   74,   47,
 /*   140 */    83,   49,   48,   75,   86,   87,   72,   88,   74,   55,
 /*   150 */    56,   57,   58,   59,   60,   61,   62,   63,   64,   65,
 /*   160 */    66,   67,    1,    2,    3,    4,   98,   99,   75,    8,
 /*   170 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*   180 */     2,    3,    4,   22,   46,   47,    8,    9,   10,   11,
 /*   190 */    12,   13,   14,   15,   16,   17,   18,   72,   46,   74,
 /*   200 */    22,   75,   50,   24,   43,   26,   27,   28,   29,   30,
 /*   210 */    31,   32,   33,   34,   35,   36,   75,   47,   47,   49,
 /*   220 */    49,   43,   75,   44,    2,    3,    4,   86,   46,   47,
 /*   230 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*   240 */    18,    2,    3,    4,   22,   42,   81,    8,    9,   10,
 /*   250 */    11,   12,   13,   14,   15,   16,   17,   18,   25,   42,
 /*   260 */    42,   22,   45,   45,   76,   43,   47,   84,   49,   48,
 /*   270 */    75,   75,   71,   45,   73,    1,   75,   76,   77,   78,
 /*   280 */     1,   46,   46,   43,   42,   22,   48,   42,   49,   48,
 /*   290 */    89,   90,   91,   92,   93,   94,   95,   48,   97,   45,
 /*   300 */    42,   40,   71,  102,   73,   45,   75,   76,   77,   78,
 /*   310 */    42,   38,   43,   37,   42,    0,  103,  103,  103,  103,
 /*   320 */    89,   90,   91,   92,   93,   94,   95,  103,   97,    2,
 /*   330 */     3,    4,  103,  102,  103,    8,    9,   10,   11,   12,
 /*   340 */    13,   14,   15,   16,   17,   18,   71,  103,   73,   22,
 /*   350 */    75,   76,   77,   78,  103,  103,  103,  103,  103,  103,
 /*   360 */   103,  103,  103,  103,  103,  103,   91,   92,   93,   94,
 /*   370 */    95,  103,   97,  103,    4,  103,  103,  102,    8,    9,
 /*   380 */    10,   11,   12,   13,   14,   15,   16,   17,   18,  103,
 /*   390 */   103,  103,   22,   71,  103,   73,  103,   75,   76,   77,
 /*   400 */   103,  103,  103,  103,  103,   71,  103,   73,  103,   75,
 /*   410 */    76,   77,  103,  103,   92,   93,   94,   95,   96,   97,
 /*   420 */   103,  103,  103,  103,  102,  103,   92,   93,   94,   95,
 /*   430 */    96,   97,  103,  103,  103,  103,  102,  103,   71,  103,
 /*   440 */    73,  103,   75,   76,   77,  103,  103,  103,  103,  103,
 /*   450 */   103,  103,  103,  103,  103,  103,  103,  103,  103,   92,
 /*   460 */    93,   94,   95,   96,   97,  103,  103,  103,   24,  102,
 /*   470 */    26,   27,   28,   29,   30,   31,   32,   33,   34,   35,
 /*   480 */    36,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*   490 */    17,   18,  103,  103,  103,   22,    8,    9,   10,   11,
 /*   500 */    12,   13,   14,   15,   16,   17,   18,  103,  103,  103,
 /*   510 */    22,  103,   71,  103,   73,  103,   75,   76,   77,  103,
 /*   520 */   103,  103,  103,  103,  103,  103,  103,  103,  103,  103,
 /*   530 */   103,  103,  103,   92,   93,   94,   95,  103,   97,  103,
 /*   540 */    71,  103,   73,  102,   75,   76,   77,  103,  103,  103,
 /*   550 */   103,   71,  103,   73,  103,   75,   76,   77,  103,  103,
 /*   560 */   103,   92,   93,   94,   95,  103,   97,  103,  103,  103,
 /*   570 */   103,  102,   92,   93,   94,   95,  103,   97,  103,  103,
 /*   580 */   103,  103,  102,  103,   71,  103,   73,  103,   75,   76,
 /*   590 */    77,  103,  103,  103,  103,  103,  103,  103,  103,  103,
 /*   600 */   103,  103,  103,  103,  103,   92,   93,   94,   95,  103,
 /*   610 */    97,  103,   71,  103,   73,  102,   75,   76,   77,  103,
 /*   620 */   103,  103,  103,   71,  103,   73,  103,   75,   76,   77,
 /*   630 */   103,  103,  103,   92,   93,   94,   95,  103,   97,  103,
 /*   640 */   103,  103,  103,  102,   92,   93,   94,   95,  103,   97,
 /*   650 */   103,  103,  103,  103,  102,  103,   71,  103,   73,  103,
 /*   660 */    75,   76,   77,  103,  103,  103,  103,  103,  103,  103,
 /*   670 */   103,  103,  103,  103,  103,  103,  103,   92,   93,   94,
 /*   680 */    95,  103,   97,  103,   71,  103,   73,  102,   75,   76,
 /*   690 */    77,  103,  103,  103,  103,   71,  103,   73,  103,   75,
 /*   700 */    76,   77,  103,  103,  103,   92,   93,   94,   95,  103,
 /*   710 */    97,  103,  103,  103,  103,  102,   92,   93,   94,   95,
 /*   720 */   103,   97,  103,  103,  103,  103,  102,  103,   71,  103,
 /*   730 */    73,  103,   75,   76,   77,  103,  103,  103,  103,  103,
 /*   740 */   103,  103,  103,  103,  103,  103,  103,  103,  103,   92,
 /*   750 */    93,   94,   95,  103,   97,  103,   71,  103,   73,  102,
 /*   760 */    75,   76,   77,  103,  103,  103,  103,   71,  103,   73,
 /*   770 */   103,   75,   76,   77,  103,  103,  103,   92,   93,   94,
 /*   780 */    95,  103,   97,  103,  103,  103,  103,  102,   92,   93,
 /*   790 */    94,   95,  103,   97,  103,  103,  103,  103,  102,  103,
 /*   800 */    71,  103,   73,  103,   75,   76,   77,  103,  103,  103,
 /*   810 */   103,  103,  103,  103,  103,  103,  103,  103,  103,  103,
 /*   820 */   103,   92,   93,   94,   95,  103,   97,  103,   71,  103,
 /*   830 */    73,  102,   75,   76,   77,  103,  103,  103,  103,   71,
 /*   840 */   103,   73,  103,   75,   76,   77,  103,  103,  103,   92,
 /*   850 */    93,   94,   95,  103,   97,  103,  103,  103,  103,  102,
 /*   860 */    92,   93,   94,   95,  103,   97,  103,  103,  103,  103,
 /*   870 */   102,  103,   71,  103,   73,  103,   75,   76,   77,  103,
 /*   880 */   103,  103,  103,  103,  103,  103,  103,  103,  103,  103,
 /*   890 */   103,  103,  103,   92,   93,   94,   95,  103,   97,  103,
 /*   900 */    71,  103,   73,  102,   75,   76,   77,  103,  103,  103,
 /*   910 */   103,   71,  103,   73,  103,   75,   76,   77,  103,  103,
 /*   920 */   103,   92,   93,   94,   95,  103,   97,  103,  103,  103,
 /*   930 */   103,  102,   92,   93,   94,   95,  103,   97,  103,  103,
 /*   940 */   103,  103,  102,  103,   71,  103,   73,  103,   75,   76,
 /*   950 */    77,  103,  103,  103,  103,  103,  103,  103,  103,  103,
 /*   960 */   103,  103,  103,  103,  103,   92,   93,   94,   95,  103,
 /*   970 */    97,  103,   71,  103,   73,  102,   75,   76,   77,  103,
 /*   980 */   103,  103,  103,   71,  103,   73,  103,   75,   76,   77,
 /*   990 */   103,  103,  103,   92,   93,   94,   95,  103,   97,  103,
 /*  1000 */   103,  103,  103,  102,   92,   93,   94,   95,  103,   97,
 /*  1010 */   103,  103,  103,  103,  102,  103,   71,  103,   73,  103,
 /*  1020 */    75,   76,   77,  103,  103,  103,  103,  103,  103,  103,
 /*  1030 */   103,  103,  103,  103,  103,  103,  103,   92,   93,   94,
 /*  1040 */    95,  103,   97,  103,   71,  103,   73,  102,   75,   76,
 /*  1050 */    77,  103,  103,  103,  103,   71,  103,   73,  103,   75,
 /*  1060 */    76,   77,  103,  103,  103,   92,   93,   94,   95,  103,
 /*  1070 */    97,  103,  103,  103,  103,  102,   92,   93,   94,   95,
 /*  1080 */   103,   97,  103,  103,  103,  103,  102,
};
#define YY_SHIFT_USE_DFLT (1087)
#define YY_SHIFT_COUNT    (113)
#define YY_SHIFT_MIN      (-26)
#define YY_SHIFT_MAX      (488)
static const short yy_shift_ofst[] = {
 /*     0 */  1087,  -15,  -15,  -15,   94,   94,   94,   38,   94,   94,
 /*    10 */    94,   94,   94,   94,   94,   94,   94,   94,   94,   94,
 /*    20 */    94,   94,   94,   94,   94,   94,   94,   94,   94,   94,
 /*    30 */    94,  -17,   34,  179,  444,  444,   67,  -22,  -22,   48,
 /*    40 */    48,   48,   48,   48,  -26,  -22,  -22,  152,   12,  -22,
 /*    50 */    12,  -22,  -22,  -22,  203,  233,  221,  -22,  -22,  161,
 /*    60 */   178,  222,  239,  327,  327,  327,  327,  327,  370,  473,
 /*    70 */   473,  473,  473,  473,  473,  488,   95,   95,   92,  170,
 /*    80 */   138,  171,  217,  218,  219,  182,  228,  274,  279,  235,
 /*    90 */   240,  236,  242,  263,  263,  263,  263,  238,  245,  263,
 /*   100 */   263,  263,  241,  249,  254,  258,  261,  268,  260,  269,
 /*   110 */   273,  276,  272,  315,
};
#define YY_REDUCE_USE_DFLT (-72)
#define YY_REDUCE_COUNT (58)
#define YY_REDUCE_MIN   (-71)
#define YY_REDUCE_MAX   (984)
static const short yy_reduce_ofst[] = {
 /*     0 */    13,  201,  231,  275,  322,  334,  367,  441,  469,  480,
 /*    10 */   513,  541,  552,  585,  613,  624,  657,  685,  696,  729,
 /*    20 */   757,  768,  801,  829,  840,  873,  901,  912,  945,  973,
 /*    30 */   984,  -48,  -24,  -70,  -42,    6,   57,   68,   58,   18,
 /*    40 */    63,   64,   74,  125,    9,  -71,  141,  -44,   -8,   41,
 /*    50 */    59,   93,  126,  147,  165,  188,  183,  195,  196,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   301,  343,  343,  344,  387,  387,  387,  408,  408,  408,
 /*    10 */   408,  408,  408,  408,  408,  408,  408,  408,  408,  408,
 /*    20 */   408,  408,  408,  408,  408,  408,  408,  408,  408,  408,
 /*    30 */   408,  338,  408,  408,  408,  408,  320,  389,  408,  320,
 /*    40 */   320,  320,  320,  320,  408,  408,  408,  408,  356,  408,
 /*    50 */   356,  408,  408,  408,  342,  408,  408,  408,  408,  408,
 /*    60 */   408,  408,  408,  354,  353,  392,  385,  386,  369,  380,
 /*    70 */   379,  378,  377,  376,  375,  368,  371,  370,  408,  408,
 /*    80 */   408,  408,  408,  302,  408,  408,  408,  408,  408,  408,
 /*    90 */   408,  408,  408,  384,  383,  382,  381,  408,  408,  374,
 /*   100 */   373,  372,  364,  304,  408,  408,  321,  408,  408,  408,
 /*   110 */   408,  306,  408,  408,
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
  "PAR_BEGIN",     "PAR_END",       "FUNCTION",      "IMPL",        
  "RETURN",        "VAR",           "LET",           "INTEGER_LITERAL",
  "INTEGER_LITERAL_I8",  "INTEGER_LITERAL_I16",  "INTEGER_LITERAL_I32",  "INTEGER_LITERAL_I64",
  "INTEGER_LITERAL_U8",  "INTEGER_LITERAL_U16",  "INTEGER_LITERAL_U32",  "INTEGER_LITERAL_U64",
  "FLOAT_LITERAL",  "FLOAT_LITERAL_32",  "FLOAT_LITERAL_64",  "BOOL_LITERAL",
  "error",         "decls",         "module",        "type_builtin",
  "type_qualified",  "type_specifier",  "qualifier",     "identifier",  
  "self",          "identifier_or_self",  "decl_var",      "decl_type",   
  "decl_function",  "function_return_decl",  "function_args_decl",  "function_arg_decl",
  "function_param_list",  "decl_struct",   "struct_decl_var",  "struct_decl_vars",
  "var_type_spec",  "statement_list",  "statement_list_or_empty",  "statement",   
  "expression",    "unaryexp",      "binaryexp",     "literal",     
  "function_call_args",  "struct_init",   "struct_member_init_list",  "struct_member_init",
  "decl_type_functions_scope",  "decl_type_function_list",  "type_array",  
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "decls ::= decls decl_type",
 /*   1 */ "decls ::= decls decl_function",
 /*   2 */ "decls ::= decls decl_type_functions_scope",
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
 /*  35 */ "decl_type_functions_scope ::= IMPL type_specifier SCOPE_BEGIN decl_type_function_list SCOPE_END",
 /*  36 */ "decl_type_function_list ::= decl_type_function_list decl_function",
 /*  37 */ "decl_type_function_list ::= decl_function",
 /*  38 */ "function_args_decl ::= function_args_decl COMMA function_arg_decl",
 /*  39 */ "function_args_decl ::= function_arg_decl",
 /*  40 */ "function_args_decl ::=",
 /*  41 */ "function_arg_decl ::= identifier COLON type_qualified",
 /*  42 */ "function_arg_decl ::= qualifier self",
 /*  43 */ "function_return_decl ::= COLON type_qualified",
 /*  44 */ "function_return_decl ::=",
 /*  45 */ "statement_list_or_empty ::=",
 /*  46 */ "statement_list_or_empty ::= statement_list",
 /*  47 */ "statement_list ::= statement_list statement",
 /*  48 */ "statement_list ::= statement",
 /*  49 */ "statement ::= expression ASSIGN expression SEMI_COLON",
 /*  50 */ "statement ::= decl_var SEMI_COLON",
 /*  51 */ "statement ::= expression SEMI_COLON",
 /*  52 */ "statement ::= RETURN expression SEMI_COLON",
 /*  53 */ "statement ::= RETURN SEMI_COLON",
 /*  54 */ "statement ::= SCOPE_BEGIN statement_list_or_empty SCOPE_END",
 /*  55 */ "decl_var ::= VAR identifier var_type_spec ASSIGN expression",
 /*  56 */ "decl_var ::= LET identifier var_type_spec ASSIGN expression",
 /*  57 */ "var_type_spec ::= COLON type_qualified",
 /*  58 */ "var_type_spec ::=",
 /*  59 */ "expression ::= PAR_BEGIN expression PAR_END",
 /*  60 */ "expression ::= literal",
 /*  61 */ "expression ::= unaryexp",
 /*  62 */ "expression ::= binaryexp",
 /*  63 */ "expression ::= struct_init",
 /*  64 */ "expression ::= expression AS type_qualified",
 /*  65 */ "expression ::= identifier_or_self",
 /*  66 */ "expression ::= expression DOT identifier",
 /*  67 */ "expression ::= identifier PAR_BEGIN function_call_args PAR_END",
 /*  68 */ "expression ::= expression DOT identifier PAR_BEGIN function_call_args PAR_END",
 /*  69 */ "expression ::= type_specifier COLON COLON identifier PAR_BEGIN function_call_args PAR_END",
 /*  70 */ "binaryexp ::= expression AND expression",
 /*  71 */ "binaryexp ::= expression OR expression",
 /*  72 */ "binaryexp ::= expression PLUS expression",
 /*  73 */ "binaryexp ::= expression MINUS expression",
 /*  74 */ "binaryexp ::= expression MULT expression",
 /*  75 */ "binaryexp ::= expression DIV expression",
 /*  76 */ "binaryexp ::= expression MOD expression",
 /*  77 */ "binaryexp ::= expression EQ expression",
 /*  78 */ "binaryexp ::= expression NE expression",
 /*  79 */ "binaryexp ::= expression LE expression",
 /*  80 */ "binaryexp ::= expression LT expression",
 /*  81 */ "binaryexp ::= expression GE expression",
 /*  82 */ "binaryexp ::= expression GT expression",
 /*  83 */ "unaryexp ::= NOT expression",
 /*  84 */ "unaryexp ::= BIT_NOT expression",
 /*  85 */ "unaryexp ::= MINUS expression",
 /*  86 */ "unaryexp ::= REFERENCE expression",
 /*  87 */ "function_call_args ::= function_call_args COMMA expression",
 /*  88 */ "function_call_args ::= expression",
 /*  89 */ "function_call_args ::=",
 /*  90 */ "struct_init ::= type_specifier SCOPE_BEGIN struct_member_init_list SCOPE_END",
 /*  91 */ "struct_member_init_list ::=",
 /*  92 */ "struct_member_init_list ::= struct_member_init_list COMMA struct_member_init",
 /*  93 */ "struct_member_init_list ::= struct_member_init",
 /*  94 */ "struct_member_init ::= identifier COLON expression",
 /*  95 */ "literal ::= INTEGER_LITERAL",
 /*  96 */ "literal ::= INTEGER_LITERAL_I8",
 /*  97 */ "literal ::= INTEGER_LITERAL_I16",
 /*  98 */ "literal ::= INTEGER_LITERAL_I32",
 /*  99 */ "literal ::= INTEGER_LITERAL_I64",
 /* 100 */ "literal ::= INTEGER_LITERAL_U8",
 /* 101 */ "literal ::= INTEGER_LITERAL_U16",
 /* 102 */ "literal ::= INTEGER_LITERAL_U32",
 /* 103 */ "literal ::= INTEGER_LITERAL_U64",
 /* 104 */ "literal ::= FLOAT_LITERAL",
 /* 105 */ "literal ::= FLOAT_LITERAL_32",
 /* 106 */ "literal ::= FLOAT_LITERAL_64",
 /* 107 */ "literal ::= BOOL_LITERAL",
 /* 108 */ "module ::= decls END",
 /* 109 */ "type_array ::= type_builtin ARRAY_BEGIN ARRAY_END",
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
  { 69, -2 },
  { 69, -2 },
  { 69, -2 },
  { 69, 0 },
  { 75, -1 },
  { 76, -1 },
  { 77, -1 },
  { 77, -1 },
  { 73, -1 },
  { 73, -1 },
  { 73, -1 },
  { 71, -1 },
  { 71, -1 },
  { 71, -1 },
  { 71, -1 },
  { 71, -1 },
  { 71, -1 },
  { 71, -1 },
  { 71, -1 },
  { 71, -1 },
  { 71, -1 },
  { 71, -1 },
  { 74, 0 },
  { 74, -1 },
  { 74, -1 },
  { 74, -2 },
  { 72, -2 },
  { 79, -5 },
  { 79, -1 },
  { 85, -7 },
  { 87, -3 },
  { 87, -1 },
  { 86, -3 },
  { 84, -3 },
  { 80, -7 },
  { 100, -5 },
  { 101, -2 },
  { 101, -1 },
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
  { 78, -5 },
  { 78, -5 },
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
  { 70, -2 },
  { 102, -3 },
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
#line 123 "stparserimpl.lemon"
{
    yylhsminor.yy152 =yymsp[-1].minor.yy152; yymsp[-1].minor.yy152->addDecl(yymsp[0].minor.yy188);
}
#line 1355 "stparserimpl.c"
  yymsp[-1].minor.yy152 = yylhsminor.yy152;
        break;
      case 1: /* decls ::= decls decl_function */
#line 126 "stparserimpl.lemon"
{
        yylhsminor.yy152 =yymsp[-1].minor.yy152; yymsp[-1].minor.yy152->addDecl(yymsp[0].minor.yy20);
}
#line 1363 "stparserimpl.c"
  yymsp[-1].minor.yy152 = yylhsminor.yy152;
        break;
      case 2: /* decls ::= decls decl_type_functions_scope */
#line 129 "stparserimpl.lemon"
{
    yylhsminor.yy152 =yymsp[-1].minor.yy152; yymsp[-1].minor.yy152->addDecl(yymsp[0].minor.yy135);
}
#line 1371 "stparserimpl.c"
  yymsp[-1].minor.yy152 = yylhsminor.yy152;
        break;
      case 3: /* decls ::= */
#line 132 "stparserimpl.lemon"
{
        yymsp[1].minor.yy152 =  pParser->getDeclModule();
}
#line 1379 "stparserimpl.c"
        break;
      case 4: /* identifier ::= IDENTIFIER */
      case 5: /* self ::= SELF */ yytestcase(yyruleno==5);
#line 139 "stparserimpl.lemon"
{
        yylhsminor.yy31 = pParser->createNode<yal::frontend::STIdentifier>(yymsp[0].minor.yy0.tokenStr);
        auto srcInfo = CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy31->setSourceInfo(srcInfo);
}
#line 1389 "stparserimpl.c"
  yymsp[0].minor.yy31 = yylhsminor.yy31;
        break;
      case 6: /* identifier_or_self ::= identifier */
      case 7: /* identifier_or_self ::= self */ yytestcase(yyruleno==7);
#line 151 "stparserimpl.lemon"
{yylhsminor.yy31=yymsp[0].minor.yy31;}
#line 1396 "stparserimpl.c"
  yymsp[0].minor.yy31 = yylhsminor.yy31;
        break;
      case 8: /* type_specifier ::= type_builtin */
#line 154 "stparserimpl.lemon"
{
        yylhsminor.yy72=yymsp[0].minor.yy72;
}
#line 1404 "stparserimpl.c"
  yymsp[0].minor.yy72 = yylhsminor.yy72;
        break;
      case 9: /* type_specifier ::= type_array */
#line 158 "stparserimpl.lemon"
{yymsp[0].minor.yy72 = nullptr;}
#line 1410 "stparserimpl.c"
        break;
      case 10: /* type_specifier ::= IDENTIFIER */
#line 159 "stparserimpl.lemon"
{
    yylhsminor.yy72 = pParser->createNode<yal::frontend::STType>(yymsp[0].minor.yy0.tokenStr);
    yylhsminor.yy72->setSourceInfo(yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0,yymsp[0].minor.yy0, pParser->getSourceHandle()));
}
#line 1418 "stparserimpl.c"
  yymsp[0].minor.yy72 = yylhsminor.yy72;
        break;
      case 11: /* type_builtin ::= TYPE_BOOL */
#line 164 "stparserimpl.lemon"
{
    yylhsminor.yy72 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Bool);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy72->setSourceInfo(srcInfo);
    }
#line 1428 "stparserimpl.c"
  yymsp[0].minor.yy72 = yylhsminor.yy72;
        break;
      case 12: /* type_builtin ::= TYPE_INT8 */
#line 169 "stparserimpl.lemon"
{
        yylhsminor.yy72 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int8);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy72->setSourceInfo(srcInfo);
    }
#line 1438 "stparserimpl.c"
  yymsp[0].minor.yy72 = yylhsminor.yy72;
        break;
      case 13: /* type_builtin ::= TYPE_UINT8 */
#line 174 "stparserimpl.lemon"
{
        yylhsminor.yy72 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt8);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy72->setSourceInfo(srcInfo);
    }
#line 1448 "stparserimpl.c"
  yymsp[0].minor.yy72 = yylhsminor.yy72;
        break;
      case 14: /* type_builtin ::= TYPE_INT16 */
#line 179 "stparserimpl.lemon"
{
        yylhsminor.yy72 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int16);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy72->setSourceInfo(srcInfo);
    }
#line 1458 "stparserimpl.c"
  yymsp[0].minor.yy72 = yylhsminor.yy72;
        break;
      case 15: /* type_builtin ::= TYPE_UINT16 */
#line 184 "stparserimpl.lemon"
{
        yylhsminor.yy72 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt16);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy72->setSourceInfo(srcInfo);
    }
#line 1468 "stparserimpl.c"
  yymsp[0].minor.yy72 = yylhsminor.yy72;
        break;
      case 16: /* type_builtin ::= TYPE_INT32 */
#line 189 "stparserimpl.lemon"
{
        yylhsminor.yy72 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int32);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy72->setSourceInfo(srcInfo);
    }
#line 1478 "stparserimpl.c"
  yymsp[0].minor.yy72 = yylhsminor.yy72;
        break;
      case 17: /* type_builtin ::= TYPE_UINT32 */
#line 194 "stparserimpl.lemon"
{
        yylhsminor.yy72 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt32);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy72->setSourceInfo(srcInfo);
    }
#line 1488 "stparserimpl.c"
  yymsp[0].minor.yy72 = yylhsminor.yy72;
        break;
      case 18: /* type_builtin ::= TYPE_INT64 */
#line 199 "stparserimpl.lemon"
{
        yylhsminor.yy72 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Int64);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy72->setSourceInfo(srcInfo);
    }
#line 1498 "stparserimpl.c"
  yymsp[0].minor.yy72 = yylhsminor.yy72;
        break;
      case 19: /* type_builtin ::= TYPE_UINT64 */
#line 204 "stparserimpl.lemon"
{
        yylhsminor.yy72 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::UInt64);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy72->setSourceInfo(srcInfo);
    }
#line 1508 "stparserimpl.c"
  yymsp[0].minor.yy72 = yylhsminor.yy72;
        break;
      case 20: /* type_builtin ::= TYPE_FLOAT */
#line 209 "stparserimpl.lemon"
{
        yylhsminor.yy72 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Float32);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy72->setSourceInfo(srcInfo);
    }
#line 1518 "stparserimpl.c"
  yymsp[0].minor.yy72 = yylhsminor.yy72;
        break;
      case 21: /* type_builtin ::= TYPE_DOUBLE */
#line 214 "stparserimpl.lemon"
{
        yylhsminor.yy72 = pParser->createNode<yal::frontend::STType>(yal::frontend::STType::Type::Float64);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy72->setSourceInfo(srcInfo);
    }
#line 1528 "stparserimpl.c"
  yymsp[0].minor.yy72 = yylhsminor.yy72;
        break;
      case 22: /* qualifier ::= */
#line 222 "stparserimpl.lemon"
{yymsp[1].minor.yy13 = 0;}
#line 1534 "stparserimpl.c"
        break;
      case 23: /* qualifier ::= MUT */
#line 223 "stparserimpl.lemon"
{yymsp[0].minor.yy13 = yal::frontend::STQualType::Qualifiers::kQualMutable;}
#line 1539 "stparserimpl.c"
        break;
      case 24: /* qualifier ::= REFERENCE */
#line 224 "stparserimpl.lemon"
{yymsp[0].minor.yy13 = yal::frontend::STQualType::Qualifiers::kQualReference;}
#line 1544 "stparserimpl.c"
        break;
      case 25: /* qualifier ::= MUT REFERENCE */
#line 225 "stparserimpl.lemon"
{
        yymsp[-1].minor.yy13 = yal::frontend::STQualType::Qualifiers::kQualReference | yal::frontend::STQualType::Qualifiers::kQualMutable;
 }
#line 1551 "stparserimpl.c"
        break;
      case 26: /* type_qualified ::= qualifier type_specifier */
#line 228 "stparserimpl.lemon"
{
    yylhsminor.yy35 = pParser->createNode<yal::frontend::STQualType>(yymsp[0].minor.yy72, yymsp[-1].minor.yy13);
}
#line 1558 "stparserimpl.c"
  yymsp[-1].minor.yy35 = yylhsminor.yy35;
        break;
      case 27: /* decl_type ::= TYPE identifier COLON type_specifier SEMI_COLON */
#line 234 "stparserimpl.lemon"
{
    yymsp[-4].minor.yy188 = nullptr;
}
#line 1566 "stparserimpl.c"
        break;
      case 28: /* decl_type ::= decl_struct */
#line 238 "stparserimpl.lemon"
{ yylhsminor.yy188 = yymsp[0].minor.yy143;}
#line 1571 "stparserimpl.c"
  yymsp[0].minor.yy188 = yylhsminor.yy188;
        break;
      case 29: /* decl_struct ::= TYPE identifier COLON STRUCT SCOPE_BEGIN struct_decl_vars SCOPE_END */
#line 243 "stparserimpl.lemon"
{
    yylhsminor.yy143 = pParser->createNode<yal::frontend::STDeclStruct>(yymsp[-5].minor.yy31, yymsp[-1].minor.yy34, *pParser);
    auto srcInfo = CreateSourceInfo(yymsp[-6].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy143->setSourceInfo(srcInfo);
}
#line 1581 "stparserimpl.c"
  yymsp[-6].minor.yy143 = yylhsminor.yy143;
        break;
      case 30: /* struct_decl_vars ::= struct_decl_vars COMMA struct_decl_var */
#line 249 "stparserimpl.lemon"
{
    auto& list = pParser->getStructMemberList();
    yylhsminor.yy34 = list.add(yymsp[-2].minor.yy34, yymsp[0].minor.yy25);
}
#line 1590 "stparserimpl.c"
  yymsp[-2].minor.yy34 = yylhsminor.yy34;
        break;
      case 31: /* struct_decl_vars ::= struct_decl_var */
#line 253 "stparserimpl.lemon"
{
    auto& list = pParser->getStructMemberList();
    yylhsminor.yy34 = list.add(yymsp[0].minor.yy25);
}
#line 1599 "stparserimpl.c"
  yymsp[0].minor.yy34 = yylhsminor.yy34;
        break;
      case 32: /* struct_decl_var ::= identifier COLON type_qualified */
#line 258 "stparserimpl.lemon"
{
    yylhsminor.yy25 = pParser->createNode<yal::frontend::STStructMember>(yymsp[-2].minor.yy31, yymsp[0].minor.yy35);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy31->getSourceInfo(), yymsp[0].minor.yy35->getSourceInfo(),pParser->getSourceHandle());
    yylhsminor.yy25->setSourceInfo(srcInfo);
}
#line 1609 "stparserimpl.c"
  yymsp[-2].minor.yy25 = yylhsminor.yy25;
        break;
      case 33: /* function_param_list ::= PAR_BEGIN function_args_decl PAR_END */
#line 267 "stparserimpl.lemon"
{
    yymsp[-2].minor.yy16 = yymsp[-1].minor.yy16;
}
#line 1617 "stparserimpl.c"
        break;
      case 34: /* decl_function ::= FUNCTION identifier function_param_list function_return_decl SCOPE_BEGIN statement_list_or_empty SCOPE_END */
#line 272 "stparserimpl.lemon"
{
    yylhsminor.yy20 = pParser->createNode<yal::frontend::STDeclFunction>(yymsp[-5].minor.yy31, yymsp[-3].minor.yy35, yymsp[-4].minor.yy16, yymsp[-1].minor.yy121, *pParser);
    yylhsminor.yy20->setSourceInfo(CreateSourceInfo(yymsp[-6].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle()));
}
#line 1625 "stparserimpl.c"
  yymsp[-6].minor.yy20 = yylhsminor.yy20;
        break;
      case 35: /* decl_type_functions_scope ::= IMPL type_specifier SCOPE_BEGIN decl_type_function_list SCOPE_END */
#line 279 "stparserimpl.lemon"
{
        yylhsminor.yy135 = pParser->createNode<yal::frontend::STDeclTypeFunctions>(yymsp[-3].minor.yy72,yymsp[-1].minor.yy122, *pParser);
        yylhsminor.yy135->setSourceInfo(CreateSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle()));
}
#line 1634 "stparserimpl.c"
  yymsp[-4].minor.yy135 = yylhsminor.yy135;
        break;
      case 36: /* decl_type_function_list ::= decl_type_function_list decl_function */
#line 284 "stparserimpl.lemon"
{
    auto& list = pParser->getDeclList();
    yylhsminor.yy122 = list.add(yymsp[-1].minor.yy122, yymsp[0].minor.yy20);
}
#line 1643 "stparserimpl.c"
  yymsp[-1].minor.yy122 = yylhsminor.yy122;
        break;
      case 37: /* decl_type_function_list ::= decl_function */
#line 289 "stparserimpl.lemon"
{
    auto& list = pParser->getDeclList();
    yylhsminor.yy122 = list.add(yymsp[0].minor.yy20);
}
#line 1652 "stparserimpl.c"
  yymsp[0].minor.yy122 = yylhsminor.yy122;
        break;
      case 38: /* function_args_decl ::= function_args_decl COMMA function_arg_decl */
#line 297 "stparserimpl.lemon"
{
        auto& list = pParser->getDeclParamList();
        yylhsminor.yy16 = list.add(yymsp[-2].minor.yy16, yymsp[0].minor.yy39);
}
#line 1661 "stparserimpl.c"
  yymsp[-2].minor.yy16 = yylhsminor.yy16;
        break;
      case 39: /* function_args_decl ::= function_arg_decl */
#line 302 "stparserimpl.lemon"
{
    auto& list = pParser->getDeclParamList();
    yylhsminor.yy16 = list.add(yymsp[0].minor.yy39);
}
#line 1670 "stparserimpl.c"
  yymsp[0].minor.yy16 = yylhsminor.yy16;
        break;
      case 40: /* function_args_decl ::= */
#line 307 "stparserimpl.lemon"
{
   yymsp[1].minor.yy16 = pParser->getDeclParamList().getRangeEmpty();
}
#line 1678 "stparserimpl.c"
        break;
      case 41: /* function_arg_decl ::= identifier COLON type_qualified */
#line 311 "stparserimpl.lemon"
{
        yylhsminor.yy39 = pParser->createNode<yal::frontend::STDeclParam>(yymsp[-2].minor.yy31, yymsp[0].minor.yy35);
        yylhsminor.yy39->setSourceInfo(yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy31->getSourceInfo(),yymsp[0].minor.yy35->getSourceInfo(), pParser->getSourceHandle()));
}
#line 1686 "stparserimpl.c"
  yymsp[-2].minor.yy39 = yylhsminor.yy39;
        break;
      case 42: /* function_arg_decl ::= qualifier self */
#line 316 "stparserimpl.lemon"
{
        auto qualType = pParser->createNode<yal::frontend::STQualType>(nullptr, yymsp[-1].minor.yy13);
        yylhsminor.yy39 = pParser->createNode<yal::frontend::STDeclParam>(yymsp[0].minor.yy31, qualType);
        yylhsminor.yy39->setSourceInfo(yymsp[0].minor.yy31->getSourceInfo());
}
#line 1696 "stparserimpl.c"
  yymsp[-1].minor.yy39 = yylhsminor.yy39;
        break;
      case 43: /* function_return_decl ::= COLON type_qualified */
#line 322 "stparserimpl.lemon"
{ yymsp[-1].minor.yy35 = yymsp[0].minor.yy35;}
#line 1702 "stparserimpl.c"
        break;
      case 44: /* function_return_decl ::= */
      case 58: /* var_type_spec ::= */ yytestcase(yyruleno==58);
#line 323 "stparserimpl.lemon"
{yymsp[1].minor.yy35 = nullptr;}
#line 1708 "stparserimpl.c"
        break;
      case 45: /* statement_list_or_empty ::= */
#line 327 "stparserimpl.lemon"
{yymsp[1].minor.yy121 = pParser->getStmtList().getRangeEmpty();}
#line 1713 "stparserimpl.c"
        break;
      case 46: /* statement_list_or_empty ::= statement_list */
#line 328 "stparserimpl.lemon"
{yylhsminor.yy121 = yymsp[0].minor.yy121;}
#line 1718 "stparserimpl.c"
  yymsp[0].minor.yy121 = yylhsminor.yy121;
        break;
      case 47: /* statement_list ::= statement_list statement */
#line 331 "stparserimpl.lemon"
{
    auto& list = pParser->getStmtList();
    yylhsminor.yy121 = list.add(yymsp[-1].minor.yy121,yymsp[0].minor.yy29);
}
#line 1727 "stparserimpl.c"
  yymsp[-1].minor.yy121 = yylhsminor.yy121;
        break;
      case 48: /* statement_list ::= statement */
#line 335 "stparserimpl.lemon"
{
    auto& list = pParser->getStmtList();
    yylhsminor.yy121 = list.add(yymsp[0].minor.yy29);
}
#line 1736 "stparserimpl.c"
  yymsp[0].minor.yy121 = yylhsminor.yy121;
        break;
      case 49: /* statement ::= expression ASSIGN expression SEMI_COLON */
#line 341 "stparserimpl.lemon"
{
   yylhsminor.yy29 = pParser->createNode<yal::frontend::STStmtAssign>(yymsp[-3].minor.yy58,yymsp[-1].minor.yy58);
   yylhsminor.yy29->setSourceInfo(yal::frontend::CreateSourceInfo(yymsp[-3].minor.yy58->getSourceInfo(), yymsp[-1].minor.yy58->getSourceInfo(), pParser->getSourceHandle()));
}
#line 1745 "stparserimpl.c"
  yymsp[-3].minor.yy29 = yylhsminor.yy29;
        break;
      case 50: /* statement ::= decl_var SEMI_COLON */
#line 345 "stparserimpl.lemon"
{
    yylhsminor.yy29 = pParser->createNode<yal::frontend::STStmtDecl>(yymsp[-1].minor.yy79);
    yylhsminor.yy29->setSourceInfo(yymsp[-1].minor.yy79->getSourceInfo());
}
#line 1754 "stparserimpl.c"
  yymsp[-1].minor.yy29 = yylhsminor.yy29;
        break;
      case 51: /* statement ::= expression SEMI_COLON */
#line 350 "stparserimpl.lemon"
{
    yylhsminor.yy29 = pParser->createNode<yal::frontend::STStmtExpression>(yymsp[-1].minor.yy58);
    yylhsminor.yy29->setSourceInfo(yymsp[-1].minor.yy58->getSourceInfo());
}
#line 1763 "stparserimpl.c"
  yymsp[-1].minor.yy29 = yylhsminor.yy29;
        break;
      case 52: /* statement ::= RETURN expression SEMI_COLON */
#line 355 "stparserimpl.lemon"
{
    yylhsminor.yy29 = pParser->createNode<yal::frontend::STStmtReturn>(yymsp[-1].minor.yy58);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy0, yymsp[-1].minor.yy58->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy29->setSourceInfo(srcInfo);
}
#line 1773 "stparserimpl.c"
  yymsp[-2].minor.yy29 = yylhsminor.yy29;
        break;
      case 53: /* statement ::= RETURN SEMI_COLON */
#line 361 "stparserimpl.lemon"
{
    yylhsminor.yy29 = pParser->createNode<yal::frontend::STStmtReturn>(nullptr);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[-1].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy29->setSourceInfo(srcInfo);
}
#line 1783 "stparserimpl.c"
  yymsp[-1].minor.yy29 = yylhsminor.yy29;
        break;
      case 54: /* statement ::= SCOPE_BEGIN statement_list_or_empty SCOPE_END */
#line 367 "stparserimpl.lemon"
{
    yylhsminor.yy29 = pParser->createNode<yal::frontend::STStmtListScoped>(yymsp[-1].minor.yy121, *pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy29->setSourceInfo(srcInfo);
}
#line 1793 "stparserimpl.c"
  yymsp[-2].minor.yy29 = yylhsminor.yy29;
        break;
      case 55: /* decl_var ::= VAR identifier var_type_spec ASSIGN expression */
#line 375 "stparserimpl.lemon"
{
    yylhsminor.yy79 = pParser->createNode<yal::frontend::STDeclVar>(yymsp[-3].minor.yy31, yymsp[-2].minor.yy35, yymsp[0].minor.yy58, false);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy58->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy79->setSourceInfo(srcInfo);
}
#line 1803 "stparserimpl.c"
  yymsp[-4].minor.yy79 = yylhsminor.yy79;
        break;
      case 56: /* decl_var ::= LET identifier var_type_spec ASSIGN expression */
#line 381 "stparserimpl.lemon"
{
    yylhsminor.yy79 = pParser->createNode<yal::frontend::STDeclVar>(yymsp[-3].minor.yy31, yymsp[-2].minor.yy35, yymsp[0].minor.yy58, true);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-4].minor.yy0, yymsp[0].minor.yy58->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy79->setSourceInfo(srcInfo);
}
#line 1813 "stparserimpl.c"
  yymsp[-4].minor.yy79 = yylhsminor.yy79;
        break;
      case 57: /* var_type_spec ::= COLON type_qualified */
#line 387 "stparserimpl.lemon"
{yymsp[-1].minor.yy35 = yymsp[0].minor.yy35;}
#line 1819 "stparserimpl.c"
        break;
      case 59: /* expression ::= PAR_BEGIN expression PAR_END */
#line 393 "stparserimpl.lemon"
{yymsp[-2].minor.yy58 = yymsp[-1].minor.yy58;}
#line 1824 "stparserimpl.c"
        break;
      case 60: /* expression ::= literal */
#line 394 "stparserimpl.lemon"
{yylhsminor.yy58 = yymsp[0].minor.yy58;}
#line 1829 "stparserimpl.c"
  yymsp[0].minor.yy58 = yylhsminor.yy58;
        break;
      case 61: /* expression ::= unaryexp */
#line 395 "stparserimpl.lemon"
{yylhsminor.yy58 = yymsp[0].minor.yy184;}
#line 1835 "stparserimpl.c"
  yymsp[0].minor.yy58 = yylhsminor.yy58;
        break;
      case 62: /* expression ::= binaryexp */
#line 396 "stparserimpl.lemon"
{yylhsminor.yy58 = yymsp[0].minor.yy114;}
#line 1841 "stparserimpl.c"
  yymsp[0].minor.yy58 = yylhsminor.yy58;
        break;
      case 63: /* expression ::= struct_init */
#line 397 "stparserimpl.lemon"
{ yylhsminor.yy58 = yymsp[0].minor.yy136;}
#line 1847 "stparserimpl.c"
  yymsp[0].minor.yy58 = yylhsminor.yy58;
        break;
      case 64: /* expression ::= expression AS type_qualified */
#line 399 "stparserimpl.lemon"
{
    yylhsminor.yy58 = pParser->createNode<yal::frontend::STExprCast>(yymsp[-2].minor.yy58, yymsp[0].minor.yy35);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy58->getSourceInfo(),
                                                   yymsp[0].minor.yy35->getSourceInfo(),
                                                   pParser->getSourceHandle());
    yylhsminor.yy58->setSourceInfo(srcInfo);
}
#line 1859 "stparserimpl.c"
  yymsp[-2].minor.yy58 = yylhsminor.yy58;
        break;
      case 65: /* expression ::= identifier_or_self */
#line 407 "stparserimpl.lemon"
{
    yylhsminor.yy58 = pParser->createNode<yal::frontend::STExprVarRef>(yymsp[0].minor.yy31);
    yylhsminor.yy58->setSourceInfo(yymsp[0].minor.yy31->getSourceInfo());
}
#line 1868 "stparserimpl.c"
  yymsp[0].minor.yy58 = yylhsminor.yy58;
        break;
      case 66: /* expression ::= expression DOT identifier */
#line 412 "stparserimpl.lemon"
{
    yylhsminor.yy58 = pParser->createNode<yal::frontend::STExprStructVarRef>(yymsp[-2].minor.yy58, yymsp[0].minor.yy31);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy58->getSourceInfo(), yymsp[0].minor.yy31->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy58->setSourceInfo(srcInfo);
}
#line 1878 "stparserimpl.c"
  yymsp[-2].minor.yy58 = yylhsminor.yy58;
        break;
      case 67: /* expression ::= identifier PAR_BEGIN function_call_args PAR_END */
#line 418 "stparserimpl.lemon"
{
    yylhsminor.yy58 = pParser->createNode<yal::frontend::STExprFnCall>(yymsp[-3].minor.yy31,yymsp[-1].minor.yy70,*pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-3].minor.yy31->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy58->setSourceInfo(srcInfo);
}
#line 1888 "stparserimpl.c"
  yymsp[-3].minor.yy58 = yylhsminor.yy58;
        break;
      case 68: /* expression ::= expression DOT identifier PAR_BEGIN function_call_args PAR_END */
#line 424 "stparserimpl.lemon"
{
    yylhsminor.yy58 = pParser->createNode<yal::frontend::STExprFnCall>(yymsp[-3].minor.yy31,yymsp[-5].minor.yy58,yymsp[-1].minor.yy70,*pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-5].minor.yy58->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy58->setSourceInfo(srcInfo);
}
#line 1898 "stparserimpl.c"
  yymsp[-5].minor.yy58 = yylhsminor.yy58;
        break;
      case 69: /* expression ::= type_specifier COLON COLON identifier PAR_BEGIN function_call_args PAR_END */
#line 430 "stparserimpl.lemon"
{
    yylhsminor.yy58 = pParser->createNode<yal::frontend::STExprFnCall>(yymsp[-3].minor.yy31,yymsp[-6].minor.yy72,yymsp[-1].minor.yy70,*pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-6].minor.yy72->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy58->setSourceInfo(srcInfo);
}
#line 1908 "stparserimpl.c"
  yymsp[-6].minor.yy58 = yylhsminor.yy58;
        break;
      case 70: /* binaryexp ::= expression AND expression */
#line 438 "stparserimpl.lemon"
{
        yylhsminor.yy114 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::And,
                                                        yymsp[-2].minor.yy58, yymsp[0].minor.yy58);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy58->getSourceInfo(), yymsp[0].minor.yy58->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy114->setSourceInfo(srcInfo);
}
#line 1919 "stparserimpl.c"
  yymsp[-2].minor.yy114 = yylhsminor.yy114;
        break;
      case 71: /* binaryexp ::= expression OR expression */
#line 444 "stparserimpl.lemon"
{
        yylhsminor.yy114 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Or,
                                                        yymsp[-2].minor.yy58, yymsp[0].minor.yy58);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy58->getSourceInfo(), yymsp[0].minor.yy58->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy114->setSourceInfo(srcInfo);
}
#line 1930 "stparserimpl.c"
  yymsp[-2].minor.yy114 = yylhsminor.yy114;
        break;
      case 72: /* binaryexp ::= expression PLUS expression */
#line 450 "stparserimpl.lemon"
{
        yylhsminor.yy114 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Plus,
                                                        yymsp[-2].minor.yy58, yymsp[0].minor.yy58);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy58->getSourceInfo(), yymsp[0].minor.yy58->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy114->setSourceInfo(srcInfo);
}
#line 1941 "stparserimpl.c"
  yymsp[-2].minor.yy114 = yylhsminor.yy114;
        break;
      case 73: /* binaryexp ::= expression MINUS expression */
#line 456 "stparserimpl.lemon"
{
        yylhsminor.yy114 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Minus,
                                                        yymsp[-2].minor.yy58, yymsp[0].minor.yy58);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy58->getSourceInfo(), yymsp[0].minor.yy58->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy114->setSourceInfo(srcInfo);
}
#line 1952 "stparserimpl.c"
  yymsp[-2].minor.yy114 = yylhsminor.yy114;
        break;
      case 74: /* binaryexp ::= expression MULT expression */
#line 462 "stparserimpl.lemon"
{
        yylhsminor.yy114 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Mult,
                                                        yymsp[-2].minor.yy58, yymsp[0].minor.yy58);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy58->getSourceInfo(), yymsp[0].minor.yy58->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy114->setSourceInfo(srcInfo);
}
#line 1963 "stparserimpl.c"
  yymsp[-2].minor.yy114 = yylhsminor.yy114;
        break;
      case 75: /* binaryexp ::= expression DIV expression */
#line 468 "stparserimpl.lemon"
{
        yylhsminor.yy114 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Div,
                                                        yymsp[-2].minor.yy58, yymsp[0].minor.yy58);
}
#line 1972 "stparserimpl.c"
  yymsp[-2].minor.yy114 = yylhsminor.yy114;
        break;
      case 76: /* binaryexp ::= expression MOD expression */
#line 472 "stparserimpl.lemon"
{
        yylhsminor.yy114 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Mod,
                                                        yymsp[-2].minor.yy58, yymsp[0].minor.yy58);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy58->getSourceInfo(), yymsp[0].minor.yy58->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy114->setSourceInfo(srcInfo);
}
#line 1983 "stparserimpl.c"
  yymsp[-2].minor.yy114 = yylhsminor.yy114;
        break;
      case 77: /* binaryexp ::= expression EQ expression */
#line 479 "stparserimpl.lemon"
{
        yylhsminor.yy114 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Eq,
                                                        yymsp[-2].minor.yy58, yymsp[0].minor.yy58);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy58->getSourceInfo(), yymsp[0].minor.yy58->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy114->setSourceInfo(srcInfo);
}
#line 1994 "stparserimpl.c"
  yymsp[-2].minor.yy114 = yylhsminor.yy114;
        break;
      case 78: /* binaryexp ::= expression NE expression */
#line 486 "stparserimpl.lemon"
{
        yylhsminor.yy114 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Ne,
                                                        yymsp[-2].minor.yy58, yymsp[0].minor.yy58);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy58->getSourceInfo(), yymsp[0].minor.yy58->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy114->setSourceInfo(srcInfo);
}
#line 2005 "stparserimpl.c"
  yymsp[-2].minor.yy114 = yylhsminor.yy114;
        break;
      case 79: /* binaryexp ::= expression LE expression */
#line 493 "stparserimpl.lemon"
{
        yylhsminor.yy114 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Le,
                                                        yymsp[-2].minor.yy58, yymsp[0].minor.yy58);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy58->getSourceInfo(), yymsp[0].minor.yy58->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy114->setSourceInfo(srcInfo);
}
#line 2016 "stparserimpl.c"
  yymsp[-2].minor.yy114 = yylhsminor.yy114;
        break;
      case 80: /* binaryexp ::= expression LT expression */
#line 500 "stparserimpl.lemon"
{
        yylhsminor.yy114 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Lt,
                                                        yymsp[-2].minor.yy58, yymsp[0].minor.yy58);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy58->getSourceInfo(), yymsp[0].minor.yy58->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy114->setSourceInfo(srcInfo);
}
#line 2027 "stparserimpl.c"
  yymsp[-2].minor.yy114 = yylhsminor.yy114;
        break;
      case 81: /* binaryexp ::= expression GE expression */
#line 507 "stparserimpl.lemon"
{
        yylhsminor.yy114 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Ge,
                                                        yymsp[-2].minor.yy58, yymsp[0].minor.yy58);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy58->getSourceInfo(), yymsp[0].minor.yy58->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy114->setSourceInfo(srcInfo);
}
#line 2038 "stparserimpl.c"
  yymsp[-2].minor.yy114 = yylhsminor.yy114;
        break;
      case 82: /* binaryexp ::= expression GT expression */
#line 514 "stparserimpl.lemon"
{
        yylhsminor.yy114 = pParser->createNode<yal::frontend::STExprBinaryOperator>(yal::frontend::BinaryOperatorType::Gt,
                                                        yymsp[-2].minor.yy58, yymsp[0].minor.yy58);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy58->getSourceInfo(), yymsp[0].minor.yy58->getSourceInfo(), pParser->getSourceHandle());
        yylhsminor.yy114->setSourceInfo(srcInfo);
}
#line 2049 "stparserimpl.c"
  yymsp[-2].minor.yy114 = yylhsminor.yy114;
        break;
      case 83: /* unaryexp ::= NOT expression */
#line 523 "stparserimpl.lemon"
{
    yylhsminor.yy184 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Not,
                                               yymsp[0].minor.yy58);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy58->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy184->setSourceInfo(srcInfo);
}
#line 2060 "stparserimpl.c"
  yymsp[-1].minor.yy184 = yylhsminor.yy184;
        break;
      case 84: /* unaryexp ::= BIT_NOT expression */
#line 529 "stparserimpl.lemon"
{
    yylhsminor.yy184 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::BitNot,
                                               yymsp[0].minor.yy58);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy58->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy184->setSourceInfo(srcInfo);
}
#line 2071 "stparserimpl.c"
  yymsp[-1].minor.yy184 = yylhsminor.yy184;
        break;
      case 85: /* unaryexp ::= MINUS expression */
#line 535 "stparserimpl.lemon"
{
    yylhsminor.yy184 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Negate,
                                               yymsp[0].minor.yy58);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy58->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy184->setSourceInfo(srcInfo);
}
#line 2082 "stparserimpl.c"
  yymsp[-1].minor.yy184 = yylhsminor.yy184;
        break;
      case 86: /* unaryexp ::= REFERENCE expression */
#line 542 "stparserimpl.lemon"
{
    yylhsminor.yy184 = pParser->createNode<yal::frontend::STExprUnaryOperator>(yal::frontend::UnaryOperatorType::Reference,
                                               yymsp[0].minor.yy58);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-1].minor.yy0, yymsp[0].minor.yy58->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy184->setSourceInfo(srcInfo);
}
#line 2093 "stparserimpl.c"
  yymsp[-1].minor.yy184 = yylhsminor.yy184;
        break;
      case 87: /* function_call_args ::= function_call_args COMMA expression */
#line 551 "stparserimpl.lemon"
{
    auto& list = pParser->getExprList();
    yylhsminor.yy70 = list.add(yymsp[-2].minor.yy70, yymsp[0].minor.yy58);
}
#line 2102 "stparserimpl.c"
  yymsp[-2].minor.yy70 = yylhsminor.yy70;
        break;
      case 88: /* function_call_args ::= expression */
#line 555 "stparserimpl.lemon"
{
    auto& list = pParser->getExprList();
    yylhsminor.yy70 = list.add(yymsp[0].minor.yy58);
}
#line 2111 "stparserimpl.c"
  yymsp[0].minor.yy70 = yylhsminor.yy70;
        break;
      case 89: /* function_call_args ::= */
#line 559 "stparserimpl.lemon"
{
    yymsp[1].minor.yy70= pParser->getExprList().getRangeEmpty();
}
#line 2119 "stparserimpl.c"
        break;
      case 90: /* struct_init ::= type_specifier SCOPE_BEGIN struct_member_init_list SCOPE_END */
#line 566 "stparserimpl.lemon"
{
    yylhsminor.yy136 = pParser->createNode<yal::frontend::STExprStructInit>(yymsp[-3].minor.yy72, yymsp[-1].minor.yy140, *pParser);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-3].minor.yy72->getSourceInfo(), yymsp[0].minor.yy0, pParser->getSourceHandle());
    yylhsminor.yy136->setSourceInfo(srcInfo);
}
#line 2128 "stparserimpl.c"
  yymsp[-3].minor.yy136 = yylhsminor.yy136;
        break;
      case 91: /* struct_member_init_list ::= */
#line 572 "stparserimpl.lemon"
{yymsp[1].minor.yy140 = pParser->getStructInitList().getRangeEmpty();}
#line 2134 "stparserimpl.c"
        break;
      case 92: /* struct_member_init_list ::= struct_member_init_list COMMA struct_member_init */
#line 573 "stparserimpl.lemon"
{
     auto& list = pParser->getStructInitList();
     yylhsminor.yy140 = list.add(yymsp[-2].minor.yy140, yymsp[0].minor.yy32);
}
#line 2142 "stparserimpl.c"
  yymsp[-2].minor.yy140 = yylhsminor.yy140;
        break;
      case 93: /* struct_member_init_list ::= struct_member_init */
#line 577 "stparserimpl.lemon"
{
    auto& list = pParser->getStructInitList();
    yylhsminor.yy140 = list.add(yymsp[0].minor.yy32);
}
#line 2151 "stparserimpl.c"
  yymsp[0].minor.yy140 = yylhsminor.yy140;
        break;
      case 94: /* struct_member_init ::= identifier COLON expression */
#line 582 "stparserimpl.lemon"
{
    yylhsminor.yy32 = pParser->createNode<yal::frontend::STStructMemberInit>(yymsp[-2].minor.yy31,yymsp[0].minor.yy58);
    auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[-2].minor.yy31->getSourceInfo(), yymsp[0].minor.yy58->getSourceInfo(), pParser->getSourceHandle());
    yylhsminor.yy32->setSourceInfo(srcInfo);
}
#line 2161 "stparserimpl.c"
  yymsp[-2].minor.yy32 = yylhsminor.yy32;
        break;
      case 95: /* literal ::= INTEGER_LITERAL */
      case 96: /* literal ::= INTEGER_LITERAL_I8 */ yytestcase(yyruleno==96);
      case 97: /* literal ::= INTEGER_LITERAL_I16 */ yytestcase(yyruleno==97);
      case 98: /* literal ::= INTEGER_LITERAL_I32 */ yytestcase(yyruleno==98);
      case 99: /* literal ::= INTEGER_LITERAL_I64 */ yytestcase(yyruleno==99);
      case 100: /* literal ::= INTEGER_LITERAL_U8 */ yytestcase(yyruleno==100);
      case 101: /* literal ::= INTEGER_LITERAL_U16 */ yytestcase(yyruleno==101);
      case 102: /* literal ::= INTEGER_LITERAL_U32 */ yytestcase(yyruleno==102);
      case 103: /* literal ::= INTEGER_LITERAL_U64 */ yytestcase(yyruleno==103);
#line 590 "stparserimpl.lemon"
{
        yylhsminor.yy58 = pParser->createNode<yal::frontend::STExprIntegerLiteral>(yymsp[0].minor.yy0);
        auto srcInfo =yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy58->setSourceInfo(srcInfo);
   }
#line 2179 "stparserimpl.c"
  yymsp[0].minor.yy58 = yylhsminor.yy58;
        break;
      case 104: /* literal ::= FLOAT_LITERAL */
      case 105: /* literal ::= FLOAT_LITERAL_32 */ yytestcase(yyruleno==105);
      case 106: /* literal ::= FLOAT_LITERAL_64 */ yytestcase(yyruleno==106);
#line 635 "stparserimpl.lemon"
{
        yylhsminor.yy58 = pParser->createNode<yal::frontend::STExprFloatLiteral>(yymsp[0].minor.yy0);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy58->setSourceInfo(srcInfo);
}
#line 2191 "stparserimpl.c"
  yymsp[0].minor.yy58 = yylhsminor.yy58;
        break;
      case 107: /* literal ::= BOOL_LITERAL */
#line 652 "stparserimpl.lemon"
{
        yylhsminor.yy58 = pParser->createNode<yal::frontend::STExprBoolLiteral>(yymsp[0].minor.yy0.tokenStr);
        auto srcInfo = yal::frontend::CreateSourceInfo(yymsp[0].minor.yy0, yymsp[0].minor.yy0, pParser->getSourceHandle());
        yylhsminor.yy58->setSourceInfo(srcInfo);
}
#line 2201 "stparserimpl.c"
  yymsp[0].minor.yy58 = yylhsminor.yy58;
        break;
      default:
      /* (108) module ::= decls END */ yytestcase(yyruleno==108);
      /* (109) type_array ::= type_builtin ARRAY_BEGIN ARRAY_END */ yytestcase(yyruleno==109);
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
#line 2285 "stparserimpl.c"
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
