#if !defined(STRUCT_CONSTRUCTOR_H__)
#define STRUCT_CONSTRUCTOR_H__

#include "ir/ast/struct_field_value.h"
#include "ir/eval/context.h"
#include "ir/eval/type.h"
#include "utils/error_utils.h"

typedef struct structConstructorData {
  Type *type;           // Type of the value being constructed
  char *generated_code; // Generated code for the struct constructor

  void (*free)(struct structConstructorData
                   *self); // Function to free the structConstructorData
} StructConstructorData;

typedef struct structConstructorElement {
  char *structName;             // Name of the struct being constructed
  StructFieldValueList *values; // List of field values
  location_t location;          // Location in the source code

  void (*free)(struct structConstructorElement
                   *self); // Function to free the structConstructor
  StructConstructorData *(*eval)(
      struct structConstructorElement *self,
      EvalContext *context); // Function to evaluate the struct constructor
} StructConstructorElement;

StructConstructorElement *newStructConstructor(const char *structName,
                                               StructFieldValueList *values,
                                               location_t location);

#endif // STRUCT_CONSTRUCTOR_H__
