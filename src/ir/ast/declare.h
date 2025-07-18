#if !defined(DECLARE_H__)
#define DECLARE_H__

#include "./typedef.h"
#include "./value.h"
#include "ir/eval/context.h"

typedef struct declareData {
  char *symbol_name;
  Type *symbol_type;
  char *generated_code; // Generated code for the declaration

  void (*free)(struct declareData *);
} DeclareData;

typedef struct declareElement {
  char *name;
  TypedefElement *type;
  ValueElement *value;
  location_t location;

  void (*free)(struct declareElement *);
  DeclareData *(*eval)(struct declareElement *, EvalContext *);
} DeclareElement;

DeclareElement *newDeclare(char *name, TypedefElement *type,
                           ValueElement *value, location_t location);

#endif // DECLARE_H__
