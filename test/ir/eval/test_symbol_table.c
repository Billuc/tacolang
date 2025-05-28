#include "ir/eval/symbol_table.h"
#include <check.h>
#include <stdlib.h>
#include <string.h>

START_TEST(test_create_and_free_SymbolTable) {
  SymbolTable *table = create_SymbolTable();
  ck_assert_ptr_nonnull(table);
  freeLinkedList(table);
}
END_TEST

START_TEST(test_add_and_find_symbol) {
  SymbolTable *table = create_SymbolTable();

  SymbolData *symbol = malloc(sizeof(SymbolData));
  symbol->name = strdup("foo");
  symbol->type = NULL;

  add(table, symbol);

  SymbolData to_find = {.name = "foo"};
  SymbolData *found = (SymbolData *)find(table, &to_find, compare_SymbolData);
  ck_assert_ptr_nonnull(found);
  ck_assert_str_eq(found->name, "foo");

  freeLinkedList(table);
}
END_TEST

START_TEST(test_compare_SymbolData) {
  SymbolData a = {.name = "a"};
  SymbolData b = {.name = "b"};
  SymbolData a2 = {.name = "a"};

  ck_assert_int_eq(compare_SymbolData(&a, &a2), 0);
  ck_assert_int_ne(compare_SymbolData(&a, &b), 0);
}
END_TEST

Suite *test_symbol_table_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Test symbol_table.c");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_create_and_free_SymbolTable);
  tcase_add_test(tc_core, test_add_and_find_symbol);
  tcase_add_test(tc_core, test_compare_SymbolData);

  suite_add_tcase(s, tc_core);

  return s;
}
