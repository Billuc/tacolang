#if !defined(MODIFIER_H__)
#define MODIFIER_H__

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
    Modifier type;
} ModifierElement;

typedef struct modifierLink
{
    ModifierElement *data;
    struct modifierLink *next;
} ModifierLink;

typedef struct
{
    TypeModifier typeModifier;
} EvalModifierData;

typedef struct
{
    TypeModifierLink *typeModifierList;
} EvalModifierLinkData;

ModifierElement *newModifier(char *type);
void freeModifier(ModifierElement *modifierEl);
EvalModifierData *evalModifier(ModifierElement *modifierElement, SymbolElement **symbolTable);

ModifierLink *addModifier(ModifierLink *list, ModifierElement *newElement);
void freeModifierList(ModifierLink *list);
EvalModifierLinkData *evalModifierList(ModifierLink *list, SymbolElement **symbolTable);

#endif // MODIFIER_H__
