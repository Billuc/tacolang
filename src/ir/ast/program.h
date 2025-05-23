#if !defined(PROGRAM_H__)
#define PROGRAM_H__

#include "ir/ast/definition.h"
#include "ir/eval/context.h"
#include "utils/error_utils.h"

typedef struct programData {
  char *generated_code;
  void (*free)(struct programData *);
} ProgramData;

typedef struct programElement {
  DefinitionList *definitions;
  location_t location;

  void (*free)(struct programElement *);
  ProgramData *(*eval)(struct programElement *, EvalContext *);
} ProgramElement;

ProgramElement *newProgram(DefinitionList *statements, location_t location);
void eval(ProgramElement *program);

#endif // PROGRAM_H__
