#include <stdlib.h>
#include <string.h>
#include "ast.h"

ASTElement *newAST(ASTType type, ASTData data, ASTElement *left, ASTElement *right)
{
    ASTElement *ast = malloc(sizeof(ASTElement));
    ast->type = type;
    ast->data = data;
    ast->left = left;
    ast->right = right;
    return ast;
}

ASTElement *newAssign(char *name, ASTElement *value)
{
    printf("Identifier : [%s]\n", name);
    AssignData assignData = {.name = strdup(name)};
    ASTData data = {.assignData = assignData};
    return newAST(assign, data, NULL, value);
}

ASTElement *newDeclare(char *name, char *type, ASTElement *value)
{
    DeclareData declareData = {.name = strdup(name), .type = strdup(type)};
    ASTData data = {.declareData = declareData};
    return newAST(declare, data, NULL, value);
}

ASTElement *newExpression(char *operation, ASTElement *left, ASTElement *right)
{
    ExpressionData expressionData = {.operation = strdup(operation)};
    ASTData data = {.expressionData = expressionData};
    return newAST(expression, data, left, right);
}

ASTElement *newInteger(int value)
{
    IntegerData integerData = {.value = value};
    ASTData data = {.integerData = integerData};
    return newAST(integer, data, NULL, NULL);
}
