#ifndef FILEIO_H
#define FILEIO_H
#include "linkedlist.h"
#include "hashtable.h"

void printFilelist();
void add_files(char *pathname);
void buildTrovefile(char* filename, char* path, int length);
void get_filelist(char *dirname);
int stringDigger(char* fName, char* sWord);
int isDirectory(char *path);
void readTrovefile(char trovefile[], char* word);
int isFile(char *input);
int fileExists(char *fName);
void createIndexFile(linkedlist* dirList);
void insertDirectory(linkedlist* dirList, char* absPath);
void buildNewTrovefile(char* fileName,int inputLength);
void getWords(int wordLength);
void printWords();
#endif