#include <stdbool.h>

#ifndef FINDER_H
#define FINDER_H
#endif

// char findWord(char path[], char word[]);
// char* wordLength(char path[], int length);
void pathFinder(char path[]);
int isDirectory(char *path);
void list_directory(char *dirname);
<<<<<<< HEAD
int searchString(char* fileNmae, char* word);
void writeFile(char *path);
void readTrovefile(char trovefile[], char* word);
=======
bool searchString(char* fileNmae, char* word);
>>>>>>> c943859d5893dab75e0d256683486e50f9157ba3
