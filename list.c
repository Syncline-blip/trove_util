#define _POSIX_C_SOURCE     200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

//  'CREATE' A NEW, EMPTY LIST - JUST A NULL POINTER
LIST *list_new(void)
{
    return NULL;
}

//  DETERMINE IF A REQUIRED ITEM (A STRING) IS STORED IN A GIVEN LIST
bool list_find(LIST *list, char *wanted)
{
    while(list != NULL) {
	if(strcmp(list->string, wanted) == 0) {
	    return true;
	}
	list	= list->next;
    }
    return false;
}

//  ALLOCATE SPACE FOR A NEW LIST ITEM, TESTING THAT ALLOCATION SUCCEEDS
LIST *list_new_item(char *newstring)
{
    LIST *new       = calloc(1, sizeof(LIST) );
    CHECK_ALLOC(new);
    new->string     =  strdup(newstring);
    CHECK_ALLOC(new->string);
    new->next       =  NULL;
    return new;
}

//  ADD A NEW (STRING) ITEM TO AN EXISTING LIST
LIST *list_add(LIST *list, char *newstring)
{
    if(list_find(list, newstring)) {            // only add each item once
        return list;
    }
    else {                                      // add new item to head of list
        LIST *new   = list_new_item(newstring);
        new->next   = list;
        return new;
    }
}

//  PRINT EACH ITEM (A STRING) IN A GIVEN LIST TO stdout
void list_print(LIST *list)
{
    if(list != NULL) {
        while(list != NULL) {
	    printf("%s", list->string);
	    if(list->next != NULL) {
	        printf(" -> ");
            }
	    list	= list->next;
        }
	printf("\n");
    }
}
