#if !defined(VALUE_H__)
#define VALUE_H__

#include "ir/ast/expression.h"
#include "ir/ast/funccall.h"
#include "ir/ast/struct_constructor.h"
#include "ir/ast/variable.h"
#include "ir/eval/context.h"
#include "ir/eval/type.h"
#include "utils/error_utils.h"
#include <stdbool.h>

typedef enum {
  v_integer,
  v_floating,
  v_boolean,
  v_character,
  v_string,
  v_expression,
  v_funccall,
  v_variable,
  v_struct_constructor,
} ValueType;

typedef union {
  int integer;
  float floating;
  bool boolean;
  char character;
  char *string;
  ExpressionElement *expression;
  FunctionCallElement *function_call;
  VariableElement *variable;
  StructConstructorElement *struct_constructor;
} Value;

typedef struct valueData {
  Type *value_type;
  char *generated_code;

  void (*free)(struct valueData *);
} ValueData;

typedef struct valueElement {
  ValueType type;
  Value value;
  location_t location;

  void (*free)(struct valueElement *);
  ValueData *(*eval)(struct valueElement *, EvalContext *);
} ValueElement;

ValueElement *newExpressionValue(ExpressionElement *value, location_t location);
ValueElement *newIntegerValue(int value, location_t location);
ValueElement *newFloatValue(float value, location_t location);
ValueElement *newBooleanValue(bool value, location_t location);
ValueElement *newCharacterValue(char value, location_t location);
ValueElement *newStringValue(char *value, location_t location);
ValueElement *newFunctionCallValue(FunctionCallElement *value,
                                   location_t location);
ValueElement *newVariableValue(VariableElement *value, location_t location);
ValueElement *newStructConstructorValue(StructConstructorElement *value,
                                        location_t location);

// freeValue isn't static because it is used for the argument list in funccall.c
void freeValue(ValueElement *value);

#endif // VALUE_H__
