#if !defined(DEFINITION_H__)
#define DEFINITION_H__

#include "./funcdef.h"
#include "ir/eval/context.h"
#include "utils/list_utils.h"

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

    void (*free)(struct definitionElement *);
    void (*eval)(struct definitionElement *, EvalContext *);
} DefinitionElement;

typedef LinkedList DefinitionList;

DefinitionElement *newFuncdefDefinition(FunctionDefinitionElement *funcdef);
DefinitionList *newDefinitionList();

#endif // DEFINITION_H__
