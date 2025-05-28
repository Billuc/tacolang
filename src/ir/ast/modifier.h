#if !defined(MODIFIER_H__)
#define MODIFIER_H__

#include "ir/eval/context.h"
#include "ir/eval/type_modifier.h"
#include "utils/error_utils.h"
#include "utils/list_utils.h"

typedef enum {
  m_mutable,
  m_reference,
  m_optional,
  m_array,
} Modifier;

typedef struct modifierData {
  TypeModifier *typeModifier;
  char *generated_code;

  void (*free)(struct modifierData *);
} ModifierData;

typedef struct modifierElement {
  Modifier type;
  location_t location;
  int array_size; // Only used for array modifiers

  void (*free)(struct modifierElement *);
  ModifierData *(*eval)(struct modifierElement *, EvalContext *);
} ModifierElement;

typedef LinkedList ModifierList;

ModifierElement *newModifier(char *type, location_t location);
ModifierElement *newArrayModifier(int size, location_t location);
ModifierList *newModifierList();

#endif // MODIFIER_H__
