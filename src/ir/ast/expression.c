#include <stdlib.h>
#include <string.h>
#include "expression.h"

ExpressionElement *newExpression(char *operation)
{
    ExpressionElement *element = malloc(sizeof(ExpressionElement));
    element->operation = strdup(operation);
    return element;
}

void freeExpression(ExpressionElement *element)
{
    free(element->operation);
    free(element);
}

// TODO : remove
const TypeData intTypeData = {
    .is_base_type = true,
    .type_data = {.baseType = U32},
    .mutable = false,
    .reference = false,
    .array = false,
    .array_len = 0,
    .optional = 0,
};

EvalExpressionData *evalExpression(ExpressionElement *element, SymbolElement **symbolTable)
{
    EvalExpressionData *expressionData = malloc(sizeof(EvalExpressionData));
    EvalType *evalType = malloc(sizeof(EvalType));
    // TODO: adapt
    EvalTypeData etd = {.variableTypeData = intTypeData};
    evalType->type = t_variable;
    evalType->data = etd;
    expressionData->type = evalType;
    return expressionData;
}