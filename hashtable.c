#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

#define DEF_CAP 39293


unsigned long hash(char* value)
{
    unsigned long i = 0;
    for (int j=0; value[j]; j++)
        i += value[j];
    return i % DEF_CAP;
    
}
// Create the table
HashTable*setTable(int tableSize)
{ 
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    table->tableSize = tableSize;
    table->itemCount = 0;
    table->hashItem = (HashItem**)calloc(table->tableSize, sizeof(HashItem*)); // Allocate multiple blocks of memory, since we dont know the size
    for(int i = 0; i<table->tableSize; i++)
    {
        table->hashItem[i] = NULL; // All null until it's overwritten.
    }
    return table;

}

//Create inItem
HashItem* setItem(char*key, char*value)
{
    HashItem* item = (HashItem*)malloc(sizeof(HashItem));
    item->key=(char*)malloc(strlen(key)+1);
    item->value=(char*)malloc(strlen(value)+1);

    strcpy(item->value, value);
    strcpy(item->key, key);

    return item;

}


// Free the shit man
void freeItem(HashItem* item)
{
    // Free the pointers then free the actual struct
    free(item->key);
    free(item->value);
    free(item);
}

void freeTable(HashTable* table)
{
    for(int i = 0; i<table->tableSize; i++)
    {
        HashItem* curItem = table->hashItem[i];
        if(curItem != NULL)
        {
            freeItem(curItem); // Free very single item in the table
        }

    }
    free(table->hashItem);
    free(table);
}

void insertItem(HashTable*table, char* inKey, char* inValue)
{
    HashItem* item = setItem(inKey, inValue);
    unsigned long hashIdx = hash(inKey);
    HashItem* curItem = table->hashItem[hashIdx];

    if(curItem == NULL)
    {
        // Check if the hash table is full
        if(table->itemCount == table->tableSize)
        {   
            freeItem(item);
            return;
        }
        // if condition is not true insert it
        table->hashItem[hashIdx] = item;
        table->itemCount++;
    }
    else
    {
        if(strcmp(curItem->key, inKey) == 0)
        {
            strcpy(table->hashItem[hashIdx]->value, inValue);
            return;
        }
        else
        {   
            unsigned long newHashIdx = hash(inKey)%table->tableSize;
            strcpy(table->hashItem[newHashIdx]->value, inValue);
        }
    }
}




char* itemSearch(HashTable* table, char*inKey)
{
    int index = hash(inKey);
    HashItem* item = table->hashItem[index];
     if (item != NULL) {
        if (strcmp(item->key, inKey) == 0)
            return item->value;
    }
    return NULL;
}



void printSearch(HashTable* table, char*key, char*writeTo)
{
    char* val;
    FILE* file = fopen(writeTo, "a");
    if(file == NULL)
    {
        exit(EXIT_FAILURE);
    }
    if ((val = itemSearch(table, key)) == NULL) {
        printf("Key:%s does not exist\n", key);
        return;
    }
    else {
 
        fprintf(file,"%s\n", val);

    }
}

void print_table(HashTable* table) {
    printf("\nHash Table\n-------------------\n");
    for (int i=0; i<table->tableSize; i++) {
        if (table->hashItem[i]) {
            printf("Index:%d, Key:%s, Value:%s\n", i, table->hashItem[i]->key, table->hashItem[i]->value);
        }
    }
    printf("-------------------\n\n");
}