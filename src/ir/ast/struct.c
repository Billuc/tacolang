#include "ir/ast/struct.h"
#include "ir/eval/type.h"
#include "utils/error_utils.h"
#include "utils/list_utils.h"
#include "utils/str_utils.h"
#include <string.h>

// Declare the free and eval functions for StructElement

static void freeStructElement(StructElement *struct_elem);
static StructData *evalStructElement(StructElement *struct_elem,
                                     EvalContext *context);

// Implement the newStructDefinition function

StructElement *newStructDefinition(char *name, StructFieldList *fields,
                                   location_t location) {
  StructElement *struct_elem = malloc(sizeof(StructElement));
  if (!struct_elem) {
    return NULL; // Handle memory allocation failure
  }

  struct_elem->name = strdup(name);
  struct_elem->fields = fields;
  struct_elem->location = location;

  struct_elem->free = freeStructElement;
  struct_elem->eval = evalStructElement;

  return struct_elem;
}

// Implement the free function for StructElement

static void freeStructElement(StructElement *struct_elem) {
  if (!struct_elem)
    return;

  if (struct_elem->name) {
    free(struct_elem->name);
  }
  if (struct_elem->fields) {
    freeLinkedList(struct_elem->fields);
  }
  free(struct_elem);
}

// Implement the free function for StructData

static void freeStructData(StructData *data) {
  if (!data)
    return;

  if (data->type) {
    free_type(data->type);
  }
  if (data->generated_code) {
    free(data->generated_code);
  }
  free(data);
}

// Implement the eval function for StructElement

static StructData *evalStructElement(StructElement *struct_elem,
                                     EvalContext *context) {
  if (!struct_elem || !context) {
    return NULL; // Handle null pointers
  }

  StructData *data = malloc(sizeof(StructData));
  if (!data) {
    return NULL; // Handle memory allocation failure
  }

  data->generated_code = strdup("struct ");
  strappend(data->generated_code, struct_elem->name);
  strappend(data->generated_code, " {\n");

  data->type->type_type = t_structure;
  StructType *struct_type = &data->type->type_data.struct_type;
  struct_type->struct_name = strdup(struct_elem->name);
  struct_type->number_of_fields = 0;

  LinkedListElement *iter;
  for (iter = struct_elem->fields->head; iter != NULL; iter = iter->next) {
    StructFieldElement *field_elem = (StructFieldElement *)iter->data;
    StructFieldData *field_data = field_elem->eval(field_elem, context);

    if (!field_data)
      continue; // If an error occurred, skip this field
    if (field_data->type->type_type != t_variable) {
      print_error(field_elem->location,
                  "Struct field type must be a variable type.");
      field_data->free(field_data);
      continue; // Skip this field if the type is not a variable type
    }

    struct_type->number_of_fields++;
    struct_type->fields[struct_type->number_of_fields - 1].field_name =
        strdup(field_data->name);
    struct_type->fields[struct_type->number_of_fields - 1].field_type =
        field_data->type->type_data.variable_type;

    strappend(data->generated_code, field_data->generated_code);
    if (iter->next != NULL) {
      strappend(data->generated_code, ",\n");
    } else {
      strappend(data->generated_code, "\n");
    }
  }

  strappend(data->generated_code, "};\n");

  data->free = freeStructData;

  return data;
}
