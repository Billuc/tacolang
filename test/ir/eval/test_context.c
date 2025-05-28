
#include "ir/eval/context.h"
#include "ir/eval/symbol_table.h"
#include <check.h>
#include <stdlib.h>
#include <string.h>

START_TEST(test_newEvalContext_and_freeEvalContext) {
  EvalContext *context = newEvalContext();
  ck_assert_ptr_nonnull(context);
  ck_assert_ptr_nonnull(context->symbolTableStack);
  context->free(context);
}
END_TEST

START_TEST(test_put_and_find_symbol) {
  EvalContext *context = newEvalContext();

  SymbolData *symbol = malloc(sizeof(SymbolData));
  symbol->name = strdup("x");
  symbol->type = NULL;

  putSymbol(context, symbol);

  SymbolData *found = findSymbol(context, "x");
  ck_assert_ptr_nonnull(found);
  ck_assert_str_eq(found->name, "x");

  context->free(context);
}
END_TEST

Suite *test_context_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Test context.c");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_newEvalContext_and_freeEvalContext);
  tcase_add_test(tc_core, test_put_and_find_symbol);

  suite_add_tcase(s, tc_core);

  return s;
}
