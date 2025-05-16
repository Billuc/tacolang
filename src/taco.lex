%option noyywrap
%option yylineno
%{
#include <string.h>
#include "taco.tab.h"
#include "utils/str_utils.h"
%}

DIGIT [0-9]
VARNAME [a-z][a-zA-Z0-9_]*
TYPE [A-Z][a-zA-Z0-9_]*

%%
[ \t]+      ;
let         { return LET; }
fn          { return FN; }
->          { return FN_RETURN; } 
mut         { yylval.string = strdup(yytext); return MODIFIER; }
ref         { yylval.string = strdup(yytext); return MODIFIER; }
opt         { yylval.string = strdup(yytext); return MODIFIER; }
{DIGIT}+    { yylval.integer = atoi(yytext); return INTEGER; }
{VARNAME}   { yylval.string = strdup(yytext); return IDENTIFIER; }
{TYPE}      { yylval.string = strdup(yytext); return TYPEDEF; }
\n          { return ENDSTMT; }
.           { return yytext[0]; }
%%
