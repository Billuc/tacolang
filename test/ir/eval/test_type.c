#include "ir/eval/type.h"
#include <check.h>
#include <stdlib.h>
#include <string.h>

START_TEST(test_compare_simpleType_base) {
  SimpleType a = {.is_base_type = 1,
                  .type_data.base_type = U8,
                  .modifiers = newLinkedList(NULL)};
  SimpleType b = {.is_base_type = 1,
                  .type_data.base_type = U8,
                  .modifiers = newLinkedList(NULL)};
  ck_assert_int_eq(compare_simpleType(a, b), 0);

  b.type_data.base_type = U16;
  ck_assert_int_ne(compare_simpleType(a, b), 0);

  freeLinkedList(a.modifiers);
  freeLinkedList(b.modifiers);
}
END_TEST

START_TEST(test_compare_simpleType_custom) {
  SimpleType a = {.is_base_type = 0,
                  .type_data.custom_type = strdup("Foo"),
                  .modifiers = newLinkedList(NULL)};
  SimpleType b = {.is_base_type = 0,
                  .type_data.custom_type = strdup("Foo"),
                  .modifiers = newLinkedList(NULL)};
  ck_assert_int_eq(compare_simpleType(a, b), 0);

  free(a.type_data.custom_type);
  free(b.type_data.custom_type);
  freeLinkedList(a.modifiers);
  freeLinkedList(b.modifiers);
}
END_TEST

START_TEST(test_copy_simpleType) {
  SimpleType a = {.is_base_type = 1,
                  .type_data.base_type = U8,
                  .modifiers = newLinkedList(NULL)};
  SimpleType b;
  copy_simpleType(&a, &b);
  ck_assert_int_eq(compare_simpleType(a, b), 0);
  free_simpleType(&a);
  free_simpleType(&b);
}
END_TEST

START_TEST(test_compare_type) {
  Type *a = malloc(sizeof(Type));
  a->type_type = t_variable;
  a->type_data.variable_type.is_base_type = 1;
  a->type_data.variable_type.type_data.base_type = U8;
  a->type_data.variable_type.modifiers = newLinkedList(NULL);

  Type *b = malloc(sizeof(Type));
  b->type_type = t_variable;
  b->type_data.variable_type.is_base_type = 1;
  b->type_data.variable_type.type_data.base_type = U8;
  b->type_data.variable_type.modifiers = newLinkedList(NULL);

  ck_assert_int_eq(compare_type(a, b), 0);

  free_type(a);
  free_type(b);
}
END_TEST

START_TEST(test_compare_functionType) {
  FunctionType a = {.return_type = {.is_base_type = 1,
                                    .type_data.base_type = U8,
                                    .modifiers = newLinkedList(NULL)},
                    .number_of_args = 2,
                    .argument_types = {{.is_base_type = 1,
                                        .type_data.base_type = U8,
                                        .modifiers = newLinkedList(NULL)},
                                       {.is_base_type = 1,
                                        .type_data.base_type = U16,
                                        .modifiers = newLinkedList(NULL)}}};

  FunctionType b = {.return_type = {.is_base_type = 1,
                                    .type_data.base_type = U8,
                                    .modifiers = newLinkedList(NULL)},
                    .number_of_args = 2,
                    .argument_types = {{.is_base_type = 1,
                                        .type_data.base_type = U8,
                                        .modifiers = newLinkedList(NULL)},
                                       {.is_base_type = 1,
                                        .type_data.base_type = U16,
                                        .modifiers = newLinkedList(NULL)}}};

  ck_assert_int_eq(compare_functionType(a, b), 0);

  freeLinkedList(a.return_type.modifiers);
  freeLinkedList(b.return_type.modifiers);
}
END_TEST

START_TEST(test_compare_different_functionType) {
  FunctionType a = {.return_type = {.is_base_type = 1,
                                    .type_data.base_type = U8,
                                    .modifiers = newLinkedList(NULL)},
                    .number_of_args = 2,
                    .argument_types = {{.is_base_type = 1,
                                        .type_data.base_type = U8,
                                        .modifiers = newLinkedList(NULL)},
                                       {.is_base_type = 1,
                                        .type_data.base_type = U16,
                                        .modifiers = newLinkedList(NULL)}}};

  FunctionType b = {.return_type = {.is_base_type = 1,
                                    .type_data.base_type = U8,
                                    .modifiers = newLinkedList(NULL)},
                    .number_of_args = 2,
                    .argument_types = {{.is_base_type = 1,
                                        .type_data.base_type = U8,
                                        .modifiers = newLinkedList(NULL)},
                                       {.is_base_type = 1,
                                        .type_data.base_type = U32,
                                        .modifiers = newLinkedList(NULL)}}};

  ck_assert_int_ne(compare_functionType(a, b), 0);

  freeLinkedList(a.return_type.modifiers);
  freeLinkedList(b.return_type.modifiers);
}
END_TEST

START_TEST(test_compare_structType) {
  StructType a = {
      .struct_name = strdup("Foo"),
      .number_of_fields = 2,
      .fields = {{.field_name = strdup("x"),
                  .field_type = {.is_base_type = 1,
                                 .type_data.base_type = U8,
                                 .modifiers = newLinkedList(NULL)}},
                 {.field_name = strdup("y"),
                  .field_type = {.is_base_type = 1,
                                 .type_data.base_type = U16,
                                 .modifiers = newLinkedList(NULL)}}}};

  StructType b = {
      .struct_name = strdup("Foo"),
      .number_of_fields = 2,
      .fields = {{.field_name = strdup("x"),
                  .field_type = {.is_base_type = 1,
                                 .type_data.base_type = U8,
                                 .modifiers = newLinkedList(NULL)}},
                 {.field_name = strdup("y"),
                  .field_type = {.is_base_type = 1,
                                 .type_data.base_type = U16,
                                 .modifiers = newLinkedList(NULL)}}}};

  ck_assert_int_eq(compare_structType(a, b), 0);

  free(a.struct_name);
  free(b.struct_name);
}
END_TEST

START_TEST(test_compare_different_structType) {
  StructType a = {
      .struct_name = strdup("Foo"),
      .number_of_fields = 2,
      .fields = {{.field_name = strdup("x"),
                  .field_type = {.is_base_type = 1,
                                 .type_data.base_type = U8,
                                 .modifiers = newLinkedList(NULL)}},
                 {.field_name = strdup("y"),
                  .field_type = {.is_base_type = 1,
                                 .type_data.base_type = U16,
                                 .modifiers = newLinkedList(NULL)}}}};

  StructType b = {
      .struct_name = strdup("Foo"),
      .number_of_fields = 2,
      .fields = {{.field_name = strdup("x"),
                  .field_type = {.is_base_type = 1,
                                 .type_data.base_type = U8,
                                 .modifiers = newLinkedList(NULL)}},
                 {.field_name = strdup("z"),
                  .field_type = {.is_base_type = 1,
                                 .type_data.base_type = U16,
                                 .modifiers = newLinkedList(NULL)}}}};

  ck_assert_int_ne(compare_structType(a, b), 0);

  free(a.struct_name);
  free(b.struct_name);
}
END_TEST

/* ------------------- MAIN ------------------- */

Suite *test_type_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Test type.c");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_compare_simpleType_base);
  tcase_add_test(tc_core, test_compare_simpleType_custom);
  tcase_add_test(tc_core, test_print_simpleType);
  tcase_add_test(tc_core, test_copy_and_free_simpleType);
  tcase_add_test(tc_core, test_compare_type);

  suite_add_tcase(s, tc_core);

  return s;
}
