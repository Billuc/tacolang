#ifndef AST_TYPES_H__
#define AST_TYPES_H__

#include "ast_data.h"

typedef enum
{
    assign,
    declare,
    expression,
    integer,
    statement,
    modifier,
    typedefinition,
} ASTType;

typedef union
{
    AssignData assignData;
    DeclareData declareData;
    ExpressionData expressionData;
    IntegerData integerData;
    NoData noData;
    ModifierData modifierData;
    TypeDefData typedefData;
} ASTData;

#endif