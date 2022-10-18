#ifndef FILEIO_H
#define FILEIO_H
#include "linkedlist.h"

void list_directory(char *dirname);
int stringDigger(char* fName, char* sWord);
int isDirectory(char *path);
void readTrovefile(char trovefile[], char* word);
int isFile(char *input);
int fileExists(char *fName);
void createIndexFile(linkedlist* dirList);
void insertDirectory(linkedlist* dirList, char* absPath);
#endif