#if !defined(MODIFIER_H__)
#define MODIFIER_H__

#include "../symbol_table/symbol_table.h"

typedef enum
{
    mutable,
    reference,
    optional,
} Modifier;

typedef struct
{
    Modifier type;
} ModifierElement;

typedef struct modifierLink
{
    ModifierElement *data;
    struct modifierLink *next;
} ModifierLink;

ModifierElement *newModifier(char *type);
void freeModifier(ModifierElement *modifierEl);
void evalModifier(ModifierElement *modifierElement, SymbolElement **symbolTable);

ModifierLink *addModifier(ModifierLink *list, ModifierElement *newElement);
void freeModifierList(ModifierLink *list);
void evalModifierList(ModifierLink *list);

#endif // MODIFIER_H__
