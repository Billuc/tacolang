#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "type_modifier.h"
#include "utils/str_utils.h"

int compare_typeModifier(TypeModifier *modifier1, TypeModifier *modifier2)
{
    int modifier_cmp = modifier1->modifier_type - modifier2->modifier_type;
    if (modifier_cmp != 0)
    {
        return modifier_cmp;
    }

    switch (modifier1->modifier_type)
    {
    case tm_array:
        return modifier1->modifier_data.array_size - modifier2->modifier_data.array_size;
    default:
        return 0;
    }
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
    case tm_array:
        sprintf(buf, "arr[%d]", modifier.modifier_data.array_size);
        break;
    case tm_list:
        strcpy(buf, "list");
        break;
    }

    return strdup(buf);
}

TypeModifierList *newTypeModifierList()
{
    return newLinkedList(NULL);
}

TypeModifier *copy_typeModifier(TypeModifier *toCopy)
{
    TypeModifier *copy = malloc(sizeof(TypeModifier));
    copy->modifier_type = toCopy->modifier_type;
    switch (toCopy->modifier_type)
    {
    case tm_array:
        copy->modifier_data.array_size = toCopy->modifier_data.array_size;
        break;
    default:
        break;
    }
    return copy;
}