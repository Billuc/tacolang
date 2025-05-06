%{
#include <stdio.h>
#include "ast/ast.h"

extern int yylex (void);

void yyerror(const char* s);
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

%type <a> declaration assignment expression statement statements

%start program

%%
program: statements { evalAST($1, NULL); }

statements: /* empty */ { $$ = NULL; }
    | statements statement ENDSTMT { $$ = newStatement($1, $2); }
;

statement: /* empty */ { $$ = NULL; }
    | declaration   { $$ = $1; }
    | assignment    { $$ = $1; }
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

void yyerror(const char* s) {
    extern int yylineno;
    fprintf(stderr, "[ERR] %s - line %d\n", s, yylineno);
}