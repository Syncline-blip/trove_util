#ifndef FILEIO_H
#define FILEIO_H
#include "HashTable.h"

void add_files(char *pathname);
int stringDigger(char* fName, char* sWord);
int isDirectory(char *path);
void readTrovefile(char trovefile[], char* word);
int isFile(char *input);
int fileExists(char *fName);
int traverse(char *dirname);
void writeFile();
#endif