#if !defined(VARIABLE_H__)
#define VARIABLE_H__

#include <stdbool.h>
#include "declare.h"
#include "common.h"
#include "ir/eval/symbol_table.h"
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
    EvalType *variableType;
    bool isDeclaration;
} EvalVariableData;

typedef struct variableElement
{
    VariableType type;
    VariableData data;

    FREE_FUNC(struct variableElement, free);
    EVAL_FUNC(struct variableElement, EvalVariableData *, eval);
} VariableElement;

VariableElement *newIdentifierVariable(char *identifier);
VariableElement *newDeclareVariable(DeclareElement *declare);
// void freeVariable(VariableElement *variableEl);
// EvalVariableData *evalVariable(VariableElement *variableElement, SymbolElement **symbolTable);

#endif // VARIABLE_H__
