#ifndef AST_TYPES_H__
#define AST_TYPES_H__

#include "ast_data.h"

typedef enum {
    assign,
    declare,
    expression,
    integer,
} ASTType;

typedef union {
    AssignData assignData;
    DeclareData declareData;
    ExpressionData expressionData;
    IntegerData integerData;
} ASTData;

#endif