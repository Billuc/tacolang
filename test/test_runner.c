#include <check.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define REGISTER_TEST_SUITE(name)     \
    extern Suite *name##_suite(void); \
    srunner_add_suite(sr, name##_suite())

// Required by the tested files
void yyerror(const char *s)
{
    fprintf(stderr, "[ERR] %s\n", s);
}

int main(void)
{
    int number_failed;
    SRunner *sr = srunner_create(NULL);

    // Add all test suites
    REGISTER_TEST_SUITE(test_statement);
    REGISTER_TEST_SUITE(test_modifier);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}