#ifndef LINKEDLIST_H
#define LINKEDLIST_H


typedef struct listnode
{
    void* value;
    struct listnode* next;
} listnode;

typedef struct
{
    listnode* head;
    int count;
} linkedlist;


typedef char* linkedlistString(void*);
typedef void(*freeSLL)(void*);
linkedlist*newlist();
void insertFirst(linkedlist*myList, void*value);
void manualFree(linkedlist*, freeSLL*);
linkedlist*resizeList(linkedlist*myList);

#endif
