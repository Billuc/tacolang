%{
#include <stdio.h>
#include "ast/elements/assign.h"
#include "ast/elements/declare.h"
#include "ast/elements/expression.h"
#include "ast/elements/modifier.h"
#include "ast/elements/statement.h"
#include "ast/elements/typedef.h"
#include "ast/elements/value.h"

extern int yylex (void);

void yyerror(const char* s);
%}
%define parse.error verbose

%union {
    AssignElement *assign;
    DeclareElement *declare;
    ExpressionElement *expression;
    ModifierLink *modifierList;
    StatementElement *statement;
    StatementLink *statementList;
    TypedefElement *typeDef;
    ValueElement *value;
    int integer;
    char* string;
}

%token LET ENDSTMT
%token <string> IDENTIFIER TYPEDEF MODIFIER
%token <integer> INTEGER

%type <assign> assignment
%type <declare> declaration
%type <expression> expression
%type <modifierList> modifiers
%type <statement> statement
%type <statementList> statements
%type <typeDef> typedef
%type <value> value

%start program

%%
program: statements { eval($1, NULL); }

statements: /* empty */ { $$ = NULL; }
    | statements statement ENDSTMT { $$ = addStatement($1, $2); }
;

statement: /* empty */ { $$ = NULL; }
    | declaration   { $$ = newDeclarationStatement($1); }
    | assignment    { $$ = newAssignmentStatement($1); }
;

declaration: LET IDENTIFIER ':' typedef '=' value { $$ = newDeclare($2, $4, $6); }

assignment: IDENTIFIER '=' value { $$ = newAssign($1, $3); }

value: INTEGER { $$ = newIntegerValue($1); }
    | expression { $$ = newExpressionValue($1); }
;

expression: '.'; /* INTEGER { $$ = newInteger($1); } */

typedef: modifiers TYPEDEF { $$ = newTypedef($1, $2); }

modifiers: /* empty */ { $$ = NULL; }
    | modifiers MODIFIER '.' { $$ = addModifier($1, $2); }

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