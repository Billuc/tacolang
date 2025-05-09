#if !defined(TYPE_MODIFIER_H__)
#define TYPE_MODIFIER_H__

#include <stdbool.h>

typedef enum
{
    tm_mutable,
    tm_reference,
    tm_optional,
} TypeModifierType;

typedef union
{

} TypeModifierData;

typedef struct
{
    TypeModifierType modifier_type;
    TypeModifierData modifier_data;
} TypeModifier;

typedef struct typeModifierLink
{
    TypeModifier element;
    struct typeModifierLink *next;
} TypeModifierLink;

bool type_modifier_equals(TypeModifier modifier1, TypeModifier modifier2);
char *print_type_modifier(TypeModifier modifier);

#endif // TYPE_MODIFIER_H__
