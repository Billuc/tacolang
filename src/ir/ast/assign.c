#include "assign.h"
#include "utils/str_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void freeAssign(AssignElement *assignElement);
static Assign *evalAssign(AssignElement *assignElement, EvalContext *context);

AssignElement *newAssign(VariableElement *left, ValueElement *right,
                         location_t loc) {
  printf("Creating new assignment\n");

  AssignElement *element = malloc(sizeof(AssignElement));
  element->left = left;
  element->right = right;
  element->location = loc;
  element->free = freeAssign;
  element->eval = evalAssign;
  return element;
}

static void freeAssign(AssignElement *assignElement) {
  assignElement->left->free(assignElement->left);
  assignElement->right->free(assignElement->right);
  free(assignElement);
}

static bool canAssign(VariableData *variable) {
  if (variable == NULL)
    return false;

  TypeModifierList *modifiers =
      variable->variable_type->type_data.variable_type.modifiers;
  if (modifiers == NULL || modifiers->size == 0)
    return false;

  TypeModifier *firstModifier = (TypeModifier *)modifiers->head->data;
  return firstModifier->modifier_type == tm_mutable;
}

static void freeAssignData(Assign *data) {
  if (data == NULL)
    return;

  free(data->generated_code);
  free(data);
}

static Assign *evalAssign(AssignElement *assignElement, EvalContext *context) {
  if (assignElement == NULL)
    return NULL;

  printf("Evaluating assignment\n");

  VariableElement *variable = assignElement->left;
  VariableData *variableData = variable->eval(variable, context);

  if (variableData == NULL) {
    print_error(assignElement->location,
                "Couldn't evaluate variable correctly.");
    return NULL;
  }

  if (!canAssign(variableData)) {
    print_error(assignElement->location,
                "You are trying to assign a value to a non mutable variable.");
    variableData->free(variableData);
    return NULL;
  }

  ValueElement *value = assignElement->right;
  ValueData *valueData = value->eval(value, context);
  if (valueData == NULL) {
    print_error(assignElement->location, "Couldn't evaluate value correctly.");
    variableData->free(variableData);
    return NULL;
  }

  if (!compare_type(valueData->value_type, variableData->variable_type)) {
    char *value_type_str = print_type(valueData->value_type);
    char *symbol_type_str = print_type(variableData->variable_type);

    print_error(assignElement->location,
                "Couldn't assign value of type '%s' to variable of type '%s'",
                value_type_str, symbol_type_str);

    free(value_type_str);
    free(symbol_type_str);
    valueData->free(valueData);
    variableData->free(variableData);
    return NULL;
  }

  Assign *result = malloc(sizeof(Assign));
  result->free = freeAssignData;
  char *s1 = strconcat(variableData->generated_code, "=");
  char *s2 = strconcat(s1, valueData->generated_code);
  result->generated_code = s2;
  free(s1);

  valueData->free(valueData);
  variableData->free(variableData);

  return result;
}
