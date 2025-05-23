#include "variable.h"
#include "utils/str_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void yyerror(const char *s);
static void freeVariable(VariableElement *variableEl);
static VariableData *evalVariable(VariableElement *variableElement,
                                  EvalContext *context);

VariableElement *newIdentifierVariable(char *identifier, location_t location) {
  printf("Creating new identifier variable: %s\n", identifier);

  VariableElement *variable =
      (VariableElement *)malloc(sizeof(VariableElement));
  if (!variable)
    return NULL;

  variable->free = freeVariable;
  variable->eval = evalVariable;
  variable->element_type = var_identifier;
  variable->element_data.identifier = strdup(identifier);
  variable->location = location;

  if (!variable->element_data.identifier) {
    free(variable);
    return NULL;
  }

  return variable;
}

static void freeVariable(VariableElement *variableEl) {
  if (!variableEl)
    return;

  switch (variableEl->element_type) {
  case var_identifier:
    free(variableEl->element_data.identifier);
    break;
  }

  free(variableEl);
}

static void freeVariableData(VariableData *variableData) {
  if (!variableData)
    return;

  free(variableData->generated_code);
  free_type(variableData->variable_type);
  free(variableData);
}

static VariableData *evalVariable(VariableElement *variableElement,
                                  EvalContext *context) {
  if (!variableElement)
    return NULL;

  printf("Evaluating variable: %s\n", variableElement->element_data.identifier);

  VariableData *result = malloc(sizeof(VariableData));
  if (!result)
    return NULL;
  result->free = freeVariableData;

  switch (variableElement->element_type) {
  case var_identifier: {
    SymbolData *symbol =
        findSymbol(context, variableElement->element_data.identifier);
    if (!symbol) {
      print_error(variableElement->location,
                  "Variable '%s' is not declared in this context",
                  variableElement->element_data.identifier);
      free(result);
      return NULL;
    }
    result->variable_type = copy_type(symbol->type);
    result->generated_code = strdup(variableElement->element_data.identifier);
    break;
  }
  }

  return result;
}
