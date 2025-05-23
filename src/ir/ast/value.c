#include "value.h"
#include "utils/str_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static ValueData *evalValue(ValueElement *valueElement, EvalContext *context);

static const SimpleType intTypeData = {
    .is_base_type = true,
    .type_data = {.base_type = U32},
    .modifiers = NULL,
};
static const SimpleType floatTypeData = {
    .is_base_type = true,
    .type_data = {.base_type = F32},
    .modifiers = NULL,
};
static const SimpleType boolTypeData = {
    .is_base_type = true,
    .type_data = {.base_type = Boolean},
    .modifiers = NULL,
};
static const SimpleType charTypeData = {
    .is_base_type = true,
    .type_data = {.base_type = U8},
    .modifiers = NULL,
};

ValueElement *newExpressionValue(ExpressionElement *value,
                                 location_t location) {
  printf("Creating new expression value\n");

  ValueElement *element = malloc(sizeof(ValueElement));
  element->type = v_expression;
  element->value.expression = value;
  element->location = location;
  element->free = freeValue;
  element->eval = evalValue;
  return element;
}

ValueElement *newIntegerValue(int value, location_t location) {
  printf("Creating new integer value: %d\n", value);

  ValueElement *element = malloc(sizeof(ValueElement));
  element->type = v_integer;
  element->value.integer = value;
  element->location = location;
  element->free = freeValue;
  element->eval = evalValue;
  return element;
}

ValueElement *newFloatValue(float value, location_t location) {
  printf("Creating new float value: %f\n", value);

  ValueElement *element = malloc(sizeof(ValueElement));
  element->type = v_floating;
  element->value.floating = value;
  element->location = location;
  element->free = freeValue;
  element->eval = evalValue;
  return element;
}

ValueElement *newBooleanValue(bool value, location_t location) {
  printf("Creating new boolean value: %s\n", value ? "true" : "false");

  ValueElement *element = malloc(sizeof(ValueElement));
  element->type = v_boolean;
  element->value.boolean = value;
  element->location = location;
  element->free = freeValue;
  element->eval = evalValue;
  return element;
}

ValueElement *newCharacterValue(char value, location_t location) {
  printf("Creating new character value: '%c'\n", value);

  ValueElement *element = malloc(sizeof(ValueElement));
  element->type = v_character;
  element->value.character = value;
  element->location = location;
  element->free = freeValue;
  element->eval = evalValue;
  return element;
}

ValueElement *newStringValue(char *value, location_t location) {
  printf("Creating new string value: \"%s\"\n", value);

  ValueElement *element = malloc(sizeof(ValueElement));
  element->type = v_string;
  element->value.string = strdup(value);
  element->location = location;
  element->free = freeValue;
  element->eval = evalValue;
  return element;
}

ValueElement *newFunctionCallValue(FunctionCallElement *value,
                                   location_t location) {
  printf("Creating new function call value\n");

  ValueElement *element = malloc(sizeof(ValueElement));
  element->type = v_funccall;
  element->value.function_call = value;
  element->location = location;
  element->free = freeValue;
  element->eval = evalValue;
  return element;
}

void freeValue(ValueElement *valueEl) {
  if (valueEl->type == v_expression)
    valueEl->value.expression->free(valueEl->value.expression);
  else if (valueEl->type == v_funccall)
    valueEl->value.function_call->free(valueEl->value.function_call);
  else if (valueEl->type == v_string)
    free(valueEl->value.string);

  free(valueEl);
}

static void freeValueData(ValueData *valueData) {
  free(valueData->generated_code);
  free_type(valueData->value_type);
  free(valueData);
}

static ValueData *evalValue(ValueElement *valueElement, EvalContext *context) {
  if (valueElement == NULL)
    return NULL;

  printf("Evaluating value\n");

  ValueData *valueData = malloc(sizeof(ValueData));
  Type *valueType;

  switch (valueElement->type) {
  case v_integer: {
    valueType = malloc(sizeof(Type));
    valueType->type_type = t_variable;
    TypeData int_td = {.variable_type = intTypeData};
    int_td.variable_type.modifiers =
        newTypeModifierList(); // We get segfault if we don't do this
    valueType->type_data = int_td;

    char code[21];
    snprintf(code, sizeof(code), "%d", valueElement->value.integer);
    valueData->generated_code = strdup(code);
    break;
  }
  case v_floating: {
    valueType = malloc(sizeof(Type));
    valueType->type_type = t_variable;
    TypeData float_td = {.variable_type = floatTypeData};
    float_td.variable_type.modifiers =
        newTypeModifierList(); // We get segfault if we don't do this
    valueType->type_data = float_td;

    char code[21];
    snprintf(code, sizeof(code), "%f", valueElement->value.floating);
    valueData->generated_code = strdup(code);
    break;
  }
  case v_boolean: {
    valueType = malloc(sizeof(Type));
    valueType->type_type = t_variable;
    TypeData bool_td = {.variable_type = boolTypeData};
    bool_td.variable_type.modifiers =
        newTypeModifierList(); // We get segfault if we don't do this
    valueType->type_data = bool_td;

    valueData->generated_code =
        strdup(valueElement->value.boolean ? "true" : "false");
    break;
  }
  case v_character: {
    valueType = malloc(sizeof(Type));
    valueType->type_type = t_variable;
    TypeData char_td = {.variable_type = charTypeData};
    char_td.variable_type.modifiers =
        newTypeModifierList(); // We get segfault if we don't do this
    valueType->type_data = char_td;

    char code[4];
    snprintf(code, sizeof(code), "'%c'", valueElement->value.character);
    valueData->generated_code = strdup(code);
    break;
  }
  case v_string: {
    valueType = malloc(sizeof(Type));
    valueType->type_type = t_variable;
    TypeData string_td = {.variable_type = charTypeData};
    string_td.variable_type.modifiers = newTypeModifierList();
    TypeModifier *string_td_modifier = malloc(sizeof(TypeModifier));
    string_td_modifier->modifier_type = tm_array;
    string_td_modifier->modifier_data.array_size =
        strlen(valueElement->value.string) + 1;
    push(string_td.variable_type.modifiers, string_td_modifier);
    valueType->type_data = string_td;

    valueData->generated_code = malloc(strlen(valueElement->value.string) + 3);
    snprintf(valueData->generated_code, strlen(valueElement->value.string) + 3,
             "\"%s\"", valueElement->value.string);
    break;
  }
  case v_expression: {
    ExpressionElement *expression = valueElement->value.expression;
    ExpressionData *expressionData = expression->eval(expression, context);
    valueType = copy_type(expressionData->type);
    // TODO
    // valueData->generated_code = strdup(expressionData->generated_code);
    expressionData->free(expressionData);
    break;
  }
  case v_funccall: {
    FunctionCallElement *functionCall = valueElement->value.function_call;
    FunctionCall *functionCallData = functionCall->eval(functionCall, context);

    if (functionCallData == NULL) {
      free(valueData);
      return NULL;
    }

    valueType = copy_type(functionCallData->returnType);
    // TODO
    // valueData->generated_code = strdup(functionCallData->generated_code);
    functionCallData->free(functionCallData);
    break;
  }
  }

  valueData->value_type = valueType;
  valueData->free = freeValueData;
  return valueData;
}
