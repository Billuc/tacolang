#include "ir/ast/struct_constructor.h"
#include "ir/eval/context.h"
#include "ir/eval/symbol_table.h"
#include "ir/eval/type.h"
#include "ir/eval/type_modifier.h"
#include "utils/error_utils.h"
#include "utils/list_utils.h"
#include "utils/str_utils.h"
#include <string.h>

// Declare the free and eval functions for StructConstructorElement

static void freeStructConstructorElement(StructConstructorElement *self);
static StructConstructorData *
evalStructConstructorElement(StructConstructorElement *self,
                             EvalContext *context);

// Implement the newStructConstructorElement function

StructConstructorElement *newStructConstructor(const char *structName,
                                               StructFieldValueList *values,
                                               location_t location) {
  StructConstructorElement *element = malloc(sizeof(StructConstructorElement));
  if (!element) {
    return NULL; // Handle memory allocation failure
  }

  element->structName = strdup(structName);
  element->values = values;
  element->location = location;

  element->free = freeStructConstructorElement;
  element->eval = evalStructConstructorElement;

  return element;
}

// Implement the free function for StructConstructorElement

static void freeStructConstructorElement(StructConstructorElement *self) {
  if (!self)
    return;
  free(self->structName);
  if (self->values) {
    freeLinkedList(self->values);
  }
  free(self);
}

// Implement the free function for StructConstructorData

static void freeStructConstructorData(StructConstructorData *self) {
  if (!self)
    return;
  if (self->type) {
    free_type(self->type);
  }
  free(self);
}

// Implement the eval function for StructConstructorElement

static StructConstructorData *
evalStructConstructorElement(StructConstructorElement *self,
                             EvalContext *context) {
  if (!self || !context) {
    return NULL; // Handle null pointers
  }

  // Create a new StructConstructorData instance
  StructConstructorData *data = malloc(sizeof(StructConstructorData));
  if (!data) {
    return NULL; // Handle memory allocation failure
  }

  // Verify that the structure is declared in the context
  SymbolData *structTypeData = findSymbol(context, self->structName);
  if (!structTypeData) {
    // Handle error: struct not found
    print_error(self->location, "Struct type '%s' not found in context.",
                self->structName);
    free(data);
    return NULL;
  }
  if (structTypeData->type->type_type != t_structure) {
    // Handle error: not a struct type
    print_error(self->location, "Symbol '%s' is not a struct type.",
                self->structName);
    free(data);
    return NULL;
  }

  data->generated_code = strdup("{ ");
  StructType *structType = &structTypeData->type->type_data.struct_type;

  int iField = 0;
  LinkedListElement *iter = self->values->head;
  for (; iField < structType->number_of_fields && iter != NULL;
       iField++, iter = iter->next) {
    StructFieldValueElement *fieldValue = (StructFieldValueElement *)iter->data;
    StructFieldValueData *fieldValueData =
        fieldValue->eval(fieldValue, context);

    // Check if the field name matches
    if (strcmp(fieldValue->name, structType->fields[iField].field_name) != 0) {
      print_error(fieldValue->location,
                  "Field '%s' does not match expected field '%s'.",
                  fieldValue->name, structType->fields[iField].field_name);
      fieldValueData->free(fieldValueData);
      free(data->generated_code);
      free(data);
      return NULL;
    }

    // Evaluate the field value
    SimpleType expectedFieldType = structType->fields[iField].field_type;
    SimpleType actualFieldType = fieldValueData->type->type_data.variable_type;
    if (compare_simpleType(expectedFieldType, actualFieldType) != 0) {
      char *expected = print_simpleType(expectedFieldType);
      char *actual = print_simpleType(actualFieldType);
      print_error(fieldValue->location,
                  "Field '%s' has type '%s', expected '%s'.",
                  structType->fields[iField].field_name, actual, expected);
      fieldValueData->free(fieldValueData);
      free(expected);
      free(actual);
      free(data->generated_code);
      free(data);
      return NULL;
    }

    strappend(&data->generated_code, fieldValueData->generated_code);
    if (iField < structType->number_of_fields - 1) {
      strappend(&data->generated_code, ", ");
    }
    fieldValueData->free(fieldValueData);
  }
  strappend(&data->generated_code, " }");

  data->type = malloc(sizeof(Type));
  data->type->type_type = t_variable;
  data->type->type_data.variable_type.is_base_type = false;
  data->type->type_data.variable_type.modifiers = newTypeModifierList();
  data->type->type_data.variable_type.type_data.custom_type =
      strdup(self->structName);

  data->free = freeStructConstructorData;

  return data;
}
