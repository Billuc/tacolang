%{
#include <stdio.h>

extern int yylex(void);
extern int yyparse(void);
extern int yywrap() { return 1; }

void yyerror(const char* s);
%}
%define parse.error verbose

%code requires {
#include "ir/ast/assign.h"
#include "ir/ast/declare.h"
#include "ir/ast/expression.h"
#include "ir/ast/modifier.h"
#include "ir/ast/statement.h"
#include "ir/ast/typedef.h"
#include "ir/ast/value.h"
#include "ir/ast/variable.h"
}

%union {
    AssignElement *assign;
    DeclareElement *declare;
    ExpressionElement *expression;
    ModifierLink *modifierList;
    ModifierElement *modifier;
    StatementElement *statement;
    StatementLink *statementList;
    TypedefElement *typeDef;
    ValueElement *value;
    VariableElement *variable;
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
%type <modifier> modifier
%type <statement> statement
%type <statementList> statements
%type <typeDef> typedef
%type <value> value
%type <variable> variable

%start program

%%
program: statements { evalStatementList($1, NULL); }

statements: /* empty */ { $$ = NULL; }
    | statements statement ENDSTMT { $$ = addStatement($1, $2); }
;

statement: /* empty */ { $$ = NULL; }
    | assignment    { $$ = newAssignmentStatement($1); }
;

declaration: LET IDENTIFIER ':' typedef { $$ = newDeclare($2, $4); }

assignment: variable '=' value { $$ = newAssign($1, $3); }

variable: IDENTIFIER { $$ = newIdentifierVariable($1); }
    | declaration { $$ = newDeclareVariable($1); }
;

value: INTEGER { $$ = newIntegerValue($1); }
    | expression { $$ = newExpressionValue($1); }
;

expression: '.' { $$ = newExpression("."); } 
/* INTEGER { $$ = newInteger($1); } */

typedef: modifiers TYPEDEF { $$ = newTypedef($1, $2); }

modifiers: /* empty */ { $$ = NULL; }
    | modifiers modifier '.' { $$ = addModifier($1, $2); }

modifier: MODIFIER { $$ = newModifier($1); }

%%

int main(int argc, char* argv[]) {
    extern FILE* yyin;

    ++argv; --argc;

    yyin = fopen(argv[0], "r");
    yyparse();

    return 1;
}

void yyerror(const char* s) {
    extern int yylineno;
    fprintf(stderr, "[ERR] %s - line %d\n", s, yylineno);
}