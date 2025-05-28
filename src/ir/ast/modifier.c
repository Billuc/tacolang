#include "modifier.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define M_MUTABLE "mut"
#define M_REFERENCE "ref"
#define M_OPTIONAL "opt"

static void freeModifier(ModifierElement *element);
static ModifierData *evalModifier(ModifierElement *element,
                                  EvalContext *context);

ModifierElement *newModifier(char *type, location_t location) {
  printf("Creating new modifier: %s\n", type);

  ModifierElement *element = malloc(sizeof(ModifierElement));
  element->free = freeModifier;
  element->eval = evalModifier;
  element->location = location;
  element->array_size = 0; // Default to 0 for non-array modifiers

  if (!strcmp(type, M_MUTABLE))
    element->type = m_mutable;
  else if (!strcmp(type, M_REFERENCE))
    element->type = m_reference;
  else if (!strcmp(type, M_OPTIONAL))
    element->type = m_optional;
  else {
    print_error(location, "Unknown modifier: %s", type);
    free(element);
    return NULL;
  }

  return element;
}

ModifierElement *newArrayModifier(int size, location_t location) {
  printf("Creating new array modifier: arr[%d]\n", size);

  ModifierElement *element = malloc(sizeof(ModifierElement));
  element->free = freeModifier;
  element->eval = evalModifier;
  element->location = location;
  element->array_size = size;
  element->type = m_array;

  return element;
}

static void freeModifier(ModifierElement *element) {
  if (element == NULL)
    return;

  free(element);
}

static void freeModifierData(ModifierData *data) {
  if (data == NULL)
    return;

  if (data->typeModifier != NULL)
    free(data->typeModifier);

  free(data->generated_code);
  free(data);
}

static ModifierData *evalModifier(ModifierElement *element,
                                  EvalContext *context) {
  if (element == NULL)
    return NULL;

  printf("Evaluating modifier\n");

  ModifierData *data = malloc(sizeof(ModifierData));
  data->free = freeModifierData;
  data->typeModifier = malloc(sizeof(TypeModifier));

  switch (element->type) {
  case m_mutable:
    data->typeModifier->modifier_type = tm_mutable;
    data->generated_code = strdup("");
    break;
  case m_reference:
    data->typeModifier->modifier_type = tm_reference;
    data->generated_code = strdup("*");
    break;
  case m_optional:
    data->typeModifier->modifier_type = tm_optional;
    data->generated_code = strdup("");
    break;
  case m_array:
    data->typeModifier->modifier_type = tm_array;
    data->typeModifier->modifier_data.array_size = element->array_size;
    data->generated_code = malloc(20);

    if (element->array_size >= 0)
      snprintf(data->generated_code, 20, "[%d]", element->array_size);
    else
      snprintf(data->generated_code, 20, "*");

    break;
  }

  return data;
}

ModifierList *newModifierList() { return newLinkedList(freeModifier); }
