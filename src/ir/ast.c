#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ast.h"

extern void yyerror(char *s);
static ASTElement *newAST(ASTType type, ASTData data, ASTElement *left, ASTElement *right);

/* ASSIGNMENT */

ASTElement *newAssign(char *name, ASTElement *value)
{
    printf("Assignment : [%s]\n", name);
    AssignData assignData = {.name = strdup(name)};
    ASTData data = {.assignData = assignData};
    return newAST(assign, data, NULL, value);
}

static void freeAssign(ASTElement *astElement)
{
    free(astElement->data.assignData.name);
    freeAST(astElement->right);
}

static void evalAssign(ASTElement *astElement, SymbolElement **symbolTable)
{
    char *variableName = astElement->data.assignData.name;
    SymbolData *symbol = getSymbol(*symbolTable, variableName);

    if (symbol == NULL)
    {
        char buf[100] = "";
        snprintf(buf, 100, "Variable '%s' has not been declared in this context", variableName);
        yyerror(buf);
    }

    // TODO : check type
}

/* DECLARATION */

ASTElement *newDeclare(char *name, char *type, ASTElement *value)
{
    printf("Declaration : [%s] of type [%s]\n", name, type);
    DeclareData declareData = {.name = strdup(name), .type = strdup(type)};
    ASTData data = {.declareData = declareData};
    return newAST(declare, data, NULL, value);
}

static void freeDeclare(ASTElement *astElement)
{
    free(astElement->data.declareData.name);
    free(astElement->data.declareData.type);
    freeAST(astElement->right);
}

static void evalDeclare(ASTElement *astElement, SymbolElement **symbolTable)
{
    char *variableName = astElement->data.declareData.name;
    SymbolData *symbol = getSymbol(*symbolTable, variableName);

    if (symbol != NULL)
    {
        char buf[100] = "";
        snprintf(buf, 100, "Variable %s has already been declared in this context", variableName);
        yyerror(buf);
    }

    SymbolData *data = malloc(sizeof(SymbolData));
    data->name = variableName;
    SymbolElement *newHead = putSymbol(*symbolTable, data);
    *symbolTable = newHead;

    // TODO : check type of initial value
}

/* EXPRESSION */

ASTElement *newExpression(char *operation, ASTElement *left, ASTElement *right)
{
    printf("Expression\n");
    ExpressionData expressionData = {.operation = strdup(operation)};
    ASTData data = {.expressionData = expressionData};
    return newAST(expression, data, left, right);
}

static void freeExpression(ASTElement *astElement)
{
    free(astElement->data.expressionData.operation);
    freeAST(astElement->left);
    freeAST(astElement->right);
}

static void evalExpression(ASTElement *astElement, SymbolElement **symbolTable)
{
    // TODO
}

/* INTEGER */

ASTElement *newInteger(int value)
{
    printf("Integer\n");
    IntegerData integerData = {.value = value};
    ASTData data = {.integerData = integerData};
    return newAST(integer, data, NULL, NULL);
}

static void freeInteger(ASTElement *astElement)
{
    return;
}

static void evalInteger(ASTElement *astElement, SymbolElement **symbolTable)
{
    // TODO
}

/* STATEMENT */

ASTElement *newStatement(ASTElement *previousStatements, ASTElement *stmt)
{
    printf("Statement\n");
    NoData noData = {};
    ASTData data = {.noData = noData};
    return newAST(statement, data, previousStatements, stmt);
}

static void freeStatement(ASTElement *astElement)
{
    freeAST(astElement->left);
    freeAST(astElement->right);
}

static void evalStatement(ASTElement *astElement, SymbolElement **symbolTable)
{
    evalAST(astElement->left, symbolTable);
    evalAST(astElement->right, symbolTable);
}

/* MODIFIER */

ASTElement *newModifier(char *modifierName, ASTElement *previousModifiers)
{
    printf("Modifier\n");
    ModifierData modifierData = {.modifier = strdup(modifierName)};
    ASTData data = {.modifierData = modifierData};
    return newAST(modifier, data, previousModifiers, NULL);
}

static void freeModifier(ASTElement *astElement)
{
    free(astElement->data.modifierData.modifier);
    freeAST(astElement->left);
}

static void evalModifier(ASTElement *astElement, SymbolElement **symbolTable)
{
    // TODO
}

/* TYPEDEF */

ASTElement *newTypedef(char *typeName, ASTElement *modifiers)
{
    printf("Typedef\n");
    TypeDefData typedefData = {.typename = strdup(typeName)};
    ASTData data = {.typedefData = typedefData};
    return newAST(typedefinition, data, modifiers, NULL);
}

/* General AST functions */

static ASTElement *newAST(ASTType type, ASTData data, ASTElement *left, ASTElement *right)
{
    ASTElement *ast = malloc(sizeof(ASTElement));
    ast->type = type;
    ast->data = data;
    ast->left = left;
    ast->right = right;
    return ast;
}

void freeAST(ASTElement *astElement)
{
    if (astElement == NULL)
        return;

    switch (astElement->type)
    {
    case assign:
        freeAssign(astElement);
        break;
    case declare:
        freeDeclare(astElement);
        break;
    case expression:
        freeExpression(astElement);
        break;
    case integer:
        freeInteger(astElement);
        break;
    case statement:
        freeStatement(astElement);
        break;
    default:
        fprintf(stderr, "Unknown AST element type: %x", astElement->type);
        break;
    }

    free(astElement);
}

void evalAST(ASTElement *astElement, SymbolElement **symbolTable)
{
    if (astElement == NULL)
        return;

    SymbolElement *localTable = createSymbolTable();
    if (symbolTable == NULL)
    {
        symbolTable = &localTable;
    }

    switch (astElement->type)
    {
    case assign:
        evalAssign(astElement, symbolTable);
        break;
    case declare:
        evalDeclare(astElement, symbolTable);
        break;
    case expression:
        evalExpression(astElement, symbolTable);
        break;
    case integer:
        evalInteger(astElement, symbolTable);
        break;
    case statement:
        evalStatement(astElement, symbolTable);
        break;
    default:
        fprintf(stderr, "Unknown AST element type: %x", astElement->type);
        break;
    }

    freeSymbolTable(localTable);
}