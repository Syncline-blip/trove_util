//idkStoppedWorking
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
char *trovefileName;


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
            //printf("-> found in: %s\n", fName);
            break; //We can remove this if we want it to keep searching the file for numerous word occurances.

        }

        
    }
    fclose(file);

    return existValue;
}


void stringByLength(char *fName, int size)
{
    size_t maxl = 256;
    //char line[256];
    char *line = malloc(maxl * sizeof(char));
     if(!line){
        printf("Memory not allocated!!\n");
        exit(EXIT_FAILURE);
    }
    FILE*file = fopen(fName, "r");
    ht = setTable(50000);
    char* key = "1";
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
        for(char* p = strtok(line,DELIMS); p; p = strtok(NULL,DELIMS))
        {
            
            if(strlen(p) == size && !strstr(p,DELIMS))
            {
                if(itemSearch(ht,p) == NULL){
                    insertItem(ht, key, p);
                    //printf ("%s\n", p);
                    //printSearch(ht,key,fName);
                }
               
//                printf("Already in hashtable\n");
            }
            key++;
            
        }    
    }
        
    // WAS TRYING BETTER STRAT - BETTER TO SAY WHAT IS INCLUDED
    // RATHER THAN SAYING WHAT ISN'T.....
    //
    // int count = 0;
    // char *p = line;
    // while (p != NULL)
    // {
    //     if(count < size)
    //     {
    //         if ((line[count] >= 'A' && line[count] <= 'Z' )    ||
    //             (line[count] >= 'a' && line[count] <= 'z' )|| 
    //             (line[count] >= '0' && line[count] <= '9' )) 
    //             {
    //              count++;
    //             }
    //         else
    //         {
    //             count = 0;
    //             ++p;
    //         }
    //     }
    //     else{
    //         ++p;
    //         insertItem(ht, key, p);
    //         key++;
    //         }
    // }
    

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

void setTrovefileName(char *fileName){
    trovefileName = fileName;
}


//Searches through all directories and sub-directories, storing all files found.
int traverse(char* directory)
{
    struct dirent *currentDir;
    DIR *traverser = opendir(directory);

    if (traverser == NULL)
    {
        printf("Error: Could not open directory.\n");
        return 0;
    }
    int forkCount =0;
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
            realpath(currentDir->d_name, buf);
            stringByLength(currentDir->d_name,getSize());
            //writeFile(trovefileName);
        }
    }

    closedir(traverser);
    return 0;
}

void writeFile(char trovefile[])
{
    FILE *file = fopen(trovefile,"w");
    for (int i=0; i<ht->tableSize; i++) {
        if (ht->hashItem[i]) {
            fprintf(file,"%s\n",ht->hashItem[i]->value);
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
        while( !EOF){
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
    writeFile(trovefile);
    printf("File built.\nexit(EXIT_SUCCESS)\n");
    exit(EXIT_SUCCESS);
}
