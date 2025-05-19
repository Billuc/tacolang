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
#include "ir/ast/block.h"
#include "ir/ast/declare.h"
#include "ir/ast/definition.h"
#include "ir/ast/expression.h"
#include "ir/ast/funccall.h"
#include "ir/ast/funcparam.h"
#include "ir/ast/funcdef.h"
#include "ir/ast/modifier.h"
#include "ir/ast/program.h"
#include "ir/ast/statement.h"
#include "ir/ast/typedef.h"
#include "ir/ast/value.h"
#include "ir/ast/variable.h"
}

%union {
    AssignElement *assign;
    BlockElement *block;
    DeclareElement *declare;
    DefinitionElement *definition;
    DefinitionList *definitionList;
    ExpressionElement *expression;
    FunctionCallElement *functionCall;
    FunctionArgumentList *arguments;
    FunctionDefinitionElement *functionDef;
    FunctionParameterList *parameters;
    FunctionParameterElement *parameter;
    ModifierList *modifierList;
    ModifierElement *modifier;
    ProgramElement *program;
    StatementElement *statement;
    StatementList *statementList;
    TypedefElement *typeDef;
    ValueElement *value;
    VariableElement *variable;
    int integer;
    char* string;
}

%token LET ENDSTMT FN FN_RETURN
%token <string> IDENTIFIER TYPEDEF MODIFIER
%token <integer> INTEGER

%type <parameters> func_params
%type <parameter> func_param
%type <assign> assignment
%type <block> block
%type <declare> declaration
%type <definition> definition
%type <definitionList> definitions
%type <expression> expression
%type <functionCall> funccall
%type <arguments> func_args
%type <functionDef> funcdef
%type <modifierList> modifiers
%type <modifier> modifier
%type <program> program
%type <statement> statement
%type <statementList> statements
%type <typeDef> typedef
%type <value> value
%type <variable> variable

%start program

%%
program: definitions { $$ = newProgram($1); eval($$); }

definitions: /* empty */ { $$ = newDefinitionList(); }
    | definitions definition ENDSTMT { push($1, $2); $$ = $1; }

definition: /* empty */ { $$ = NULL; }
    | funcdef { $$ = newFuncdefDefinition($1); }
    /* | declaration { $$ = newDeclare($1); } */ // Consts ?

statements: /* empty */ { $$ = newStatementList(); }
    | statements statement ENDSTMT { push($1, $2); $$ = $1; }

statement: /* empty */ { $$ = NULL; }
    | assignment    { $$ = newAssignmentStatement($1); }
    | declaration    { $$ = newDeclareStatement($1); }
    | value      { $$ = newValueStatement($1); }

funcdef: FN IDENTIFIER '(' func_params ')' FN_RETURN typedef block { $$ = newFunctionDefinition($2, $4, $7, $8); }
    | FN IDENTIFIER '(' ')' FN_RETURN typedef block { $$ = newFunctionDefinition($2, NULL, $6, $7); }

func_params: func_param { $$ = newFunctionParameterList(); push($$, $1); }
    | func_params ',' func_param { push($1, $3); $$ = $1; }

func_param: IDENTIFIER ':' typedef { $$ = newFunctionParameter($1, $3); }

block: '{' statements '}' { $$ = newBlock($2); }

declaration: LET IDENTIFIER ':' typedef '=' value { $$ = newDeclare($2, $4, $6); }
    | LET IDENTIFIER ':' '=' value { $$ = newDeclare($2, NULL, $5); }

assignment: variable '=' value { $$ = newAssign($1, $3); }

variable: IDENTIFIER { $$ = newIdentifierVariable($1); }

value: INTEGER { $$ = newIntegerValue($1); }
    | expression { $$ = newExpressionValue($1); }
    | funccall { $$ = newFunctionCallValue($1); }

funccall: IDENTIFIER '(' func_args ')' { $$ = newFunctionCall($1, $3); }
    | IDENTIFIER '(' ')' { $$ = newFunctionCall($1, NULL); }

func_args: value { $$ = newFunctionArgumentList(); push($$, $1); }
    | func_args ',' value { push($1, $3); $$ = $1; }

expression: '.' { $$ = newExpression("."); } 
/* INTEGER { $$ = newInteger($1); } */

typedef: modifiers TYPEDEF { $$ = newTypedef($1, $2); }

modifiers: /* empty */ { $$ = newModifierList(); }
    | modifiers modifier '.' { push($1, $2); $$ = $1; }

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