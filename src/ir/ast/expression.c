#include "expression.h"
#include "utils/str_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void freeExpression(ExpressionElement *element);
static ExpressionData *evalExpression(ExpressionElement *element,
                                      EvalContext *context);

// TODO : remove
static const SimpleType intTypeData = {
    .is_base_type = true,
    .type_data = {.base_type = U32},
    .modifiers = NULL,
};

ExpressionElement *newExpression(char *operation, location_t location) {
  printf("Creating new expression: %s\n", operation);

  ExpressionElement *element = malloc(sizeof(ExpressionElement));
  element->operation = strdup(operation);
  element->location = location;
  element->free = freeExpression;
  element->eval = evalExpression;
  return element;
}

static void freeExpression(ExpressionElement *element) {
  free(element->operation);
  free(element);
}

static void freeExpressionData(ExpressionData *data) {
  free(data->generated_code);
  free_type(data->type);
  free(data);
}

static ExpressionData *evalExpression(ExpressionElement *element,
                                      EvalContext *context) {
  if (element == NULL)
    return NULL;

  printf("Evaluating expression: %s\n", element->operation);

  ExpressionData *expressionData = malloc(sizeof(ExpressionData));
  expressionData->free = freeExpressionData;

  Type *exp_type = malloc(sizeof(Type));
  // TODO: adapt
  TypeData etd = {.variable_type = intTypeData};
  exp_type->type_type = t_variable;
  exp_type->type_data = etd;
  expressionData->type = exp_type;
  expressionData->generated_code = strdup(element->operation);
  return expressionData;
}
