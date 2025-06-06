#include "ir/ast/structfield.h"
#include "ir/eval/type.h"
#if !defined(STRUCT_H__)
#define STRUCT_H__

#include "ir/eval/context.h"
#include "ir/eval/type.h"

typedef struct structData {
  Type *type;           // Type of the struct
  char *generated_code; // Generated code for the struct definition

  void (*free)(struct structData *); // Function to free the struct data
} StructData;

typedef struct structElement {
  char *name;
  StructFieldList *fields; // List of fields in the struct
  location_t location;

  void (*free)(struct structElement *); // Function to free the struct element
  StructData *(*eval)(struct structElement *, EvalContext *);
} StructElement;

StructElement *newStructDefinition(char *name, StructFieldList *fields,
                                   location_t location);

#endif // !STRUCT_H__
