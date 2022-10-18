#include <stdlib.h>
#include <stdio.h>
#include "linkedlist.h"


linkedlist*newlist()
{
    linkedlist* myList = (linkedlist*)malloc(sizeof(linkedlist));
    myList->head = NULL;
    myList->count = 0;
    return myList;
}


void insertFirst(linkedlist*myList, void*value)
{
    listnode*newNode = (listnode*)malloc(sizeof(listnode));
    
    newNode->value = value;
    newNode->next = NULL;

    // Check if list is empty!
    if(myList->head == NULL)
    {
        myList->head = newNode;
    }
    else
    {
        newNode->next = myList->head;
        myList->head = newNode;
    }
    myList->count++;
}

void manualFree(linkedlist* list, freeSLL* freeVal)
{
    listnode*node;
    listnode*nextnode;

    if(list->count > 0)
    {
        node = list->head;
        while(node != NULL)
        {   
            // free the nodes
            nextnode = node->next;
            (*freeVal)(node->value);
            free(node);
            node = nextnode;
        }
    }
    // free the list
    free(list);
}