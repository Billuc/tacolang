#include <string.h>
#include "type_modifier.h"
#include "utils/str_utils.h"

int compare_typeModifier(TypeModifier *modifier1, TypeModifier *modifier2)
{
    return modifier1->modifier_type - modifier2->modifier_type;
}

char *print_typeModifier(TypeModifier modifier)
{
    char buf[10] = "";

    switch (modifier.modifier_type)
    {
    case tm_mutable:
        strcpy(buf, "mut");
        break;
    case tm_optional:
        strcpy(buf, "opt");
        break;
    case tm_reference:
        strcpy(buf, "ref");
        break;
    }

    return strdup(buf);
}

TypeModifierList *newTypeModifierList()
{
    return newLinkedList(NULL);
}