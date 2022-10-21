#define DEFAULT_SIZE 1024
#define _DEFAULT_SOURCE
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/queue.h>
#include <sys/wait.h>
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <glob.h>
#include <ctype.h>
#include "fileio.h"
#include "HashTable.h"
#include "main.h"
#define DELIMS "-`~!@#$%^&*();[]\{}}|<>?.,_+=:'\t'\"'\n''//''/*''*\' "
typedef struct {
    char fileName[100];
    char filePath[DEFAULT_SIZE];
}Files;

typedef struct
{
    /* data */
    char* filePath;
}fileStruct;

// static Files filesQ[1000]; //Perhaps need malloc()
HashTable* ht;
int position = 0;
static int forks[DEFAULT_SIZE];
HashTable* files;
char *troveName;

void setName(char *name)
{
    troveName = name;
}
// Looks for the given source
int stringDigger(char *fName, char *sWord)
{
    size_t maxl = 256;
    //char line[256];
    char *line = malloc(maxl * sizeof(char));
     if(!line){
        printf("Memory not allocated!!\n");
        exit(EXIT_FAILURE);
    }
    FILE*file = fopen(fName, "rb");
    int existValue = 0;
    if(file == NULL)
    {
        return existValue;
    }
    while (fgets(line, maxl, file)) {
        line[strcspn(line, "\r\n")] = 0;
        while(line[strlen(line) - 1] != '\n' && line[strlen(line) - 1] != '\r' && !EOF){
            char *tmp = realloc (line, 2 * maxl * sizeof(char));

            fseek(file,0,SEEK_SET);          //or wherever you want to seek to
            if (tmp) {
                line = tmp;
                maxl *= 2;
                fgets(line, maxl, file);
            }
            else{
                printf("Not enough memory for this line!!\n");
                exit(EXIT_FAILURE);
            }
        }
        
        if(strstr(line, sWord) != NULL)
        {
            existValue = 1;
            printf("-> found in: %s\n", fName);
            break; //We can remove this if we want it to keep searching the file for numerous word occurances.

        }

        
    }
    fclose(file);

    return existValue;
}


void stringByLength(char *fName, int size)
{
    printf("stringByLength(%s,%d)\n",fName,size);
    size_t maxl = 256;
    //char line[256];
    char *line = malloc(maxl * sizeof(char));
     if(!line){
        printf("Memory not allocated!!\n");
        exit(EXIT_FAILURE);
    }
    printf("here(%s, %d)\n",fName,size);
    FILE*file = fopen(fName, "r");
    printf("here(%s, %d) | %d\n",fName,size, file==NULL);
    ht = setTable(50000);
    char* key = "1";
    while (fgets(line, maxl, file)) {
        printf("stringByLength(%s, %d) IM HERE\n",fName,size);
        line[strcspn(line, "\r\n")] = 0;
        printf("stringByLength(%s, %d)\n",fName,size);
        while(line[strlen(line) - 1] != '\n' && line[strlen(line) - 1] != '\r' && !EOF){
            char *tmp = realloc (line, 2 * maxl * sizeof(char));

            fseek(file,0,SEEK_SET);          //or wherever you want to seek to
            if (tmp) {
                line = tmp;
                maxl *= 2;
                fgets(line, maxl, file);
                
            }
            else{
                printf("Not enough memory for this line!!\n");
                exit(EXIT_FAILURE);
            }
        }
        
        // char word[100] = "";
        // int count = 0;
        // for(int i = 0; line[i] != 0; i++)
        // {
        //     if(count == 6 && !isalpha(line[i]) && !isdigit(line[i]))
        //     {
        //         insertItem(ht, key, word);
        //         count = 0;
        //     }
        //     if(isalpha(line[i]) || isdigit(line[i]))
        //     {
        //         printf("%d | %c\n",count,line[i]);
        //         strcat(word, &line[i]);
        //         count++;
        //     }else
        //     {
        //         count = 0;
        //     }
        // }
        printf("line: %s\n",line);
        for(char* p = strtok(line,DELIMS); p; p = strtok(NULL,DELIMS))
        {
            printf("p: %s\n",p);
            if(strlen(p) == size && !strstr(p,DELIMS))
            {
                if(itemSearch(ht,key) == NULL){
                    insertItem(ht, key, p);
                    printSearch(ht,key,troveName);
                    key++;
                }
               
            }
            
            }
        }
  
    fclose(file);
}


int fileExists(char *fName)
{
    glob_t gstruct;
    int r;
    r = glob(fName, GLOB_ERR , NULL, &gstruct); // Need to look for an exact match

    if(r!= 0)
    {
        if(r==GLOB_NOMATCH)
        {
            return 0;
        }
    }
    
    return 1;
}

int isDirectory(char *path)
{
    struct stat statbuf;
    if(stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
    {
        return 1;
    }
    return 0;
}



//Checks if arg is a file.
int isFile(char *input)
{
    struct stat statbuf;
    stat(input, &statbuf);
    return S_ISREG(statbuf.st_mode);
}


//Searches through all directories and sub-directories, storing all files found.
int traverse(char* directory)
{
    struct dirent *currentDir;
    DIR *traverser = opendir(directory);
    char *tmp = strrchr(directory, '/');
    tmp++;
    printf("tmp: %s\n", tmp);
    if(traverser == NULL)
    {
        FILE *fp = fopen(directory, "r");
        if(fp == NULL){
            printf("Error: Could not open directory: '%s'\n",directory);
            exit(0);
        }
        else if(strcmp(tmp, ".") != 0 && strcmp(tmp, "..") != 0){
            printf("trying to search: %s\n",directory);
            stringByLength(directory,getSize());
            return 1;
        }
    }
    int forkCount = 0;
    char buf[1024 + 1];
    while ((currentDir = readdir(traverser)) != NULL)
    {
        if (currentDir->d_type == DT_DIR && strcmp(currentDir->d_name, ".") != 0 && strcmp(currentDir->d_name, "..") != 0)
        {
            int PID = fork();

            if (PID == 0)
            {
                forkCount++;
                char pathBuffer[1024];
                snprintf(pathBuffer, sizeof(pathBuffer), "%s/%s", directory, currentDir->d_name);
                printf("traverse(%s)\n",pathBuffer);
                traverse(pathBuffer);
                exit(0);
            }
            else
            {
                forks[forkCount] = PID;
                int status = 0;
                wait(&status); //wait at end
            }
        }
        else if (strcmp(currentDir->d_name, ".") != 0 && strcmp(currentDir->d_name, "..") != 0)
        {
            //realpath(currentDir->d_name, buf);
            char pathBuffer[1024];
            snprintf(pathBuffer, sizeof(pathBuffer), "%s/%s", directory, currentDir->d_name);
            printf("currentDir->d_name: %s\nfilepath:%s\n",currentDir->d_name,buf);
            stringByLength(pathBuffer,getSize());
            //printf("file: %s \n", buf);
        }
    }

    closedir(traverser);
    return 0;
}

//Used for -r
void populate(char *file,char *key)
{
    printf("Populating ht -> %s\n",file);
    if(ht == NULL){
        ht = setTable(5000);
    }else{ insertItem(ht,key,file); }
    printf("Populated\n");
}

void writeFile(HashTable *table,char trovefile[])
{
    FILE *file = fopen(trovefile,"w");
    for (int i=0; i<table->tableSize; i++) {
        if (table->hashItem[i]) {
            fprintf(file,"%s\n",table->hashItem[i]->value);
        }
    }
}

// Reads through all files in the provided trovefile,
// and checks if the given word appears. If not, it is 
// removed from the file.
void readTrovefile(char trovefile[], char* word)
{
   size_t maxl = 256;
    
    char *line = malloc(maxl * sizeof(char));
    if(!line){
        printf("Memory not allocated!!\n");
        exit(EXIT_FAILURE);
    }
    FILE*file = fopen(trovefile, "r");
    char* key = "1";
    ht = setTable(50000);
    while (fgets(line, maxl, file)) {
        line[strcspn(line, "\r\n")] = 0;
        while(line[strlen(line) - 1] != '\n' && line[strlen(line) - 1] != '\r' && !EOF){
            
            char *tmp = realloc (line, 2 * maxl * sizeof(char));

            fseek(file,0,SEEK_SET);          //or wherever you want to seek to
            if (tmp) {
                line = tmp;
                maxl *= 2;
                fgets(line, maxl, file);
            }
            else{
                printf("Not enough memory for this line!!\n");
                exit(EXIT_FAILURE);
            }
        }
        if(stringDigger(line,word) == 1)
        {
            insertItem(ht, key, line);
            key++;
        }

    }
    
    remove(trovefile);
    writeFile(ht, trovefile);
    exit(EXIT_SUCCESS);
}

void removeFiles(char *fileName)
{
    HashTable *table = setTable(50000);
    FILE*file = fopen(fileName, "r");
    size_t maxl = 256;
    char* key = "1";
    char *line = malloc(maxl * sizeof(char));
    while (fgets(line, maxl, file)) 
    {
        line[strcspn(line, "\r\n")] = 0;
        for (int i=0; i<ht->tableSize; i++) 
        {
            if(strcmp(ht->hashItem[i]->value, line) == 0)
            {
                insertItem(table,key,line);
                key++;    
            }
        }
    }
    printf("Here\n");
    fclose(file);
    remove(fileName);
    writeFile(table, fileName);

}

// void updateTrovefile(char trovefile[])
// {
//    size_t maxl = 256;
    
//     char *line = malloc(maxl * sizeof(char));
//     if(!line){
//         printf("Memory not allocated!!\n");
//         exit(EXIT_FAILURE);
//     }
//     FILE*file = fopen(trovefile, "r");
//     char* key = "1";
//     ht = setTable(50000);
//     while (fgets(line, maxl, file)) {
//         line[strcspn(line, "\r\n")] = 0;
//         while(line[strlen(line) - 1] != '\n' && line[strlen(line) - 1] != '\r' && !EOF){
            
//             char *tmp = realloc (line, 2 * maxl * sizeof(char));

//             fseek(file,0,SEEK_SET);          //or wherever you want to seek to
//             if (tmp) {
//                 line = tmp;
//                 maxl *= 2;
//                 fgets(line, maxl, file);
//             }
//             else{
//                 printf("Not enough memory for this line!!\n");
//                 exit(EXIT_FAILURE);
//             }
//         }
//         if(stringDigger(line,word) == 1)
//         {
//             insertItem(ht, key, line);
//             key++;
//         }

//     }
    
//     remove(trovefile);
//     writeFile(trovefile);
//     exit(EXIT_SUCCESS);
// }