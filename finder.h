#ifndef FINDER_H
#define FINDER_H
#endif

// char findWord(char path[], char word[]);
// char* wordLength(char path[], int length);
void pathFinder(char path[]);
int isDirectory(char *path);
void list_directory(char *dirname);
int searchString(char* fileNmae, char* word);
void writeFile(char *path);
void readTrovefile(char trovefile[], char* word);
