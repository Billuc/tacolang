#include "definition.h"
#include "utils/str_utils.h"
#include <stdio.h>
#include <stdlib.h>

static void freeDefinition(DefinitionElement *def);
static DefinitionData *evalDefinition(DefinitionElement *def, EvalContext *ctx);

DefinitionElement *newFuncdefDefinition(FunctionDefinitionElement *funcdef,
                                        location_t location) {
  printf("Creating new function definition\n");

  DefinitionElement *def = malloc(sizeof(DefinitionElement));
  def->type = def_funcdef;
  def->definition.funcdef = funcdef;
  def->location = location;
  def->free = freeDefinition;
  def->eval = evalDefinition;
  return def;
}

static void freeDefinition(DefinitionElement *def) {
  if (def->type == def_funcdef) {
    def->definition.funcdef->free(def->definition.funcdef);
  }
  free(def);
}

static void freeDefinitionData(DefinitionData *data) {
  free(data->generated_code);
  free(data);
}

static DefinitionData *evalDefinition(DefinitionElement *def,
                                      EvalContext *ctx) {
  if (def == NULL)
    return NULL;

  printf("Evaluating definition\n");

  DefinitionData *data = malloc(sizeof(DefinitionData));
  data->free = freeDefinitionData;
  data->generated_code = strdup("");

  if (def->type == def_funcdef) {
    FunctionDefinitionData *funcdef_data =
        def->definition.funcdef->eval(def->definition.funcdef, ctx);
    strappend(&data->generated_code, funcdef_data->generated_code);
    funcdef_data->free(funcdef_data);
  }

  return data;
}

DefinitionList *newDefinitionList() { return newLinkedList(freeDefinition); }
