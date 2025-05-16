#if !defined(MOCK_TYPEDEF_H__)
#define MOCK_TYPEDEF_H__

#include "ir/ast/typedef.h"

extern int calls_to_freeTypedef;
extern int calls_to_evalTypedef;
extern int calls_to_freeTypedefData;

TypedefElement *mock_newTypedef(void);

#endif // MOCK_TYPEDEF_H__
