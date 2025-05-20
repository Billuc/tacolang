#include "definition.h"
#include <stdlib.h>

static void freeDefinition(DefinitionElement *def);
static void evalDefinition(DefinitionElement *def, EvalContext *ctx);

DefinitionElement *newFuncdefDefinition(FunctionDefinitionElement *funcdef, location_t location)
{
    DefinitionElement *def = malloc(sizeof(DefinitionElement));
    def->type = def_funcdef;
    def->definition.funcdef = funcdef;
    def->location = location;
    def->free = freeDefinition;
    def->eval = evalDefinition;
    return def;
}

static void freeDefinition(DefinitionElement *def)
{
    if (def->type == def_funcdef)
    {
        def->definition.funcdef->free(def->definition.funcdef);
    }
    free(def);
}

static void evalDefinition(DefinitionElement *def, EvalContext *ctx)
{
    if (def->type == def_funcdef)
    {
        def->definition.funcdef->eval(def->definition.funcdef, ctx);
    }
}

DefinitionList *newDefinitionList()
{
    return newLinkedList(freeDefinition);
}