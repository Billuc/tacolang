#if !defined(DEFINITION_H__)
#define DEFINITION_H__

#include "./funcdef.h"
#include "ir/ast/struct.h"
#include "ir/eval/context.h"
#include "utils/error_utils.h"
#include "utils/list_utils.h"

typedef enum {
  def_funcdef,
  def_structdef,
} DefinitionType;

typedef union {
  FunctionDefinitionElement *funcdef;
  StructElement *structdef;
} Definition;

typedef struct definitionData {
  char *generated_code; // Generated code for the definition

  void (*free)(struct definitionData *);
} DefinitionData;

typedef struct definitionElement {
  DefinitionType type;
  Definition definition;
  location_t location;

  void (*free)(struct definitionElement *);
  DefinitionData *(*eval)(struct definitionElement *, EvalContext *);
} DefinitionElement;

typedef LinkedList DefinitionList;

DefinitionElement *newFuncdefDefinition(FunctionDefinitionElement *funcdef,
                                        location_t location);
DefinitionElement *newStructdefDefinition(StructElement *structdef,
                                          location_t location);
DefinitionList *newDefinitionList();

#endif // DEFINITION_H__
