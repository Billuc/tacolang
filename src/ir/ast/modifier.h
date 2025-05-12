#if !defined(MODIFIER_H__)
#define MODIFIER_H__

#include "common.h"
#include "../eval/type_modifier.h"
#include "../symbol_table/symbol_table.h"

typedef enum
{
    mutable,
    reference,
    optional,
} Modifier;

typedef struct
{
    TypeModifier typeModifier;
} EvalModifierData;

typedef struct modifierElement
{
    Modifier type;

    FREE_FUNC(struct modifierElement, free);
    EVAL_FUNC(struct modifierElement, EvalModifierData *, eval);
} ModifierElement;

typedef struct
{
    TypeModifierLink *typeModifierList;
} EvalModifierLinkData;

typedef struct modifierLink
{
    ModifierElement *data;
    struct modifierLink *next;
} ModifierLink;

ModifierElement *newModifier(char *type);
// void freeModifier(ModifierElement *modifierEl);
// EvalModifierData *evalModifier(ModifierElement *modifierElement, SymbolElement **symbolTable);

ModifierLink *addModifier(ModifierLink *list, ModifierElement *newElement);
void freeModifierList(ModifierLink *list);
EvalModifierLinkData *evalModifierList(ModifierLink *list, SymbolElement **symbolTable);

#endif // MODIFIER_H__
