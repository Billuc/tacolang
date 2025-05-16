#if !defined(MOCK_VARIABLE_H__)
#define MOCK_VARIABLE_H__

#include "ir/ast/variable.h"

typedef enum
{
    mock_DeclareVariable,
    mock_IdentifierVariable,
} MockVariableType;

extern int calls_to_freeVariable;
extern int calls_to_freeVariableData;
extern int calls_to_evalVariable;

VariableElement *mock_newVariable(MockVariableType variant);

#endif // MOCK_VARIABLE_H__
