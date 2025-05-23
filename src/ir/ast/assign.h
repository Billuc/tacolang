#if !defined(ASSIGN_H__)
#define ASSIGN_H__

#include "./value.h"
#include "./variable.h"
#include "ir/eval/context.h"
#include "utils/error_utils.h"

typedef struct assign {
  char *generated_code;

  void (*free)(struct assign *);
} Assign;

typedef struct assignElement {
  VariableElement *left;
  ValueElement *right;
  location_t location;

  void (*free)(struct assignElement *);
  Assign *(*eval)(struct assignElement *, EvalContext *);
} AssignElement;

AssignElement *newAssign(VariableElement *left, ValueElement *right,
                         location_t loc);

#endif // ASSIGN_H__
