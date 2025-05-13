#if !defined(LIST_UTILS_H__)
#define LIST_UTILS_H__

typedef struct linkedListElement
{
    void *data;
    struct linkedListElement *next;
    struct linkedListElement *prev;
} LinkedListElement;

typedef struct linkedList
{
    LinkedListElement *head;
    LinkedListElement *tail;
    int size;
    void (*freeData)(void *data);
} LinkedList;

LinkedList *newLinkedList(void (*freeData)(void *));
void freeLinkedList(LinkedList *list);
void push(LinkedList *list, void *data);
void unshift(LinkedList *list, void *data);
void *find(LinkedList *list, void *data, int (*compare)(void *, void *));
int compare(LinkedList *list1, LinkedList *list2, int (*compare)(void *, void *));

#endif // LIST_UTILS_H__
