#include "utils/str_utils.h"
#include <check.h>
#include <stdlib.h>
#include <string.h>

START_TEST(test_strdup_valid_string)
{
    const char *original = "Hello, world!";
    char *copy = strdup(original);

    ck_assert_ptr_nonnull(copy);
    ck_assert_str_eq(copy, original);

    free(copy);
}
END_TEST

START_TEST(test_strdup_empty_string)
{
    const char *original = "";
    char *copy = strdup(original);

    ck_assert_ptr_nonnull(copy);
    ck_assert_str_eq(copy, original);

    free(copy);
}
END_TEST

START_TEST(test_strdup_null_pointer)
{
    char *copy = strdup(NULL);

    ck_assert_ptr_null(copy);
}
END_TEST

START_TEST(test_strndup_valid_string)
{
    const char *original = "Hello, world!";
    size_t len = 5;
    char *copy = strndup(original, len);

    ck_assert_ptr_nonnull(copy);
    ck_assert_int_eq(strlen(copy), len);
    ck_assert_strn_eq(copy, original, len);

    free(copy);
}
END_TEST

START_TEST(test_strndup_empty_string)
{
    const char *original = "";
    size_t len = 5;
    char *copy = strndup(original, len);

    ck_assert_ptr_nonnull(copy);
    ck_assert_str_eq(copy, original);

    free(copy);
}
END_TEST

START_TEST(test_strndup_null_pointer)
{
    char *copy = strndup(NULL, 5);

    ck_assert_ptr_null(copy);
}
END_TEST

START_TEST(test_strndup_length_exceeds_string)
{
    const char *original = "Hello";
    size_t len = 10;
    char *copy = strndup(original, len);

    ck_assert_ptr_nonnull(copy);
    ck_assert_str_eq(copy, original);

    free(copy);
}
END_TEST

Suite *test_str_utils_suite(void)
{
    Suite *s = suite_create("str_utils");
    TCase *tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_strdup_valid_string);
    tcase_add_test(tc_core, test_strdup_empty_string);
    tcase_add_test(tc_core, test_strdup_null_pointer);
    tcase_add_test(tc_core, test_strndup_valid_string);
    tcase_add_test(tc_core, test_strndup_empty_string);
    tcase_add_test(tc_core, test_strndup_null_pointer);
    tcase_add_test(tc_core, test_strndup_length_exceeds_string);

    suite_add_tcase(s, tc_core);
    return s;
}
