#include <stdlib.h>
#include "list_utils.h"

LinkedList *newLinkedList(void (*freeData)(void *))
{
    if (freeData == NULL)
    {
        freeData = free;
    }

    LinkedList *list = malloc(sizeof(LinkedList));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->freeData = freeData;
    return list;
}

void freeLinkedList(LinkedList *list)
{
    if (list == NULL)
        return;

    LinkedListElement *current = list->head;
    LinkedListElement *next;

    while (current != NULL)
    {
        next = current->next;
        list->freeData(current->data);
        free(current);
        current = next;
    }

    free(list);
}

void push(LinkedList *list, void *data)
{
    if (data == NULL)
        return;

    LinkedListElement *newElement = malloc(sizeof(LinkedListElement));
    newElement->data = data;
    newElement->next = NULL;

    if (list->tail == NULL)
    {
        newElement->prev = NULL;
        list->head = newElement;
        list->tail = newElement;
    }
    else
    {
        newElement->prev = list->tail;
        list->tail->next = newElement;
        list->tail = newElement;
    }

    list->size++;
}

void unshift(LinkedList *list, void *data)
{
    LinkedListElement *newElement = malloc(sizeof(LinkedListElement));
    newElement->data = data;
    newElement->prev = NULL;

    if (list->head == NULL)
    {
        newElement->next = NULL;
        list->head = newElement;
        list->tail = newElement;
    }
    else
    {
        newElement->next = list->head;
        list->head->prev = newElement;
        list->head = newElement;
    }

    list->size++;
}

static int ptr_compare(void *a, void *b)
{
    return a - b;
}

void *find(LinkedList *list, void *data, int (*compare)(void *, void *))
{
    if (compare == NULL)
    {
        compare = ptr_compare;
    }

    LinkedListElement *current = list->head;

    while (current != NULL)
    {
        if (compare(current->data, data) == 0)
        {
            return current->data;
        }
        current = current->next;
    }

    return NULL;
}

int compare(LinkedList *list1, LinkedList *list2, int (*compare)(void *, void *))
{
    if (list1 == NULL && list2 == NULL)
        return 0;
    if (list1 == NULL)
        return -1;
    if (list2 == NULL)
        return 1;

    if (list1->size != list2->size)
    {
        return list1->size - list2->size;
    }

    if (compare == NULL)
    {
        compare = ptr_compare;
    }

    LinkedListElement *current1 = list1->head;
    LinkedListElement *current2 = list2->head;

    while (current1 != NULL && current2 != NULL)
    {
        int compare_result = compare(current1->data, current2->data);
        if (compare_result != 0)
        {
            return compare_result;
        }

        current1 = current1->next;
        current2 = current2->next;
    }

    if (current1 != NULL)
        return 1;
    if (current2 != NULL)
        return -1;
    return 0;
}

LinkedList *map(LinkedList *list, void *(*map)(void *), void (*freeData)(void *))
{
    LinkedList *newList = newLinkedList(freeData);
    LinkedListElement *current;

    for (current = list->head; current != NULL; current = current->next)
    {
        push(newList, map(current->data));
    }

    return newList;
}

LinkedList *copy(LinkedList *list, void *(*copyElement)(void *))
{
    LinkedList *newList = newLinkedList(list->freeData);
    LinkedListElement *current;

    for (current = list->head; current != NULL; current = current->next)
    {
        push(newList, copyElement(current->data));
    }

    return newList;
}