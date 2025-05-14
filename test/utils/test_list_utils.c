#include "utils/list_utils.h"
#include <check.h>
#include "utils/list_utils.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void freeString(void *data)
{
    free(data);
}

void *copyString(void *data)
{
    return strdup((char *)data);
}

void *toUpperCase(void *data)
{
    char *str = strdup((char *)data);
    for (char *p = str; *p; ++p)
    {
        *p = toupper(*p);
    }
    return str;
}

int stringCompare(void *a, void *b)
{
    return strcmp((char *)a, (char *)b);
}

START_TEST(test_newLinkedList)
{
    LinkedList *list = newLinkedList(freeString);
    ck_assert_ptr_nonnull(list);
    ck_assert_int_eq(list->size, 0);
    ck_assert_ptr_null(list->head);
    ck_assert_ptr_null(list->tail);
    freeLinkedList(list);
}
END_TEST

START_TEST(test_push)
{
    LinkedList *list = newLinkedList(freeString);

    push(list, strdup("world"));
    push(list, strdup("!"));

    ck_assert_int_eq(list->size, 2);
    ck_assert_str_eq((char *)list->head->data, "world");
    ck_assert_str_eq((char *)list->tail->data, "!");

    freeLinkedList(list);
}
END_TEST

START_TEST(test_unshift)
{
    LinkedList *list = newLinkedList(freeString);

    unshift(list, strdup("world"));
    unshift(list, strdup("Hello"));

    ck_assert_int_eq(list->size, 2);
    ck_assert_str_eq((char *)list->head->data, "Hello");
    ck_assert_str_eq((char *)list->tail->data, "world");

    freeLinkedList(list);
}
END_TEST

START_TEST(test_find)
{
    LinkedList *list = newLinkedList(freeString);

    push(list, strdup("apple"));
    push(list, strdup("banana"));
    push(list, strdup("cherry"));

    void *found = find(list, "banana", stringCompare);
    ck_assert_ptr_nonnull(found);
    ck_assert_str_eq((char *)found, "banana");

    found = find(list, "grape", stringCompare);
    ck_assert_ptr_null(found);

    freeLinkedList(list);
}
END_TEST

START_TEST(test_compare)
{
    LinkedList *list1 = newLinkedList(freeString);
    LinkedList *list2 = newLinkedList(freeString);

    push(list1, strdup("a"));
    push(list1, strdup("b"));
    push(list1, strdup("c"));

    push(list2, strdup("a"));
    push(list2, strdup("b"));
    push(list2, strdup("c"));

    ck_assert_int_eq(compare(list1, list2, stringCompare), 0);

    push(list2, strdup("d"));
    ck_assert_int_lt(compare(list1, list2, stringCompare), 0);

    push(list1, strdup("e"));
    ck_assert_int_gt(compare(list1, list2, stringCompare), 0);

    freeLinkedList(list1);
    freeLinkedList(list2);
}
END_TEST

START_TEST(test_map)
{
    LinkedList *list = newLinkedList(freeString);
    push(list, strdup("hello"));
    push(list, strdup("world"));

    LinkedList *upperList = map(list, toUpperCase, freeString);

    ck_assert_int_eq(upperList->size, 2);
    ck_assert_str_eq((char *)upperList->head->data, "HELLO");
    ck_assert_str_eq((char *)upperList->tail->data, "WORLD");

    freeLinkedList(list);
    freeLinkedList(upperList);
}
END_TEST

START_TEST(test_copy)
{
    LinkedList *list = newLinkedList(freeString);
    push(list, strdup("foo"));
    push(list, strdup("bar"));

    LinkedList *copyList = copy(list, copyString);

    ck_assert_int_eq(copyList->size, 2);
    ck_assert_str_eq((char *)copyList->head->data, "foo");
    ck_assert_str_eq((char *)copyList->tail->data, "bar");

    freeLinkedList(list);
    freeLinkedList(copyList);
}
END_TEST

Suite *test_list_utils_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ListUtils");

    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_newLinkedList);
    tcase_add_test(tc_core, test_push);
    tcase_add_test(tc_core, test_unshift);
    tcase_add_test(tc_core, test_find);
    tcase_add_test(tc_core, test_compare);
    tcase_add_test(tc_core, test_map);
    tcase_add_test(tc_core, test_copy);
    suite_add_tcase(s, tc_core);

    return s;
}
