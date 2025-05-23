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
  }

  return data;
}

ModifierList *newModifierList() { return newLinkedList(freeModifier); }
