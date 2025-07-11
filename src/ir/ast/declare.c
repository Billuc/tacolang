#include "declare.h"
#include "ir/eval/type.h"
#include "utils/str_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void freeDeclare(DeclareElement *element);
static DeclareData *evalDeclare(DeclareElement *declareElement,
                                EvalContext *context);

DeclareElement *newDeclare(char *name, TypedefElement *type,
                           ValueElement *value, location_t location) {
  printf("Creating new declaration: %s\n", name);

  DeclareElement *element = malloc(sizeof(DeclareElement));
  element->name = strdup(name);
  element->type = type;
  element->value = value;
  element->location = location;
  element->free = freeDeclare;
  element->eval = evalDeclare;
  return element;
}

static void freeDeclare(DeclareElement *element) {
  free(element->name);
  if (element->type != NULL)
    element->type->free(element->type);
  element->value->free(element->value);
  free(element);
}

static void freeDeclareData(DeclareData *data) {
  free(data->generated_code);
  free(data->symbol_name);
  free_type(data->symbol_type);
  free(data);
}

static DeclareData *evalDeclare(DeclareElement *declareElement,
                                EvalContext *context) {
  if (declareElement == NULL)
    return NULL;

  printf("Evaluating declaration: %s\n", declareElement->name);

  char *variableName = declareElement->name;
  SymbolData *symbol = findSymbol(context, variableName);

  if (symbol != NULL) {
    print_error(declareElement->location,
                "Variable '%s' has already been declared in this context",
                variableName);
    return NULL;
  }

  ValueData *valueData =
      declareElement->value->eval(declareElement->value, context);

  SymbolData *newSymbol = malloc(sizeof(SymbolData));
  newSymbol->name = strdup(declareElement->name);

  if (declareElement->type != NULL) {
    TypedefData *typedefData =
        declareElement->type->eval(declareElement->type, context);
    newSymbol->type = copy_type(typedefData->type);
    typedefData->free(typedefData);
  } else {
    newSymbol->type = copy_type(valueData->value_type);
  }

  putSymbol(context, newSymbol);

  DeclareData *data = malloc(sizeof(DeclareData));
  data->free = freeDeclareData;
  data->symbol_name = strdup(variableName);
  data->symbol_type = copy_type(newSymbol->type);
  data->generated_code = print_c_type(data->symbol_type);
  strappend(&data->generated_code, " ");
  strappend(&data->generated_code, variableName);

  if (!compare_type(valueData->value_type, newSymbol->type)) {
    char *value_type_str = print_type(valueData->value_type);
    char *symbol_type_str = print_type(newSymbol->type);

    print_error(declareElement->location,
                "Couldn't assign value of type '%s' to variable of type '%s'",
                value_type_str, symbol_type_str);

    data->free(data);
    free(value_type_str);
    free(symbol_type_str);
  }

  strappend(&data->generated_code, " = ");
  strappend(&data->generated_code, valueData->generated_code);

  valueData->free(valueData);
  return data;
}
