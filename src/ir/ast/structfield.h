#if !defined(STRUCTFIELD_H__)
#define STRUCTFIELD_H__

#include "ir/ast/typedef.h"
#include "ir/eval/context.h"
#include "ir/eval/type.h"

typedef struct structFieldData {
  char *name;           // Name of the field
  Type *type;           // Type of the field
  char *generated_code; // Generated code for the field definition

  void (*free)(
      struct structFieldData *); // Function to free the struct field data
} StructFieldData;

typedef struct structFieldElement {
  char *name;           // Name of the field
  TypedefElement *type; // Type of the field
  location_t location;  // Location in the source code

  void (*free)(
      struct structFieldElement *); // Function to free the struct field element
  StructFieldData *(*eval)(
      struct structFieldElement *,
      EvalContext *); // Function to evaluate the struct field element
} StructFieldElement;

typedef LinkedList StructFieldList;

StructFieldElement *newStructField(char *name, TypedefElement *type,
                                   location_t location);
StructFieldList *newStructFieldList(void);

#endif // STRUCTFIELD_H__
