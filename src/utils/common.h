#if !defined(COMMON_H__)
#define COMMON_H__

#include "ir/eval/symbol_table.h"

// A macro to declare a free function pointer type for a struct
#define FREE_FUNC(type, name) \
    void (*name)(type *)

// A macro to define an eval function for a struct
#define EVAL_FUNC(type, data, name) \
    data (*name)(type * element, SymbolTable * symbolTable)

#endif // COMMON_H__
