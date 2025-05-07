#if !defined(DECLARE_H__)
#define DECLARE_H__

#include "value.h"
#include "typedef.h"
#include "../symbol_table/symbol_table.h"

typedef struct
{
    char *name;
    TypedefElement *type;
    ValueElement *value;
} DeclareElement;

DeclareElement *newDeclare(char *name, TypedefElement *type, ValueElement *value);
void freeDeclare(DeclareElement *declareEl);
void evalDeclare(DeclareElement *declareElement, SymbolElement **symbolTable);

#endif // DECLARE_H__
