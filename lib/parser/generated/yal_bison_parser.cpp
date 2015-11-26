/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 2 "parser.y" /* yacc.c:339  */

#include <stdlib.h>
#include <stdio.h>
#include <yal/yal.h>
#include <yal/parser/parser_state.h>
#include <yal/memory/memory_utils.h>
#include <yal/parser/bison_utils.h>
#include <yal/ast/asthdrs.h>
#include <yal/types/builtintype.h>
#include <yal/types/undefined.h>
#include <yal/types/type.h>
#include "yal_bison_parser.hpp"
#include "yal_flex_lexer.h"
using namespace yal;
extern void yyerror(YYLTYPE* location,
                     void* scanner,
                     yal::ParserState_t* state,
                     const char* s);
#define YYERROR_VERBOSE 1

#line 87 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "yal_bison_parser.hpp".  */
#ifndef YY_YY_HOME_LBEERNAERT_CODE_BUILD_YAL_QTCREATOR_LIB_PARSER_YAL_BISON_PARSER_HPP_INCLUDED
# define YY_YY_HOME_LBEERNAERT_CODE_BUILD_YAL_QTCREATOR_LIB_PARSER_YAL_BISON_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 24 "parser.y" /* yacc.c:355  */

#include <yal/yal.h>
#include <yal/parser/parser_state.h>
#include <yal/memory/memory_utils.h>
#include <yal/parser/bison_utils.h>
#include <yal/util/constantvalue.h>
#include <yal/ast/asthdrs.h>
#include <yal/types/typehdrs.h>


#line 128 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:355  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TK_INT32 = 258,
    TK_UINT32 = 259,
    TK_INT64 = 260,
    TK_UINT64 = 261,
    TK_FLT32 = 262,
    TK_FLT64 = 263,
    TK_BOOL = 264,
    TK_TEXT = 265,
    TK_ID = 266,
    TK_FUNC_BEGIN = 267,
    TK_RETURN = 268,
    TK_VAR_DECL = 269,
    TK_SPACE = 270,
    TK_TYPE_BOOL = 271,
    TK_TYPE_INT32 = 272,
    TK_TYPE_UINT32 = 273,
    TK_TYPE_INT64 = 274,
    TK_TYPE_UINT64 = 275,
    TK_TYPE_F32 = 276,
    TK_TYPE_F64 = 277,
    TK_TYPE_STRING = 278,
    TK_IF = 279,
    TK_ELIF = 280,
    TK_ELSE = 281,
    TK_END = 282,
    END = 283,
    TK_NL = 284,
    TK_PRINT = 285,
    TK_WHILE = 286,
    TK_NATIVE = 287,
    TK_ATTRIB_EXCALL = 288,
    TK_SELF = 289,
    TK_OP_ASSIGN = 290,
    TK_OP_ASSIGN_PLUS = 291,
    TK_OP_ASSIGN_MINUS = 292,
    TK_OP_ASSIGN_MULT = 293,
    TK_OP_ASSIGN_DIV = 294,
    TK_OP_ASSIGN_AND = 295,
    TK_OP_ASSIGN_OR = 296,
    TK_OP_ASSIGN_XOR = 297,
    TK_OP_ASSIGN_SHIFT_LEFT = 298,
    TK_OP_ASSIGN_SHIFT_RIGHT = 299,
    TK_NOT = 300,
    TK_BIT_TIL = 301,
    TK_OR = 302,
    TK_AND = 303,
    TK_BIT_OR = 304,
    TK_BIT_XOR = 305,
    TK_BIT_AND = 306,
    TK_CMP_EQ = 307,
    TK_CMP_NE = 308,
    TK_CMP_GT = 309,
    TK_CMP_GE = 310,
    TK_CMP_LE = 311,
    TK_CMP_LT = 312,
    TK_SHIFT_LEFT = 313,
    TK_SHIFT_RIGHT = 314,
    TK_OP_PLUS = 315,
    TK_OP_MINUS = 316,
    TK_OP_DIV = 317,
    TK_OP_MULT = 318,
    TK_MOD = 319,
    TK_DOT = 320,
    TK_ARRAY_BGN = 321,
    TK_ARRAY_END = 322,
    TK_PREC_NEG = 323,
    TK_PREC_NOT = 324
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 41 "parser.y" /* yacc.c:355  */

    yal_i32 integer32;
    yal_u32 uinteger32;
    yal_i64 integer64;
    yal_u64 uinteger64;
    yal_f32 float32;
    yal_f64 float64;
    yal_bool boolean;
    yal::Type* type;
    char* text;
    class yal::ConditionNode* nodeIf;
    class yal::ExpressionNode* nodeExp;
    class yal::StatementNode* nodeSt;
    class yal::CodeBodyNode* nodeCodeBody;
    class yal::AstBaseNode* node;
    class yal::ArgumentDeclNode* nodeDeclArg;
    class yal::ArgumentDeclsNode* nodeDeclArgs;
    class yal::PrintArgsNode* nodePrintArgs;
    class yal::FunctionCallArgsNode* nodeFunCallArgs;
    class yal::WhileLoopNode* nodeWhileLoop;
    class yal::ArrayCtrNode* nodeArrayCtr;

#line 233 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



int yyparse (void* scanner, yal::ParserState_t *state);

#endif /* !YY_YY_HOME_LBEERNAERT_CODE_BUILD_YAL_QTCREATOR_LIB_PARSER_YAL_BISON_PARSER_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 263 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   815

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  74
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  27
/* YYNRULES -- Number of rules.  */
#define YYNRULES  108
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  222

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   324

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      71,    72,     2,     2,    70,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    73,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   170,   170,   171,   172,   173,   177,   180,   181,   182,
     185,   187,   189,   191,   196,   197,   200,   201,   204,   205,
     208,   209,   210,   211,   212,   213,   214,   217,   220,   221,
     224,   226,   227,   228,   231,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   274,   275,   279,   280,   281,   285,   286,   287,
     290,   293,   294,   297,   298,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   313,   316,   317,   321,   322,   323,
     324,   325,   326,   327,   328,   331,   332,   333,   336
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "\"integer 32bit\"",
  "\"unsigned integer 32bit\"", "\"integer 64bit\"",
  "\"unsigned integer 64bit\"", "\"decimal 32bit\"", "\"decimal 64bit\"",
  "\"boolean\"", "\"string\"", "\"id\"", "\"func\"", "\"return\"",
  "\"var\"", "\" \"", "\"type boolean\"", "\"type integer 32bit\"",
  "\"type unsigned integer 32bit\"", "\"type integer 64bit\"",
  "\"type unsigned integer 64bit\"", "\"type decimal 32bit\"",
  "\"type decimal 64bit\"", "\"type string\"", "\"if\"", "\"elif\"",
  "\"else\"", "\"end\"", "END", "\"new line\"", "\"print\"", "\"while\"",
  "\"native\"", "\"attribute excall\"", "\"self\"", "\"=\"", "\"+=\"",
  "\"-=\"", "\"*=\"", "\"/=\"", "\"&=\"", "\"|=\"", "\"^=\"", "\"<<=\"",
  "\">>=\"", "\"not\"", "\"~\"", "\"or\"", "\"and\"", "\"|\"", "\"^\"",
  "\"&\"", "\"==\"", "\"!=\"", "\">\"", "\">=\"", "\"<=\"", "\"<\"",
  "\"<<\"", "\">>\"", "\"+\"", "\"-\"", "\"/\"", "\"*\"", "\"mod\"",
  "\".\"", "\"[\"", "\"]\"", "TK_PREC_NEG", "TK_PREC_NOT", "','", "'('",
  "')'", "':'", "$accept", "program", "attribute", "attributes",
  "func_decl", "func_return", "code_body", "st_upper", "statement",
  "print_statement", "print_args", "if_statement", "if_statement_next",
  "while_statement", "expression", "var_access", "call_args", "decl_args",
  "decl_arg", "var_decl", "var_decl_exp", "basic_type", "compound_type",
  "type", "constant", "array_ctr_exp", "array_ctr", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
      44,    40,    41,    58
};
# endif

#define YYPACT_NINF -176

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-176)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -176,    72,  -176,  -176,  -176,  -176,  -176,  -176,  -176,  -176,
    -176,   -45,   429,    -4,   -44,  -176,   -40,    24,    86,  -176,
    -176,   429,   429,   429,   429,    60,    87,    -9,    90,  -176,
    -176,  -176,   651,  -176,  -176,  -176,   429,   651,    81,   133,
     429,   429,   429,   181,   -48,   -48,   -48,   466,  -176,    91,
     226,  -176,  -176,   429,   429,   429,   429,   429,   429,   429,
     429,   429,   429,   429,   429,   429,   429,   429,   429,   429,
     429,   429,   429,   429,   429,   429,   429,   429,   429,   429,
     429,   109,   429,   651,   -67,   416,  -176,  -176,  -176,  -176,
    -176,  -176,  -176,  -176,  -176,  -176,  -176,     5,   504,   -47,
     651,   542,    56,   -52,  -176,  -176,    65,   -51,   651,   651,
     651,   651,   651,   651,   651,   651,   651,   651,   670,   688,
     705,   721,   736,   749,   749,    49,    49,    49,    49,   115,
     115,   -26,   -26,   -48,   -48,   -48,    68,   618,   429,  -176,
     429,   651,  -176,  -176,   110,    79,   119,   429,  -176,   128,
     133,    88,   133,    89,   429,  -176,   651,   651,   -66,   416,
    -176,   384,   651,   384,   -38,  -176,     8,   148,    18,   161,
      21,  -176,   429,  -176,  -176,   160,  -176,   153,   205,   133,
     120,  -176,   112,   120,   124,  -176,   651,   125,   178,  -176,
    -176,  -176,  -176,  -176,  -176,   133,  -176,   133,   188,   133,
     429,   384,   154,    22,   384,    51,   580,   250,   120,   295,
     120,   193,  -176,  -176,  -176,   194,   384,   384,   160,   340,
    -176,  -176
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       5,     9,     1,    98,    99,   100,   101,   102,   103,    97,
     104,    72,    26,     0,     0,     4,     0,     0,     0,     6,
      73,     0,     0,     0,     0,     0,     0,     0,     0,    21,
      23,    24,    22,    69,    20,    68,    76,    25,     0,     0,
       0,     0,     0,     0,    38,    39,    37,     0,     7,     9,
       0,     2,     3,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,     0,     0,    92,    85,    86,    87,
      88,    89,    90,    91,    93,    95,    96,     0,     0,     0,
      29,     0,    92,     0,    36,     8,    92,     0,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    41,    40,
      53,    54,    55,    46,    47,    42,    43,    44,    45,    56,
      57,    48,    49,    51,    50,    52,     0,     0,     0,    70,
     107,    83,    81,    84,     0,     0,     0,     0,    27,     0,
      79,     0,    79,     0,    76,    35,    74,   106,     0,     0,
      94,     0,    28,     0,     0,    78,     0,     0,     0,     0,
       0,   108,     0,    82,    19,     0,    16,     0,     0,     0,
      14,    80,     0,    14,     0,    71,   105,     0,     0,    33,
      17,    30,    18,    34,    77,     0,    10,    79,     0,    79,
       0,     0,    15,     0,     0,     0,     0,     0,    14,     0,
      14,     0,    32,    11,    12,     0,     0,     0,     0,     0,
      31,    13
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -176,  -176,  -176,   175,  -176,  -175,   -79,   -78,   224,  -176,
    -176,  -176,     9,  -176,   -12,  -176,    74,   -65,    47,  -176,
      71,  -176,  -176,   -37,  -176,  -176,  -176
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    25,    26,    27,   196,   175,   176,   177,    29,
      99,    30,   191,    31,    32,    33,    84,   164,   165,    34,
     142,    95,    96,   166,    35,   158,   143
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      37,   171,    97,   138,   172,   139,   103,    38,   198,    44,
      45,    46,    47,   107,   145,   145,   144,    81,    82,   181,
      51,   151,   153,   147,    83,   148,    36,    40,    98,   100,
     101,    41,   179,   213,   180,   215,    78,    79,    80,    81,
      82,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,    39,
     137,   145,     2,   141,   145,     3,     4,     5,     6,     7,
       8,     9,    10,    11,   178,    12,    13,   168,   179,    48,
     183,   138,   179,   185,   208,    42,    14,   190,    43,    50,
     190,    15,    16,    17,    18,    19,    20,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    85,    21,    22,    52,
     136,   179,   207,   210,    19,   209,   156,   150,   157,   190,
      49,   190,   203,    23,   205,   162,   152,   218,   219,   154,
     190,   190,    83,    24,    86,   159,   160,   141,   161,    87,
      88,    89,    90,    91,    92,    93,    94,   163,   202,   182,
     186,   167,   169,     3,     4,     5,     6,     7,     8,     9,
      10,    11,   184,    12,    13,    76,    77,    78,    79,    80,
      81,    82,   192,   197,    14,   187,   188,   189,   206,   174,
      16,    17,   102,   195,    20,   199,   200,    87,    88,    89,
      90,    91,    92,    93,    94,    21,    22,   201,     3,     4,
       5,     6,     7,     8,     9,    10,    11,   204,    12,    13,
     145,    23,   216,   217,   105,    28,   194,   220,   170,    14,
     173,    24,   193,     0,   174,    16,    17,   106,     0,    20,
       0,     0,    87,    88,    89,    90,    91,    92,    93,    94,
      21,    22,     0,     3,     4,     5,     6,     7,     8,     9,
      10,    11,     0,    12,    13,     0,    23,     0,     0,     0,
       0,     0,     0,     0,    14,     0,    24,   212,     0,   174,
      16,    17,     0,     0,    20,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    21,    22,     0,     3,     4,
       5,     6,     7,     8,     9,    10,    11,     0,    12,    13,
       0,    23,     0,     0,     0,     0,     0,     0,     0,    14,
       0,    24,   214,     0,   174,    16,    17,     0,     0,    20,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,    22,     0,     3,     4,     5,     6,     7,     8,     9,
      10,    11,     0,    12,    13,     0,    23,     0,     0,     0,
       0,     0,     0,     0,    14,     0,    24,   221,     0,   174,
      16,    17,     0,     0,    20,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    21,    22,     3,     4,     5,
       6,     7,     8,     9,    10,    11,     0,    12,    13,     0,
       0,    23,     0,     0,     0,     0,     0,     0,    14,     0,
       0,    24,     0,   174,    16,    17,     0,     0,    20,     3,
       4,     5,     6,     7,     8,     9,    10,    11,     0,    21,
      22,     0,     3,     4,     5,     6,     7,     8,     9,    10,
      11,     0,     0,     0,     0,    23,     0,     0,     0,     0,
      20,     0,     0,     0,     0,    24,     0,     0,     0,     0,
       0,    21,    22,    20,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,    22,     0,    23,     0,     0,
       0,     0,   140,     0,     0,     0,     0,    24,     0,     0,
      23,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      24,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,     0,     0,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,     0,     0,     0,     0,     0,   104,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,     0,
       0,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,     0,     0,     0,     0,     0,   146,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,     0,     0,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,     0,
       0,     0,     0,     0,   149,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,     0,     0,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,     0,     0,     0,
       0,     0,   211,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,     0,     0,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,   155,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,     0,     0,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82
};

static const yytype_int16 yycheck[] =
{
      12,    67,    39,    70,    70,    72,    43,    11,   183,    21,
      22,    23,    24,    50,    66,    66,    11,    65,    66,    11,
      29,    73,    73,    70,    36,    72,    71,    71,    40,    41,
      42,    71,    70,   208,    72,   210,    62,    63,    64,    65,
      66,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    73,
      82,    66,     0,    85,    66,     3,     4,     5,     6,     7,
       8,     9,    10,    11,   163,    13,    14,   152,    70,    29,
      72,    70,    70,    72,    72,    71,    24,   175,    12,    12,
     178,    29,    30,    31,    32,    33,    34,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    35,    45,    46,    29,
      11,    70,   201,    72,    33,   204,   138,    71,   140,   207,
      70,   209,   197,    61,   199,   147,    71,   216,   217,    71,
     218,   219,   154,    71,    11,    35,    67,   159,    29,    16,
      17,    18,    19,    20,    21,    22,    23,    29,   195,    11,
     172,    73,    73,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    11,    13,    14,    60,    61,    62,    63,    64,
      65,    66,    29,    71,    24,    25,    26,    27,   200,    29,
      30,    31,    11,    73,    34,    71,    71,    16,    17,    18,
      19,    20,    21,    22,    23,    45,    46,    29,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    29,    13,    14,
      66,    61,    29,    29,    49,     1,   179,   218,   154,    24,
     159,    71,    27,    -1,    29,    30,    31,    11,    -1,    34,
      -1,    -1,    16,    17,    18,    19,    20,    21,    22,    23,
      45,    46,    -1,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    -1,    13,    14,    -1,    61,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    71,    27,    -1,    29,
      30,    31,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    45,    46,    -1,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    -1,    13,    14,
      -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      -1,    71,    27,    -1,    29,    30,    31,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      45,    46,    -1,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    -1,    13,    14,    -1,    61,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    71,    27,    -1,    29,
      30,    31,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    45,    46,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    -1,    13,    14,    -1,
      -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    71,    -1,    29,    30,    31,    -1,    -1,    34,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    -1,    45,
      46,    -1,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,    -1,
      34,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      -1,    45,    46,    34,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    45,    46,    -1,    61,    -1,    -1,
      -1,    -1,    66,    -1,    -1,    -1,    -1,    71,    -1,    -1,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    -1,    -1,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    -1,    -1,    -1,    -1,    -1,    72,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    -1,
      -1,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    -1,    -1,    -1,    -1,    -1,    72,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    -1,
      -1,    -1,    -1,    -1,    72,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    -1,    -1,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    -1,    -1,    -1,
      -1,    -1,    72,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    -1,    -1,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    -1,    -1,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    75,     0,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    13,    14,    24,    29,    30,    31,    32,    33,
      34,    45,    46,    61,    71,    76,    77,    78,    82,    83,
      85,    87,    88,    89,    93,    98,    71,    88,    11,    73,
      71,    71,    71,    12,    88,    88,    88,    88,    29,    70,
      12,    29,    29,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    88,    90,    35,    11,    16,    17,    18,
      19,    20,    21,    22,    23,    95,    96,    97,    88,    84,
      88,    88,    11,    97,    72,    77,    11,    97,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    11,    88,    70,    72,
      66,    88,    94,   100,    11,    66,    72,    70,    72,    72,
      71,    73,    71,    73,    71,    67,    88,    88,    99,    35,
      67,    29,    88,    29,    91,    92,    97,    73,    91,    73,
      90,    67,    70,    94,    29,    80,    81,    82,    80,    70,
      72,    11,    11,    72,    11,    72,    88,    25,    26,    27,
      81,    86,    29,    27,    92,    73,    79,    71,    79,    71,
      71,    29,    97,    91,    29,    91,    88,    80,    72,    80,
      72,    72,    27,    79,    27,    79,    29,    29,    80,    80,
      86,    27
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    74,    75,    75,    75,    75,    76,    77,    77,    77,
      78,    78,    78,    78,    79,    79,    80,    80,    81,    81,
      82,    82,    82,    82,    82,    82,    82,    83,    84,    84,
      85,    86,    86,    86,    87,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    89,    89,    90,    90,    90,    91,    91,    91,
      92,    93,    93,    94,    94,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    96,    97,    97,    98,    98,    98,
      98,    98,    98,    98,    98,    99,    99,    99,   100
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     3,     2,     0,     1,     2,     3,     0,
       7,    10,    10,    13,     0,     2,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     2,     1,     4,     3,     1,
       7,     7,     4,     1,     7,     4,     3,     2,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     1,
       4,     6,     1,     1,     3,     1,     0,     3,     1,     0,
       2,     4,     6,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     0,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (&yylloc, scanner, state, YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location, scanner, state); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, void* scanner, yal::ParserState_t *state)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  YYUSE (scanner);
  YYUSE (state);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, void* scanner, yal::ParserState_t *state)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, scanner, state);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, void* scanner, yal::ParserState_t *state)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , scanner, state);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, scanner, state); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, void* scanner, yal::ParserState_t *state)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (scanner);
  YYUSE (state);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void* scanner, yal::ParserState_t *state)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
                    &yystacksize);

        yyls = yyls1;
        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex (&yylval, &yylloc, scanner);
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 170 "parser.y" /* yacc.c:1646  */
    {state->program.push_back(static_cast<yal::StatementNode*>(yal::BisonYyltypeToLocation(yylloc),(yyvsp[-1].node)));}
#line 1728 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 171 "parser.y" /* yacc.c:1646  */
    {state->program.push_back((yyvsp[-1].nodeSt));}
#line 1734 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 186 "parser.y" /* yacc.c:1646  */
    { (yyval.node) = new yal::FunctionDeclNativeNode(yal::BisonYyltypeToLocation(yylloc), (yyvsp[-4].text), nullptr, (yyvsp[-2].nodeDeclArgs), (yyvsp[0].type)); }
#line 1740 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 188 "parser.y" /* yacc.c:1646  */
    { (yyval.node) = new yal::FunctionDeclNativeNode(yal::BisonYyltypeToLocation(yylloc), (yyvsp[-4].text), (yyvsp[-7].type), (yyvsp[-2].nodeDeclArgs), (yyvsp[0].type)); }
#line 1746 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 190 "parser.y" /* yacc.c:1646  */
    { (yyval.node) = new yal::FunctionDeclNode(yal::BisonYyltypeToLocation(yylloc), (yyvsp[-7].text), nullptr, (yyvsp[-5].nodeDeclArgs), (yyvsp[-3].type), (yyvsp[-1].nodeCodeBody)); }
#line 1752 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 192 "parser.y" /* yacc.c:1646  */
    { (yyval.node) = new yal::FunctionDeclNode(yal::BisonYyltypeToLocation(yylloc), (yyvsp[-7].text), (yyvsp[-10].type), (yyvsp[-5].nodeDeclArgs), (yyvsp[-3].type), (yyvsp[-1].nodeCodeBody)); }
#line 1758 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 196 "parser.y" /* yacc.c:1646  */
    { (yyval.type) = yal::BuiltinType::GetBuiltinType(yal::BuiltinType::kVoid); }
#line 1764 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 197 "parser.y" /* yacc.c:1646  */
    { (yyval.type) = (yyvsp[0].type); }
#line 1770 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 200 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeCodeBody) = new CodeBodyNode(yal::BisonYyltypeToLocation(yylloc)); if((yyvsp[0].nodeSt)) {(yyval.nodeCodeBody)->addStatement((yyvsp[0].nodeSt));}}
#line 1776 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 201 "parser.y" /* yacc.c:1646  */
    {if((yyvsp[0].nodeSt)) {(yyval.nodeCodeBody)->addStatement((yyvsp[0].nodeSt));}}
#line 1782 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 204 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeSt) = (yyvsp[-1].nodeSt);}
#line 1788 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 205 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeSt) = nullptr;}
#line 1794 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 208 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeSt) = (yyvsp[0].nodeSt);}
#line 1800 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 209 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeSt) = static_cast<yal::StatementNode*>((yyvsp[0].nodeSt));}
#line 1806 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 210 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeSt) = static_cast<yal::StatementNode*>((yyvsp[0].nodeExp));}
#line 1812 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 211 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeSt) = static_cast<yal::StatementNode*>((yyvsp[0].nodeIf));}
#line 1818 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 212 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeSt) = static_cast<yal::StatementNode*>((yyvsp[0].nodeWhileLoop)); }
#line 1824 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 213 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeSt) = new yal::ReturnNode(yal::BisonYyltypeToLocation(yylloc), (yyvsp[0].nodeExp));}
#line 1830 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 214 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeSt) = new yal::ReturnNode(yal::BisonYyltypeToLocation(yylloc), nullptr);}
#line 1836 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 217 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeSt) = new yal::PrintNode(yal::BisonYyltypeToLocation(yylloc), (yyvsp[-1].nodePrintArgs));}
#line 1842 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 220 "parser.y" /* yacc.c:1646  */
    {(yyval.nodePrintArgs)->expressions.push_back((yyvsp[0].nodeExp));}
#line 1848 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 221 "parser.y" /* yacc.c:1646  */
    { (yyval.nodePrintArgs) = new yal::PrintArgsNode(yal::BisonYyltypeToLocation(yylloc)); (yyval.nodePrintArgs)->expressions.push_back((yyvsp[0].nodeExp));}
#line 1854 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 224 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeIf) = new yal::ConditionNode(yal::BisonYyltypeToLocation(yylloc), (yyvsp[-4].nodeExp), (yyvsp[-1].nodeCodeBody), (yyvsp[0].nodeIf));}
#line 1860 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 226 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeIf) = new ConditionNode(yal::BisonYyltypeToLocation(yylloc), (yyvsp[-4].nodeExp), (yyvsp[-1].nodeCodeBody), (yyvsp[0].nodeIf));}
#line 1866 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 227 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeIf) = new ConditionNode(yal::BisonYyltypeToLocation(yylloc), nullptr, (yyvsp[-1].nodeCodeBody), nullptr);}
#line 1872 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 228 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeIf) = nullptr;}
#line 1878 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 231 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeWhileLoop) = new yal::WhileLoopNode(yal::BisonYyltypeToLocation(yylloc), (yyvsp[-4].nodeExp), (yyvsp[-1].nodeCodeBody));}
#line 1884 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 235 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeExp) = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeArray, (yyvsp[-3].nodeExp), (yyvsp[-1].nodeExp));}
#line 1890 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 236 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeExp) =  new yal::ParentExpNode(yal::BisonYyltypeToLocation(yylloc), (yyvsp[-1].nodeExp));}
#line 1896 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 237 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeExp) = new yal::SingleOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeMinus, (yyvsp[0].nodeExp));}
#line 1902 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 238 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeExp) = new yal::SingleOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeNot, (yyvsp[0].nodeExp));}
#line 1908 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 239 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeExp) = new yal::SingleOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeBitNot, (yyvsp[0].nodeExp));}
#line 1914 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 240 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeExp) = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeAnd, (yyvsp[-2].nodeExp), (yyvsp[0].nodeExp));}
#line 1920 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 241 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeExp) = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeOr, (yyvsp[-2].nodeExp), (yyvsp[0].nodeExp));}
#line 1926 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 242 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeExp) = new yal::CompareOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeGt, (yyvsp[-2].nodeExp), (yyvsp[0].nodeExp));}
#line 1932 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 243 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeExp) = new yal::CompareOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeGe, (yyvsp[-2].nodeExp), (yyvsp[0].nodeExp));}
#line 1938 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 244 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeExp) = new yal::CompareOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeLe, (yyvsp[-2].nodeExp), (yyvsp[0].nodeExp));}
#line 1944 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 245 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeExp) = new yal::CompareOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeLt, (yyvsp[-2].nodeExp), (yyvsp[0].nodeExp));}
#line 1950 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 246 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeExp) = new yal::CompareOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeEq, (yyvsp[-2].nodeExp), (yyvsp[0].nodeExp));}
#line 1956 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 247 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeExp) = new yal::CompareOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeNe, (yyvsp[-2].nodeExp), (yyvsp[0].nodeExp));}
#line 1962 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 248 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeExp) = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypePlus, (yyvsp[-2].nodeExp), (yyvsp[0].nodeExp));}
#line 1968 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 249 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeExp) = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeMinus, (yyvsp[-2].nodeExp), (yyvsp[0].nodeExp));}
#line 1974 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 250 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeExp) = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeMult, (yyvsp[-2].nodeExp), (yyvsp[0].nodeExp));}
#line 1980 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 251 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeExp) = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeDiv, (yyvsp[-2].nodeExp), (yyvsp[0].nodeExp));}
#line 1986 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 252 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeExp) = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeMod, (yyvsp[-2].nodeExp), (yyvsp[0].nodeExp));}
#line 1992 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 253 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeExp) = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeBitOr, (yyvsp[-2].nodeExp), (yyvsp[0].nodeExp));}
#line 1998 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 254 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeExp) = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeBitXor, (yyvsp[-2].nodeExp), (yyvsp[0].nodeExp));}
#line 2004 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 255 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeExp) = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeBitAnd, (yyvsp[-2].nodeExp), (yyvsp[0].nodeExp));}
#line 2010 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 256 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeExp) = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeBitShiftLeft, (yyvsp[-2].nodeExp), (yyvsp[0].nodeExp));}
#line 2016 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 257 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeExp) = new yal::DualOperatorNode(yal::BisonYyltypeToLocation(yylloc), kOperatorTypeBitShiftRight, (yyvsp[-2].nodeExp), (yyvsp[0].nodeExp));}
#line 2022 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 258 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeExp) = new yal::AssignOperatorNode(yal::BisonYyltypeToLocation(yylloc), (yyvsp[-2].nodeExp), kOperatorTypeCopy, (yyvsp[0].nodeExp));}
#line 2028 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 259 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeExp) = new yal::AssignOperatorNode(yal::BisonYyltypeToLocation(yylloc), (yyvsp[-2].nodeExp), kOperatorTypePlus, (yyvsp[0].nodeExp));}
#line 2034 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 260 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeExp) = new yal::AssignOperatorNode(yal::BisonYyltypeToLocation(yylloc), (yyvsp[-2].nodeExp), kOperatorTypeMinus, (yyvsp[0].nodeExp));}
#line 2040 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 261 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeExp) = new yal::AssignOperatorNode(yal::BisonYyltypeToLocation(yylloc), (yyvsp[-2].nodeExp), kOperatorTypeMult, (yyvsp[0].nodeExp));}
#line 2046 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 262 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeExp) = new yal::AssignOperatorNode(yal::BisonYyltypeToLocation(yylloc), (yyvsp[-2].nodeExp), kOperatorTypeDiv, (yyvsp[0].nodeExp));}
#line 2052 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 263 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeExp) = new yal::AssignOperatorNode(yal::BisonYyltypeToLocation(yylloc), (yyvsp[-2].nodeExp), kOperatorTypeBitAnd, (yyvsp[0].nodeExp));}
#line 2058 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 264 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeExp) = new yal::AssignOperatorNode(yal::BisonYyltypeToLocation(yylloc), (yyvsp[-2].nodeExp), kOperatorTypeBitOr, (yyvsp[0].nodeExp));}
#line 2064 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 265 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeExp) = new yal::AssignOperatorNode(yal::BisonYyltypeToLocation(yylloc), (yyvsp[-2].nodeExp), kOperatorTypeBitXor, (yyvsp[0].nodeExp));}
#line 2070 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 266 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeExp) = new yal::AssignOperatorNode(yal::BisonYyltypeToLocation(yylloc), (yyvsp[-2].nodeExp), kOperatorTypeBitShiftLeft, (yyvsp[0].nodeExp));}
#line 2076 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 267 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeExp) = new yal::AssignOperatorNode(yal::BisonYyltypeToLocation(yylloc), (yyvsp[-2].nodeExp), kOperatorTypeBitShiftRight, (yyvsp[0].nodeExp));}
#line 2082 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 268 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeExp) = (yyvsp[0].nodeExp);}
#line 2088 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 269 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeExp) = (yyvsp[0].nodeExp);}
#line 2094 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 270 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeExp) = new yal::FunctionCallNode(yal::BisonYyltypeToLocation(yylloc), nullptr, (yyvsp[-3].text), (yyvsp[-1].nodeFunCallArgs));}
#line 2100 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 271 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeExp) = new yal::FunctionCallNode(yal::BisonYyltypeToLocation(yylloc), (yyvsp[-5].nodeExp), (yyvsp[-3].text), (yyvsp[-1].nodeFunCallArgs));}
#line 2106 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 274 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeExp) = new yal::VariableAccessNode(yal::BisonYyltypeToLocation(yylloc), (yyvsp[0].text)); }
#line 2112 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 275 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeExp) = new yal::VariableAccessNode(yal::BisonYyltypeToLocation(yylloc), "self");}
#line 2118 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 279 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeFunCallArgs)->expressions.push_back((yyvsp[0].nodeExp));}
#line 2124 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 280 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeFunCallArgs) = new yal::FunctionCallArgsNode(yal::BisonYyltypeToLocation(yylloc)); (yyval.nodeFunCallArgs)->expressions.push_back((yyvsp[0].nodeExp));}
#line 2130 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 281 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeFunCallArgs) = new yal::FunctionCallArgsNode(yal::BisonYyltypeToLocation(yylloc)); }
#line 2136 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 285 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeDeclArgs)->addArgument((yyvsp[0].nodeDeclArg));}
#line 2142 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 286 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeDeclArgs) = new yal::ArgumentDeclsNode(yal::BisonYyltypeToLocation(yylloc)); (yyval.nodeDeclArgs)->addArgument((yyvsp[0].nodeDeclArg));}
#line 2148 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 287 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeDeclArgs) = new yal::ArgumentDeclsNode(yal::BisonYyltypeToLocation(yylloc));}
#line 2154 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 290 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeDeclArg) = new yal::ArgumentDeclNode(yal::BisonYyltypeToLocation(yylloc), (yyvsp[-1].type), (yyvsp[0].text));}
#line 2160 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 293 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeSt) = new yal::VariableDeclNode(yal::BisonYyltypeToLocation(yylloc), (yyvsp[-2].text), (yyvsp[0].nodeExp), nullptr);}
#line 2166 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 294 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeSt) = new yal::VariableDeclNode(yal::BisonYyltypeToLocation(yylloc), (yyvsp[-2].text), (yyvsp[0].nodeExp), (yyvsp[-3].type));}
#line 2172 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 297 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeExp) = (yyvsp[0].nodeExp);}
#line 2178 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 298 "parser.y" /* yacc.c:1646  */
    {(yyval.nodeExp) = (yyvsp[0].nodeExp);}
#line 2184 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 302 "parser.y" /* yacc.c:1646  */
    {(yyval.type) = yal::BuiltinType::GetBuiltinType(yal::BuiltinType::kBool);}
#line 2190 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 303 "parser.y" /* yacc.c:1646  */
    {(yyval.type) = yal::BuiltinType::GetBuiltinType(yal::BuiltinType::kInt32);}
#line 2196 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 304 "parser.y" /* yacc.c:1646  */
    {(yyval.type) = yal::BuiltinType::GetBuiltinType(yal::BuiltinType::kUInt32);}
#line 2202 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 305 "parser.y" /* yacc.c:1646  */
    {(yyval.type) = yal::BuiltinType::GetBuiltinType(yal::BuiltinType::kInt64);}
#line 2208 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 306 "parser.y" /* yacc.c:1646  */
    {(yyval.type) = yal::BuiltinType::GetBuiltinType(yal::BuiltinType::kUInt64);}
#line 2214 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 307 "parser.y" /* yacc.c:1646  */
    {(yyval.type) = yal::BuiltinType::GetBuiltinType(yal::BuiltinType::kFloat32);}
#line 2220 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 308 "parser.y" /* yacc.c:1646  */
    {(yyval.type) = yal::BuiltinType::GetBuiltinType(yal::BuiltinType::kFloat64);}
#line 2226 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 309 "parser.y" /* yacc.c:1646  */
    { (yyval.type) = state->registry.registerUndefined((yyvsp[0].text)); yal_free((yyvsp[0].text));}
#line 2232 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 310 "parser.y" /* yacc.c:1646  */
    { (yyval.type) = yal::StringType::GetType(); }
#line 2238 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 313 "parser.y" /* yacc.c:1646  */
    { (yyval.type) = state->registry.registerArray((yyvsp[-2].type)); }
#line 2244 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 316 "parser.y" /* yacc.c:1646  */
    {(yyval.type) = (yyvsp[0].type);}
#line 2250 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 317 "parser.y" /* yacc.c:1646  */
    {(yyval.type) = (yyvsp[0].type);}
#line 2256 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 97:
#line 321 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeExp) = new yal::ConstantNode(yal::BisonYyltypeToLocation(yylloc), yal::ConstantValue((yyvsp[0].boolean)));}
#line 2262 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 322 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeExp) = new yal::ConstantNode(yal::BisonYyltypeToLocation(yylloc), yal::ConstantValue((yyvsp[0].integer32)));}
#line 2268 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 99:
#line 323 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeExp) = new yal::ConstantNode(yal::BisonYyltypeToLocation(yylloc),yal::ConstantValue((yyvsp[0].uinteger32)));}
#line 2274 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 100:
#line 324 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeExp) = new yal::ConstantNode(yal::BisonYyltypeToLocation(yylloc), yal::ConstantValue((yyvsp[0].integer64)));}
#line 2280 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 101:
#line 325 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeExp) = new yal::ConstantNode(yal::BisonYyltypeToLocation(yylloc), yal::ConstantValue((yyvsp[0].uinteger64)));}
#line 2286 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 102:
#line 326 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeExp) = new yal::ConstantNode(yal::BisonYyltypeToLocation(yylloc), yal::ConstantValue((yyvsp[0].float32)));}
#line 2292 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 103:
#line 327 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeExp) = new yal::ConstantNode(yal::BisonYyltypeToLocation(yylloc), yal::ConstantValue((yyvsp[0].float64)));}
#line 2298 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 104:
#line 328 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeExp) =  new yal::StringCreateNode (new yal::ConstantNode(yal::BisonYyltypeToLocation(yylloc), yal::ConstantValue((yyvsp[0].text))));}
#line 2304 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 105:
#line 331 "parser.y" /* yacc.c:1646  */
    {(yyvsp[-2].nodeArrayCtr)->addExpression((yyvsp[0].nodeExp)); (yyval.nodeArrayCtr)=(yyvsp[-2].nodeArrayCtr);}
#line 2310 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 106:
#line 332 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeArrayCtr) = new yal::ArrayCtrNode(yal::BisonYyltypeToLocation(yylloc)); (yyval.nodeArrayCtr)->addExpression((yyvsp[0].nodeExp));}
#line 2316 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 107:
#line 333 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeArrayCtr) = new yal::ArrayCtrNode(yal::BisonYyltypeToLocation(yylloc));}
#line 2322 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 108:
#line 336 "parser.y" /* yacc.c:1646  */
    { (yyval.nodeExp) = (yyvsp[-1].nodeArrayCtr);}
#line 2328 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
    break;


#line 2332 "/home/lbeernaert/Code/build/YAL-qtcreator/lib/parser/yal_bison_parser.cpp" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (&yylloc, scanner, state, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (&yylloc, scanner, state, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, scanner, state);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[1] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp, scanner, state);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, scanner, state, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, scanner, state);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp, scanner, state);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 342 "parser.y" /* yacc.c:1906  */

