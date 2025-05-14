#if !defined(VARIABLE_H__)
#define VARIABLE_H__

#include <stdbool.h>
#include "./declare.h"
#include "ir/eval/type.h"
#include "ir/eval/context.h"

typedef enum
{
    var_identifier,
    var_declare,
} VariableElementType;

typedef union
{
    DeclareElement *declare;
    char *identifier;
} VariableElementData;

typedef struct variableData
{
    Type *variable_type;
    bool is_declaration;

    void (*free)(struct variableData *);
} VariableData;

typedef struct variableElement
{
    VariableElementType element_type;
    VariableElementData element_data;

    void (*free)(struct variableElement *);
    VariableData *(*eval)(struct variableElement *, EvalContext *);
} VariableElement;

VariableElement *newIdentifierVariable(char *identifier);
VariableElement *newDeclareVariable(DeclareElement *declare);

#endif // VARIABLE_H__
