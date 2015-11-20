%option noyywrap
%option reentrant bison-bridge
%option bison-locations
%option extra-type="struct FlexState *"
%{
#include <stdio.h>
#include <yal/parser/flex_utils.h>
#include "yal_bison_parser.hpp"

// update bison location tracking
#define YY_USER_ACTION \
    yylloc->first_line = yylloc->last_line; \
    yylloc->first_column = yylloc->last_column; \
    for(int i = 0; yytext[i] != '\0'; i++) { \
        if(yytext[i] == '\n') { \
            UpdateLineCounter(yyscanner); \
            yylloc->last_line++; \
            yylloc->last_column = 0; \
        } \
        else { \
            UpdateOffset(yyscanner); \
            yylloc->last_column++; \
        } \
    }

// Override input
#define YY_INPUT(buf, result, max_size) \
            result = yy_input_proc(buf, max_size, yyscanner);
extern int yy_input_proc(char* buf, int size, yyscan_t yyscanner);
extern void UpdateLineCounter(yyscan_t yyscanner);
extern void UpdateOffset(yyscan_t yyscanner);

%}

INTEGER [0-9]+
DECIMAL [0-9]+\.[0-9]+
ID [a-zA-Z][a-zA-Z0-9_]*

%%

^#.+\n {}

native {return TK_NATIVE;}

@excall {return TK_ATTRIB_EXCALL;}

\n {return TK_NL; }

true {return yal::FlexReadBool(yylval, yytext);}

false {return yal::FlexReadBool(yylval, yytext);}

func {return TK_FUNC_BEGIN;}

var {return TK_VAR_DECL;}

if {return TK_IF;}

elif {return TK_ELIF;}

else {return TK_ELSE;}

end {return TK_END;}

and {return TK_AND;}

or {return TK_OR;}

not {return TK_NOT;}

return {return TK_RETURN;}

while {return TK_WHILE;}

int32 {return TK_TYPE_INT32;}

uint32 {return TK_TYPE_UINT32;}

int64 {return TK_TYPE_INT64;}

uint64 {return TK_TYPE_UINT64;}

float32 {return TK_TYPE_F32;}

float64 {return TK_TYPE_F64;}

bool {return TK_TYPE_BOOL;}

string {return TK_TYPE_STRING;}

print {return TK_PRINT;}

self {return TK_SELF;}

% {return TK_MOD;}

"." {return TK_DOT;}

"[" {return TK_ARRAY_BGN;}

"]" {return TK_ARRAY_END;}

">>" {return TK_SHIFT_RIGHT;}

"<<" {return TK_SHIFT_LEFT;}

"^" {return TK_BIT_XOR;}

"~" {return TK_BIT_TIL;}

"&" {return TK_BIT_AND;}

"|" {return TK_BIT_OR;}

">=" {return TK_CMP_GE;}

"<" {return TK_CMP_LT;}

"<=" {return TK_CMP_LE;}

"==" {return TK_CMP_EQ;}

"!=" {return TK_CMP_NE;}

"=" {return TK_OP_ASSIGN;}

"+=" {return TK_OP_ASSIGN_PLUS;}

"-=" {return TK_OP_ASSIGN_MINUS;}

"*=" {return TK_OP_ASSIGN_MULT;}

"/=" {return TK_OP_ASSIGN_DIV;}

"&=" {return TK_OP_ASSIGN_AND;}

"|=" {return TK_OP_ASSIGN_OR;}

"^=" {return TK_OP_ASSIGN_XOR;}

">>=" {return TK_OP_ASSIGN_SHIFT_RIGHT;}

"<<=" {return TK_OP_ASSIGN_SHIFT_LEFT;}

"+" {return TK_OP_PLUS;}

"-" {return TK_OP_MINUS;}

"*" {return TK_OP_MULT;}

"/" {return TK_OP_DIV;}

">" {return TK_CMP_GT;}


{DECIMAL} { return yal::FlexReadFloat32(yylval, yytext);}

{DECIMAL}l { return yal::FlexReadFloat64(yylval, yytext);}

{INTEGER}u {return yal::FlexReadUnsignedInteger32(yylval, yytext);}

{INTEGER}ul {return yal::FlexReadUnsignedInteger64(yylval, yytext);}

{INTEGER} { return yal::FlexReadInteger32(yylval, yytext);}

{INTEGER}l { return yal::FlexReadInteger64(yylval, yytext);}

{ID} {return yal::FlexReadId(yylval, yytext);}

\"[^\"]+\" { return yal::FlexReadText(yylval, yytext);}

[\t ]+ ;

. {return yytext[0];}
%%
