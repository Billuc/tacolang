#include <stdlib.h>
#include <string.h>
#include "expression.h"
#include "../../utils/str_utils.h"

static void freeExpression(ExpressionElement *element);
static EvalExpressionData *evalExpression(ExpressionElement *element, SymbolElement **symbolTable);

// TODO : remove
static const TypeData intTypeData = {
    .is_base_type = true,
    .type_data = {.baseType = U32},
    .modifiers = NULL,
};

ExpressionElement *newExpression(char *operation)
{
    ExpressionElement *element = malloc(sizeof(ExpressionElement));
    element->operation = strdup(operation);
    element->free = freeExpression;
    element->eval = evalExpression;
    return element;
}

static void freeExpression(ExpressionElement *element)
{
    free(element->operation);
    free(element);
}

static EvalExpressionData *evalExpression(ExpressionElement *element, SymbolElement **symbolTable)
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