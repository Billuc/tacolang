#include "ir/ast/struct_field_value.h"
#include "ir/ast/value.h"
#include "ir/eval/type.h"
#include "utils/list_utils.h"
#include "utils/str_utils.h"

// Declare the free and eval functions for StructFieldValueElement

static void freeStructFieldValueElement(StructFieldValueElement *self);
static StructFieldValueData *
evalStructFieldValueElement(StructFieldValueElement *self,
                            EvalContext *context);

// Implement the newStructFieldValue function

StructFieldValueElement *newStructFieldValue(const char *name,
                                             ValueElement *value,
                                             location_t location) {
  StructFieldValueElement *element = malloc(sizeof(StructFieldValueElement));
  if (!element) {
    return NULL; // Handle memory allocation failure
  }

  element->name = strdup(name);
  element->value = value;
  element->location = location;

  element->free = freeStructFieldValueElement;
  element->eval = evalStructFieldValueElement;

  return element;
}

// Implement the free function for StructFieldValueElement

static void freeStructFieldValueElement(StructFieldValueElement *self) {
  if (!self)
    return;

  free(self->name);
  if (self->value) {
    self->value->free(self->value);
  }
  free(self);
}

// Implement the free function for StructFieldValueData

static void freeStructFieldValueData(StructFieldValueData *self) {
  if (!self)
    return;

  free(self->name);
  if (self->type) {
    free_type(self->type);
  }
  if (self->generated_code) {
    free(self->generated_code);
  }
  free(self);
}

// Implement the eval function for StructFieldValueElement

StructFieldValueData *evalStructFieldValueElement(StructFieldValueElement *self,
                                                  EvalContext *context) {
  if (!self || !context) {
    return NULL; // Handle null pointers
  }

  StructFieldValueData *data = malloc(sizeof(StructFieldValueData));
  if (!data) {
    return NULL; // Handle memory allocation failure
  }

  data->name = strdup(self->name);

  ValueData *value_data = self->value->eval(self->value, context);
  if (!value_data) {
    free(data);
    return NULL; // Handle evaluation failure
  }
  data->type = copy_type(value_data->value_type);

  data->generated_code = strdup(".");
  strappend(&data->generated_code, self->name);
  strappend(&data->generated_code, " = ");
  strappend(&data->generated_code, value_data->generated_code);

  data->free = freeStructFieldValueData;

  return data;
}

StructFieldValueList *newStructFieldValueList(void) {
  return newLinkedList(freeStructFieldValueElement);
}
