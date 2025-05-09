#if !defined(VARIABLE_H__)
#define VARIABLE_H__

#include <stdbool.h>
#include "declare.h"
#include "../symbol_table/symbol_table.h"
#include "../eval/type.h"

typedef enum
{
    var_identifier,
    var_declare,
} VariableType;

typedef union
{
    DeclareElement *declare;
    char *identifier;
} VariableData;

typedef struct
{
    VariableType type;
    VariableData variable;
} VariableElement;

typedef struct
{
    EvalType *variableType;
    bool isDeclaration;
} EvalVariableData;

VariableElement *newIdentifierVariable(char *identifier);
VariableElement *newDeclareVariable(DeclareElement *declare);
void freeVariable(VariableElement *variableEl);
EvalVariableData *evalVariable(VariableElement *variableElement, SymbolElement **symbolTable);

#endif // VARIABLE_H__
