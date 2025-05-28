
#include "ir/eval/type_modifier.h"
#include <check.h>
#include <stdlib.h>

extern int number_of_errors;
extern char **error_messages;

/* ------------------------ MOCKS ------------------------ */

/* ------------------------ FIXTURES ------------------------ */

static void setup(void) {}

static void teardown(void) {}

/* ------------------------ TESTS ------------------------ */

START_TEST(test_compare_typeModifier) {
  TypeModifier *modifier1 = malloc(sizeof(TypeModifier));
  TypeModifier *modifier2 = malloc(sizeof(TypeModifier));

  modifier1->modifier_type = tm_mutable;
  modifier2->modifier_type = tm_mutable;

  ck_assert_int_eq(compare_typeModifier(modifier1, modifier2), 0);

  free(modifier1);
  free(modifier2);
}
END_TEST

START_TEST(test_compare_array_typeModifier) {
  TypeModifier *modifier1 = malloc(sizeof(TypeModifier));
  TypeModifier *modifier2 = malloc(sizeof(TypeModifier));

  modifier1->modifier_type = tm_array;
  modifier1->modifier_data.array_size = 5;

  modifier2->modifier_type = tm_array;
  modifier2->modifier_data.array_size = 5;

  ck_assert_int_eq(compare_typeModifier(modifier1, modifier2), 0);

  free(modifier1);
  free(modifier2);
}
END_TEST

START_TEST(test_compare_different_typeModifier) {
  TypeModifier *modifier1 = malloc(sizeof(TypeModifier));
  TypeModifier *modifier2 = malloc(sizeof(TypeModifier));

  modifier1->modifier_type = tm_mutable;
  modifier2->modifier_type = tm_array;

  ck_assert_int_ne(compare_typeModifier(modifier1, modifier2), 0);

  free(modifier1);
  free(modifier2);
}
END_TEST

START_TEST(test_copy_typeModifier) {
  TypeModifier *original = malloc(sizeof(TypeModifier));
  original->modifier_type = tm_mutable;

  TypeModifier *copy = copy_typeModifier(original);

  ck_assert_ptr_nonnull(copy);
  ck_assert_int_eq(copy->modifier_type, original->modifier_type);
  ck_assert_int_eq(compare_typeModifier(original, copy), 0);

  free(original);
  free(copy);
}
END_TEST

/* ------------------------- MAIN ------------------------- */

Suite *test_type_modifier_suite(void) {
  Suite *s;
  TCase *tc_type_modifier;

  s = suite_create("Test type_modifier.c");
  tc_type_modifier = tcase_create("Core");

  tcase_add_checked_fixture(tc_type_modifier, setup, teardown);
  tcase_add_test(tc_type_modifier, test_compare_typeModifier);
  tcase_add_test(tc_type_modifier, test_compare_array_typeModifier);
  tcase_add_test(tc_type_modifier, test_compare_different_typeModifier);
  tcase_add_test(tc_type_modifier, test_copy_typeModifier);
  suite_add_tcase(s, tc_type_modifier);

  return s;
}
