%option noyywrap noinput nounput yylineno
%{
#include <string.h>
#include <stdlib.h>
#include "taco.tab.h"
#include "utils/str_utils.h"

#define YY_USER_ACTION \
    { \
        yylloc.first_line = yylloc.last_line; \
        yylloc.first_column = yylloc.last_column; \
        yylloc.last_line = yylineno; \
        yylloc.last_column = yylloc.first_column + yyleng; \
    }
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
arr         { return ARR; }
0x{HEX_DIGIT}+  { yylval.integer = strtol(strremove(yytext, 'x'), NULL, 16); return INTEGER; }
0o{OCT_DIGIT}+  { yylval.integer = strtol(strremove(yytext, 'o'), NULL, 8); return INTEGER; }
0b{BIN_DIGIT}+  { yylval.integer = strtol(strremove(yytext, 'b'), NULL, 2); return INTEGER; }
({DIGIT}){1,3}((_)?{DIGIT}{3})*    { yylval.integer = atoi(strremove(yytext, '_')); return INTEGER; }
({DIGIT}){1,3}((_)?{DIGIT}{3})*\.{DIGIT}*  { yylval.floating = atof(strremove(yytext, '_')); return FLOAT; }
true        { yylval.boolean = 1; return BOOLEAN; }
false       { yylval.boolean = 0; return BOOLEAN; }
\/\/[^\n]*  { yylval.string = strdup(&yytext[2]); return COMMENT; }
'.{1}'          { yylval.character = yytext[1]; return CHARACTER; }
'\\n'            { yylval.character = '\n'; return CHARACTER; }
'\\r'            { yylval.character = '\r'; return CHARACTER; }
'\\t'            { yylval.character = '\t'; return CHARACTER; }
'\\\\'            { yylval.character = '\\'; return CHARACTER; }
'\\\''            { yylval.character = '\''; return CHARACTER; }
'\\\"'            { yylval.character = '\"'; return CHARACTER; }
'\\0'            { yylval.character = '\0'; return CHARACTER; }
'\\x{DIGIT}{2}'  { yylval.character = (char)strtol(&yytext[3], NULL, 16); return CHARACTER; }
\"[^\"]*\"      { yylval.string = strdup(strremove(yytext, '"')); return STRING; }
\\\\[^\n]*\n  { yylloc.last_column = 1; yylval.string = strdup(&yytext[2]); return MULTILINE_STRING; }
{VARNAME}   { yylval.string = strdup(yytext); return IDENTIFIER; }
{TYPE}      { yylval.string = strdup(yytext); return TYPEDEF; }
\n          { yylloc.last_column = 1; return ENDSTMT; }
.           { return yytext[0]; }
%%
