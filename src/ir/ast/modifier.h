#if !defined(MODIFIER_H__)
#define MODIFIER_H__

#include "utils/common.h"
#include "utils/list_utils.h"
#include "ir/eval/type_modifier.h"
#include "ir/eval/symbol_table.h"

typedef enum
{
    m_mutable,
    m_reference,
    m_optional,
} Modifier;

typedef struct modifierData
{
    TypeModifier *typeModifier;

    FREE_FUNC(struct modifierData, free);
} ModifierData;

typedef struct modifierElement
{
    Modifier type;

    FREE_FUNC(struct modifierElement, free);
    EVAL_FUNC(struct modifierElement, ModifierData *, eval);
} ModifierElement;

typedef LinkedList ModifierList;

ModifierElement *newModifier(char *type);
ModifierList *newModifierList();

#endif // MODIFIER_H__
