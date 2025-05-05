#ifndef AST_H__
#define AST_H__

#include "ast_types.h"

typedef struct astElement
{
    ASTType type;
    ASTData data;
    struct astElement *left;
    struct astElement *right;
} ASTElement;

ASTElement *newAST(ASTType type, ASTData data, ASTElement *left, ASTElement *right);
ASTElement *newAssign(char *name, ASTElement *value);
ASTElement *newDeclare(char *name, char *type, ASTElement *value);
ASTElement *newExpression(char *operation, ASTElement *left, ASTElement *right);
ASTElement *newInteger(int value);

#endif