#ifndef AST_H__
#define AST_H__

#include "ast_types.h"
#include "../symbol_table/symbol_table.h"

typedef struct astElement
{
    ASTType type;
    ASTData data;
    struct astElement *left;
    struct astElement *right;
} ASTElement;

ASTElement *newAssign(char *name, ASTElement *value);
ASTElement *newDeclare(char *name, char *type, ASTElement *value);
ASTElement *newExpression(char *operation, ASTElement *left, ASTElement *right);
ASTElement *newInteger(int value);
ASTElement *newStatement(ASTElement *previousStatements, ASTElement *stmt);
ASTElement *newModifier(char *modifier, ASTElement *previousModifiers);
ASTElement *newTypedef(char *typeName, ASTElement *modifiers);

void freeAST(ASTElement *astElement);
void evalAST(ASTElement *astElement, SymbolElement **symbolTable);

#endif