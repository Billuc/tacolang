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
void *pop(LinkedList *list);
void *shift(LinkedList *list);
void *find(LinkedList *list, void *data, int (*compare)(void *, void *));
int compare(LinkedList *list1, LinkedList *list2, int (*compare)(void *, void *));
LinkedList *map(LinkedList *list, void *(*map)(void *), void (*freeData)(void *));
LinkedList *copy(LinkedList *list, void *(*copyElement)(void *));

#endif // LIST_UTILS_H__
