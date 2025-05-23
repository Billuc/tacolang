#if !defined(VARIABLE_H__)
#define VARIABLE_H__

#include <stdbool.h>
#include "./declare.h"
#include "ir/eval/type.h"
#include "ir/eval/context.h"
#include "utils/error_utils.h"

typedef enum
{
    var_identifier,
} VariableElementType;

typedef union
{
    char *identifier;
} VariableElementData;

typedef struct variableData
{
    Type *variable_type;
    char *generated_code;

    void (*free)(struct variableData *);
} VariableData;

typedef struct variableElement
{
    VariableElementType element_type;
    VariableElementData element_data;
    location_t location;

    void (*free)(struct variableElement *);
    VariableData *(*eval)(struct variableElement *, EvalContext *);
} VariableElement;

VariableElement *newIdentifierVariable(char *identifier, location_t location);

#endif // VARIABLE_H__
