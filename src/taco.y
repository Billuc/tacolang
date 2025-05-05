%{
#include <stdio.h>
#include "ast/ast.h"

extern int yylineno;
%}
%define parse.error verbose

%union {
    ASTElement *a;
    int integer;
    char* string;
}

%token LET ENDSTMT
%token <string> IDENTIFIER TYPEDEF
%token <integer> INTEGER

%type <a> declaration assignment expression

%start program

%%
program: statements;

statements: /* empty */ 
    | statements statement ENDSTMT
;

statement: /* empty */
    | declaration
    | assignment
;

declaration: LET IDENTIFIER ':' TYPEDEF '=' expression { $$ = newDeclare($2, $4, $6); }

assignment: IDENTIFIER '=' expression { $$ = newAssign($1, $3); }

expression: INTEGER { $$ = newInteger($1); }

%%

void main(int argc, char* argv[]) {
    extern FILE* yyin;

    ++argv; --argc;

    yyin = fopen(argv[0], "r");
    yyparse();
}

void yyerror(char* s) {
    fprintf(stderr, "[ERR] %s in line %d\n", s, yylineno);
}