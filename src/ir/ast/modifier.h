#if !defined(MODIFIER_H__)
#define MODIFIER_H__

#include "utils/list_utils.h"
#include "ir/eval/type_modifier.h"
#include "ir/eval/context.h"

typedef enum
{
    m_mutable,
    m_reference,
    m_optional,
} Modifier;

typedef struct modifierData
{
    TypeModifier *typeModifier;

    void (*free)(struct modifierData *);
} ModifierData;

typedef struct modifierElement
{
    Modifier type;

    void (*free)(struct modifierElement *);
    ModifierData *(*eval)(struct modifierElement *, EvalContext *);
} ModifierElement;

typedef LinkedList ModifierList;

ModifierElement *newModifier(char *type);
ModifierList *newModifierList();

#endif // MODIFIER_H__
