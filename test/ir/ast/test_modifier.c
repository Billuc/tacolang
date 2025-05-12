#include <check.h>
#include <stdlib.h>
#include "ir/ast/modifier.h"

/* ------------------------ MOCKS ------------------------ */

// Add mock implementations for functions in modifier.c if needed

/* ------------------------ FIXTURES ------------------------ */

static void setup(void)
{
    // Initialize any global state or variables
}

static void teardown(void)
{
    // Clean up any global state or variables
}

/* ------------------------ TESTS ------------------------ */

START_TEST(test_newModifier_valid_types)
{
    ModifierElement *mutableMod = newModifier("mut");
    ck_assert_ptr_nonnull(mutableMod);
    ck_assert_int_eq(mutableMod->type, mutable);
    mutableMod->free(mutableMod);

    ModifierElement *referenceMod = newModifier("ref");
    ck_assert_ptr_nonnull(referenceMod);
    ck_assert_int_eq(referenceMod->type, reference);
    referenceMod->free(referenceMod);

    ModifierElement *optionalMod = newModifier("opt");
    ck_assert_ptr_nonnull(optionalMod);
    ck_assert_int_eq(optionalMod->type, optional);
    optionalMod->free(optionalMod);
}
END_TEST

START_TEST(test_newModifier_invalid_type)
{
    ModifierElement *invalidMod = newModifier("invalid");
    ck_assert_ptr_null(invalidMod);
}
END_TEST

START_TEST(test_freeModifier)
{
    ModifierElement *mod = newModifier("mut");

    mod->free(mod);

    // No direct way to test free, but ensure no segfault occurs
    ck_assert_msg(1, "freeModifier executed without crashing");
}

START_TEST(test_evalModifier)
{
    ModifierElement *mod = newModifier("mut");
    SymbolElement *mockSymbolTable = NULL; // Mock or initialize as needed

    EvalModifierData *evalData = mod->eval(mod, &mockSymbolTable);

    ck_assert_ptr_nonnull(evalData);
    ck_assert_int_eq(evalData->typeModifier.modifier_type, tm_mutable);

    free(evalData);
}

START_TEST(test_addModifier)
{
    ModifierElement *mod1 = newModifier("mut");
    ModifierElement *mod2 = newModifier("ref");
    ModifierLink *list = NULL;

    list = addModifier(list, mod1);
    ck_assert_ptr_nonnull(list);
    ck_assert_ptr_eq(list->data, mod1);

    list = addModifier(list, mod2);
    ck_assert_ptr_nonnull(list);
    ck_assert_ptr_eq(list->data, mod2);
    ck_assert_ptr_eq(list->next->data, mod1);

    freeModifierList(list);
}
END_TEST

START_TEST(test_freeModifierList)
{
    ModifierElement *mod1 = newModifier("mut");
    ModifierElement *mod2 = newModifier("ref");
    ModifierLink *list = NULL;

    list = addModifier(list, mod1);
    list = addModifier(list, mod2);

    freeModifierList(list);
    // No memory leaks should occur
}
END_TEST

START_TEST(test_evalModifierList)
{
    ModifierElement *mod1 = newModifier("mut");
    ModifierElement *mod2 = newModifier("ref");
    ModifierLink *list = NULL;

    list = addModifier(list, mod1);
    list = addModifier(list, mod2);

    SymbolElement *symbolTable = NULL; // Mock or initialize as needed
    EvalModifierLinkData *evalData = evalModifierList(list, &symbolTable);

    ck_assert_ptr_nonnull(evalData);
    ck_assert_ptr_nonnull(evalData->typeModifierList);
    ck_assert_int_eq(evalData->typeModifierList->element.modifier_type, tm_mutable);
    ck_assert_int_eq(evalData->typeModifierList->next->element.modifier_type, tm_reference);

    // Free allocated memory
    freeModifierList(list);
    free(evalData);
}
END_TEST

Suite *test_modifier_suite(void)
{
    Suite *s;
    TCase *tc_modifier, *tc_modifier_list;

    s = suite_create("Test modifier.c");
    tc_modifier = tcase_create("Modifier");

    tcase_add_checked_fixture(tc_modifier, setup, teardown);
    tcase_add_test(tc_modifier, test_newModifier_valid_types);
    tcase_add_test(tc_modifier, test_newModifier_invalid_type);
    tcase_add_test(tc_modifier, test_freeModifier);
    tcase_add_test(tc_modifier, test_evalModifier);
    suite_add_tcase(s, tc_modifier);

    tc_modifier_list = tcase_create("Modifier List");
    tcase_add_test(tc_modifier_list, test_addModifier);
    tcase_add_test(tc_modifier_list, test_freeModifierList);
    tcase_add_test(tc_modifier_list, test_evalModifierList);
    suite_add_tcase(s, tc_modifier_list);

    return s;
}
