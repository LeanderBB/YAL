/* This file was automatically generated.  Do not edit! */
#define YALSTParserTOKENTYPE yal::frontend::TokenInfo
#define YALSTParserARG_PDECL , yal::frontend::STParser *pParser 
void YALSTParser(void *yyp,int yymajor,YALSTParserTOKENTYPE yyminor YALSTParserARG_PDECL);
#if defined(YYTRACKMAXSTACKDEPTH)
int YALSTParserStackPeak(void *p);
#endif
#if !defined(YALSTParser_ENGINEALWAYSONSTACK)
void YALSTParserFree(void *p,void(*freeProc)(void *));
#endif
void YALSTParserFinalize(void *p);
#if !defined(YALSTParser_ENGINEALWAYSONSTACK)
void *YALSTParserAlloc(void *(*mallocProc)(YYMALLOCARGTYPE));
#endif
void YALSTParserInit(void *yypParser);
#if !defined(NDEBUG)
void YALSTParserTrace(FILE *TraceFILE,char *zTracePrompt);
#endif
#define YALSTParserARG_STORE yypParser->pParser  = pParser 
#define YALSTParserARG_FETCH  yal::frontend::STParser *pParser  = yypParser->pParser 
#define YALSTParserARG_SDECL  yal::frontend::STParser *pParser ;
#define YAL_TOKEN_BOOL_LITERAL                   68
#define YAL_TOKEN_FLOAT_LITERAL_64               67
#define YAL_TOKEN_FLOAT_LITERAL_32               66
#define YAL_TOKEN_FLOAT_LITERAL                  65
#define YAL_TOKEN_INTEGER_LITERAL_U64            64
#define YAL_TOKEN_INTEGER_LITERAL_U32            63
#define YAL_TOKEN_INTEGER_LITERAL_U16            62
#define YAL_TOKEN_INTEGER_LITERAL_U8             61
#define YAL_TOKEN_INTEGER_LITERAL_I64            60
#define YAL_TOKEN_INTEGER_LITERAL_I32            59
#define YAL_TOKEN_INTEGER_LITERAL_I16            58
#define YAL_TOKEN_INTEGER_LITERAL_I8             57
#define YAL_TOKEN_INTEGER_LITERAL                56
#define YAL_TOKEN_LET                            55
#define YAL_TOKEN_VAR                            54
#define YAL_TOKEN_RETURN                         53
#define YAL_TOKEN_IMPL                           52
#define YAL_TOKEN_FUNCTION                       51
#define YAL_TOKEN_PAR_END                        50
#define YAL_TOKEN_PAR_BEGIN                      49
#define YAL_TOKEN_COMMA                          48
#define YAL_TOKEN_SCOPE_END                      47
#define YAL_TOKEN_SCOPE_BEGIN                    46
#define YAL_TOKEN_STRUCT                         45
#define YAL_TOKEN_SEMI_COLON                     44
#define YAL_TOKEN_COLON                          43
#define YAL_TOKEN_TYPE                           42
#define YAL_TOKEN_MUT                            41
#define YAL_TOKEN_ARRAY_END                      40
#define YAL_TOKEN_ARRAY_BEGIN                    39
#define YAL_TOKEN_TYPE_DOUBLE                    38
#define YAL_TOKEN_TYPE_FLOAT                     37
#define YAL_TOKEN_TYPE_UINT64                    36
#define YAL_TOKEN_TYPE_INT64                     35
#define YAL_TOKEN_TYPE_UINT32                    34
#define YAL_TOKEN_TYPE_INT32                     33
#define YAL_TOKEN_TYPE_UINT16                    32
#define YAL_TOKEN_TYPE_INT16                     31
#define YAL_TOKEN_TYPE_UINT8                     30
#define YAL_TOKEN_TYPE_INT8                      29
#define YAL_TOKEN_TYPE_BOOL                      28
#define YAL_TOKEN_SELF                           27
#define YAL_TOKEN_IDENTIFIER                     26
#define YAL_TOKEN_END                            25
#define YAL_TOKEN_DOT                            24
#define YAL_TOKEN_REFERENCE                      23
#define YAL_TOKEN_DEREF                          22
#define YAL_TOKEN_NOT                            21
#define YAL_TOKEN_BIT_NOT                        20
#define YAL_TOKEN_NEGATE                         19
#define YAL_TOKEN_MOD                            18
#define YAL_TOKEN_DIV                            17
#define YAL_TOKEN_MULT                           16
#define YAL_TOKEN_MINUS                          15
#define YAL_TOKEN_PLUS                           14
#define YAL_TOKEN_LE                             13
#define YAL_TOKEN_GE                             12
#define YAL_TOKEN_LT                             11
#define YAL_TOKEN_GT                             10
#define YAL_TOKEN_NE                              9
#define YAL_TOKEN_EQ                              8
#define YAL_TOKEN_BIT_AND                         7
#define YAL_TOKEN_BIT_XOR                         6
#define YAL_TOKEN_BIT_OR                          5
#define YAL_TOKEN_AND                             4
#define YAL_TOKEN_OR                              3
#define YAL_TOKEN_AS                              2
#define YAL_TOKEN_ASSIGN                          1
#define INTERFACE 0
