#include <check.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define REGISTER_TEST_SUITE(name)     \
    extern Suite *name##_suite(void); \
    srunner_add_suite(sr, name##_suite())

int number_of_errors = 0;
char **error_messages = NULL;

// Required by the tested files
void yyerror(const char *s)
{
    number_of_errors++;
    error_messages = realloc(error_messages, sizeof(char *) * number_of_errors);
    error_messages[number_of_errors - 1] = malloc(strlen(s) + 1);
    strcpy(error_messages[number_of_errors - 1], s);
    fprintf(stderr, "[ERR] %s\n", s);
}

int main(void)
{
    int number_failed;
    SRunner *sr = srunner_create(NULL);

    // Add all test suites
    // Utils
    REGISTER_TEST_SUITE(test_list_utils);
    REGISTER_TEST_SUITE(test_str_utils);
    // EVAL
    // AST
    REGISTER_TEST_SUITE(test_assign);
    REGISTER_TEST_SUITE(test_declare);
    // REGISTER_TEST_SUITE(test_expression);
    // REGISTER_TEST_SUITE(test_modifier);
    // REGISTER_TEST_SUITE(test_program);
    // REGISTER_TEST_SUITE(test_statement);
    // REGISTER_TEST_SUITE(test_typedef);
    // REGISTER_TEST_SUITE(test_value);
    // REGISTER_TEST_SUITE(test_variable);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}