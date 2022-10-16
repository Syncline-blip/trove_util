#ifndef FILEIO_H
#define FILEIO_H
#endif

void list_directory(char *dirname);
int stringDigger(char* fName, char* sWord);
int isDirectory(char *path);
void readTrovefile(char trovefile[], char* word);
int isFile(char *input);
int fileExists(char *fName);