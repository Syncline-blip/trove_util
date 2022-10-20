#ifndef HASHTABLE_H
#define HASHTABLE_H

// Struct for the item, its hashkey and actual value.
typedef struct HashItem 
{
    char* key;
    char* value;

} HashItem;

// Struct for the table
typedef struct HashTable 
{
    
    HashItem** hashItem;
    int tableSize;
    int itemCount;
    
}HashTable;

unsigned long hash(char* value);
HashTable*setTable(int tableSize);
HashItem* setItem(char*key, char*value);
void freeItem(HashItem* item);
void freeTable(HashTable* table);
void insertItem(HashTable*table, char* inKey, char* inValue);
char* itemSearch(HashTable* table, char*inKey);
void printSearch(HashTable* table, char*key, char*writeTo);
void print_table(HashTable* table);
#endif