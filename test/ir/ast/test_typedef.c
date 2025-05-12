#include <check.h>
#include <stdlib.h>
#include "ir/ast/typedef.h"
#include "ir/ast/modifier.h"

/* ------------------------ MOCKS ------------------------ */

static int calls_to_freeModifierList = 0;
void mock_freeModifierList(ModifierLink *modifiers)
{
    calls_to_freeModifierList++;
    free(modifiers);
}

ModifierLink *mock_newModifierList(void)
{
    ModifierLink *modifiers = malloc(sizeof(ModifierLink));
    return modifiers;
}

/* ------------------------ FIXTURES ------------------------ */

static void setup(void)
{
    calls_to_freeModifierList = 0;
}

static void teardown(void)
{
}

/* ------------------------ TESTS ------------------------ */

START_TEST(test_newTypedef)
{
    ModifierLink *mockModifiers = mock_newModifierList();
    TypedefElement *typedefEl = newTypedef(mockModifiers, "int");

    ck_assert_ptr_nonnull(typedefEl);
    ck_assert_str_eq(typedefEl->name, "int");
    ck_assert_ptr_eq(typedefEl->modifiers, mockModifiers);

    typedefEl->free(typedefEl);
    ck_assert_int_eq(calls_to_freeModifierList, 1);
}
END_TEST

START_TEST(test_evalTypedef)
{
    ModifierLink *mockModifiers = mock_newModifierList();
    TypedefElement *typedefEl = newTypedef(mockModifiers, "int");
    SymbolElement *mockSymbolTable = NULL;

    typedefEl->eval(typedefEl, &mockSymbolTable);

    typedefEl->free(typedefEl);
    ck_assert_int_eq(calls_to_freeModifierList, 1);
}
END_TEST

Suite *test_typedef_suite(void)
{
    Suite *s;
    TCase *tc_typedef;

    s = suite_create("Test typedef.c");
    tc_typedef = tcase_create("Typedef");

    tcase_add_checked_fixture(tc_typedef, setup, teardown);
    tcase_add_test(tc_typedef, test_newTypedef);
    tcase_add_test(tc_typedef, test_evalTypedef);
    suite_add_tcase(s, tc_typedef);

    return s;
}
