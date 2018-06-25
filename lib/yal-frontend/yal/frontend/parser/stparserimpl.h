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
#define YAL_TOKEN_BOOL_LITERAL                   67
#define YAL_TOKEN_FLOAT_LITERAL_64               66
#define YAL_TOKEN_FLOAT_LITERAL_32               65
#define YAL_TOKEN_FLOAT_LITERAL                  64
#define YAL_TOKEN_INTEGER_LITERAL_U64            63
#define YAL_TOKEN_INTEGER_LITERAL_U32            62
#define YAL_TOKEN_INTEGER_LITERAL_U16            61
#define YAL_TOKEN_INTEGER_LITERAL_U8             60
#define YAL_TOKEN_INTEGER_LITERAL_I64            59
#define YAL_TOKEN_INTEGER_LITERAL_I32            58
#define YAL_TOKEN_INTEGER_LITERAL_I16            57
#define YAL_TOKEN_INTEGER_LITERAL_I8             56
#define YAL_TOKEN_INTEGER_LITERAL                55
#define YAL_TOKEN_LET                            54
#define YAL_TOKEN_VAR                            53
#define YAL_TOKEN_RETURN                         52
#define YAL_TOKEN_IMPL                           51
#define YAL_TOKEN_FUNCTION                       50
#define YAL_TOKEN_PAR_END                        49
#define YAL_TOKEN_PAR_BEGIN                      48
#define YAL_TOKEN_COMMA                          47
#define YAL_TOKEN_SCOPE_END                      46
#define YAL_TOKEN_SCOPE_BEGIN                    45
#define YAL_TOKEN_STRUCT                         44
#define YAL_TOKEN_SEMI_COLON                     43
#define YAL_TOKEN_COLON                          42
#define YAL_TOKEN_TYPE                           41
#define YAL_TOKEN_REFERENCE                      40
#define YAL_TOKEN_MUT                            39
#define YAL_TOKEN_ARRAY_END                      38
#define YAL_TOKEN_ARRAY_BEGIN                    37
#define YAL_TOKEN_TYPE_DOUBLE                    36
#define YAL_TOKEN_TYPE_FLOAT                     35
#define YAL_TOKEN_TYPE_UINT64                    34
#define YAL_TOKEN_TYPE_INT64                     33
#define YAL_TOKEN_TYPE_UINT32                    32
#define YAL_TOKEN_TYPE_INT32                     31
#define YAL_TOKEN_TYPE_UINT16                    30
#define YAL_TOKEN_TYPE_INT16                     29
#define YAL_TOKEN_TYPE_UINT8                     28
#define YAL_TOKEN_TYPE_INT8                      27
#define YAL_TOKEN_TYPE_BOOL                      26
#define YAL_TOKEN_SELF                           25
#define YAL_TOKEN_IDENTIFIER                     24
#define YAL_TOKEN_END                            23
#define YAL_TOKEN_DOT                            22
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
