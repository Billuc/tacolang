#include <string.h>
#include "type_modifier.h"

bool type_modifier_equals(TypeModifier *modifier1, TypeModifier *modifier2)
{
    return modifier1->modifier_type == modifier2->modifier_type;
}

char *print_type_modifier(TypeModifier *modifier)
{
    char buf[10] = "";

    switch (modifier->modifier_type)
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