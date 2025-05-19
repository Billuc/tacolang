%option noyywrap
%option yylineno
%{
#include <string.h>
#include <stdlib.h>
#include "taco.tab.h"
#include "utils/str_utils.h"
%}

DIGIT [0-9]
OCT_DIGIT [0-7]
HEX_DIGIT [0-9a-fA-F]
BIN_DIGIT [01]
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
0x{HEX_DIGIT}+  { yylval.integer = strtol(yytext, NULL, 16); return INTEGER; }
0o{OCT_DIGIT}+  { yylval.integer = strtol(strremove(yytext, 'o'), NULL, 8); return INTEGER; }
0b{BIN_DIGIT}+  { yylval.integer = strtol(yytext, NULL, 2); return INTEGER; }
({DIGIT}){1,2}(_{DIGIT}{3})*    { yylval.integer = atoi(strremove(yytext, '_')); return INTEGER; }
{VARNAME}   { yylval.string = strdup(yytext); return IDENTIFIER; }
{TYPE}      { yylval.string = strdup(yytext); return TYPEDEF; }
\n          { return ENDSTMT; }
.           { return yytext[0]; }
%%
