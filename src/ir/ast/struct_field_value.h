#if !defined(STRUCT_FIELD_VALUE_H__)
#define STRUCT_FIELD_VALUE_H__

#include "ir/eval/context.h"
#include "ir/eval/type.h"
#include "utils/error_utils.h"
#include "utils/list_utils.h"

typedef struct valueElement ValueElement;

typedef struct structFieldValueData {
  char *name; // Name of the field
  Type *type; // Type of the field
  char *generated_code;

  void (*free)(struct structFieldValueData *self);
} StructFieldValueData;

typedef struct structFieldValueElement {
  char *name;          // Name of the field
  ValueElement *value; // Value of the field
  location_t location; // Location of the field in the source code

  void (*free)(struct structFieldValueElement *self);
  StructFieldValueData *(*eval)(struct structFieldValueElement *self,
                                EvalContext *context);
} StructFieldValueElement;

typedef LinkedList StructFieldValueList;

StructFieldValueElement *
newStructFieldValue(const char *name, ValueElement *value, location_t location);
StructFieldValueList *newStructFieldValueList(void);

#endif // STRUCT_FIELD_VALUE_H__
