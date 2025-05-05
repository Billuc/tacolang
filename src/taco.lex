%option noyywrap
%option yylineno
%{
#include <string.h>
#include "ast/ast.h"
#include "taco.tab.h"
%}

DIGIT [0-9]
VARNAME [a-z][a-zA-Z0-9_]*
TYPE [A-Z][a-zA-Z0-9_]*

%%
[ \t]+      ;
let         { return LET; }
{DIGIT}+    { yylval.integer = atoi(yytext); return INTEGER; }
{VARNAME}   { yylval.string = yytext; return IDENTIFIER; }
{TYPE}      { yylval.string = yytext; return TYPEDEF; }
\n          { return ENDSTMT; }
.           { return yytext[0]; }
%%

/* #define yyterminate() return END */
