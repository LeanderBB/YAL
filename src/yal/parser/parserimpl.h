/* This file was automatically generated.  Do not edit! */
#define YALParserTOKENTYPE yal::TokenInfo
#define YALParserARG_PDECL , yal::Parser *pParser 
void YALParser(void *yyp,int yymajor,YALParserTOKENTYPE yyminor YALParserARG_PDECL);
#if defined(YYTRACKMAXSTACKDEPTH)
int YALParserStackPeak(void *p);
#endif
#if !defined(YALParser_ENGINEALWAYSONSTACK)
void YALParserFree(void *p,void(*freeProc)(void *));
#endif
void YALParserFinalize(void *p);
#if !defined(YALParser_ENGINEALWAYSONSTACK)
void *YALParserAlloc(void *(*mallocProc)(YYMALLOCARGTYPE));
#endif
void YALParserInit(void *yypParser);
#if !defined(NDEBUG)
void YALParserTrace(FILE *TraceFILE,char *zTracePrompt);
#endif
#define YALParserARG_STORE yypParser->pParser  = pParser 
#define YALParserARG_FETCH  yal::Parser *pParser  = yypParser->pParser 
#define YALParserARG_SDECL  yal::Parser *pParser ;
#define YAL_TOKEN_BOOL_LITERAL                   54
#define YAL_TOKEN_DECIMAL_LITERAL                53
#define YAL_TOKEN_INTEGER_LITERAL                52
#define YAL_TOKEN_LET                            51
#define YAL_TOKEN_VAR                            50
#define YAL_TOKEN_RETURN                         49
#define YAL_TOKEN_COMMA                          48
#define YAL_TOKEN_PAR_END                        47
#define YAL_TOKEN_PAR_BEGIN                      46
#define YAL_TOKEN_FUNCTION                       45
#define YAL_TOKEN_SEMI_COLON                     44
#define YAL_TOKEN_SCOPE_END                      43
#define YAL_TOKEN_SCOPE_BEGIN                    42
#define YAL_TOKEN_STRUCT                         41
#define YAL_TOKEN_COLON                          40
#define YAL_TOKEN_TYPE                           39
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
#define YAL_TOKEN_REFERENCE                      25
#define YAL_TOKEN_MUT                            24
#define YAL_TOKEN_IDENTIFIER                     23
#define YAL_TOKEN_END                            22
#define YAL_TOKEN_DOT                            21
#define YAL_TOKEN_NOT                            20
#define YAL_TOKEN_BIT_NOT                        19
#define YAL_TOKEN_NEGATE                         18
#define YAL_TOKEN_MOD                            17
#define YAL_TOKEN_DIV                            16
#define YAL_TOKEN_MULT                           15
#define YAL_TOKEN_MINUS                          14
#define YAL_TOKEN_PLUS                           13
#define YAL_TOKEN_LE                             12
#define YAL_TOKEN_GE                             11
#define YAL_TOKEN_LT                             10
#define YAL_TOKEN_GT                              9
#define YAL_TOKEN_NE                              8
#define YAL_TOKEN_EQ                              7
#define YAL_TOKEN_BIT_AND                         6
#define YAL_TOKEN_BIT_XOR                         5
#define YAL_TOKEN_BIT_OR                          4
#define YAL_TOKEN_AND                             3
#define YAL_TOKEN_OR                              2
#define YAL_TOKEN_ASSIGN                          1
#define INTERFACE 0
