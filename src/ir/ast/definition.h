#if !defined(DEFINITION_H__)
#define DEFINITION_H__

#include "./funcdef.h"
#include "ir/eval/context.h"
#include "utils/list_utils.h"
#include "utils/error_utils.h"

typedef enum
{
    def_funcdef,
} DefinitionType;

typedef union
{
    FunctionDefinitionElement *funcdef;
} Definition;

typedef struct definitionElement
{
    DefinitionType type;
    Definition definition;
    location_t location;

    void (*free)(struct definitionElement *);
    void (*eval)(struct definitionElement *, EvalContext *);
} DefinitionElement;

typedef LinkedList DefinitionList;

DefinitionElement *newFuncdefDefinition(FunctionDefinitionElement *funcdef, location_t location);
DefinitionList *newDefinitionList();

#endif // DEFINITION_H__
