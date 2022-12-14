#ifndef FILEIO_H
#define FILEIO_H
#include "hashtable.h"

void add_files(char *pathname);
int stringDigger(char* fName, char* sWord);
int isDirectory(char *path);
void readTrovefile(char trovefile[], char* word);
int isFile(char *input);
int fileExists(char *fName);
int traverse(char *dirname);
void writeFile();
void removeFiles(char *fileName);
void populate(char *file,char *key);
void setName(char *name);
int checkString(char str1[], int size);
int checkGZ(char fileToCheck[]);
char *decompress(char *fileToOpen);
int fileCheck(char fileToCheck[]);
#endif