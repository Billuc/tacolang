#include "ir/ast/structfield.h"
#include "ir/ast/typedef.h"
#include "utils/error_utils.h"
#include "utils/list_utils.h"
#include "utils/str_utils.h"

static void freeStructFieldElement(StructFieldElement *field);
static StructFieldData *evalStructFieldElement(StructFieldElement *field,
                                               EvalContext *context);

StructFieldElement *newStructField(char *name, TypedefElement *type,
                                   location_t location) {
  StructFieldElement *field = malloc(sizeof(StructFieldElement));
  if (!field) {
    return NULL; // Handle memory allocation failure
  }

  field->name = strdup(name);
  field->type = type;
  field->location = location;

  field->free = freeStructFieldElement;
  field->eval = evalStructFieldElement;

  return field;
}

static void freeStructFieldElement(StructFieldElement *field) {
  if (!field)
    return;

  if (field->name) {
    free(field->name);
  }
  if (field->type) {
    field->type->free(field->type);
  }
  free(field);
}

static void freeStructFieldData(StructFieldData *data) {
  if (!data)
    return;

  if (data->name) {
    free(data->name);
  }
  if (data->type) {
    free_type(data->type);
  }
  if (data->generated_code) {
    free(data->generated_code);
  }
  free(data);
}

static StructFieldData *evalStructFieldElement(StructFieldElement *field,
                                               EvalContext *context) {
  if (!field || !context) {
    return NULL; // Handle null pointers
  }

  StructFieldData *data = malloc(sizeof(StructFieldData));
  if (!data) {
    return NULL; // Handle memory allocation failure
  }

  data->name = field->name;

  TypedefData *typeData = field->type->eval(field->type, context);
  if (!typeData) {
    free(data);
    return NULL; // Handle evaluation failure
  }

  data->type = typeData->type;

  data->generated_code = strdup("");
  strappend(&data->generated_code, typeData->generated_code);
  strappend(&data->generated_code, " ");
  strappend(&data->generated_code, field->name);

  // Set the free function for the struct field data
  data->free = freeStructFieldData;

  free(typeData->generated_code);

  return data;
}

StructFieldList *newStructFieldList(void) {
  StructFieldList *list = newLinkedList(freeStructFieldElement);
  return list;
}
