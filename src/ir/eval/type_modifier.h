#if !defined(TYPE_MODIFIER_H__)
#define TYPE_MODIFIER_H__

#include "utils/list_utils.h"

typedef enum
{
    tm_mutable,
    tm_reference,
    tm_optional,
    tm_array,
    tm_list,
} TypeModifierType;

typedef union
{
    int array_size;
} TypeModifierData;

typedef struct typeModifier
{
    TypeModifierType modifier_type;
    TypeModifierData modifier_data;
} TypeModifier;

typedef LinkedList TypeModifierList;

int compare_typeModifier(TypeModifier *modifier1, TypeModifier *modifier2);
char *print_typeModifier(TypeModifier modifier);
TypeModifierList *newTypeModifierList();
TypeModifier *copy_typeModifier(TypeModifier *toCopy);

#endif // TYPE_MODIFIER_H__
