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
#include "linkedlist.h"
#include "fileio.h"

typedef struct {
    char fileName[100];
    char filePath[DEFAULT_SIZE];
}Files;

typedef struct 
{
    /* data */
    char* filePath;
}fileStruct;

static int forks[DEFAULT_SIZE];
linkedlist* dirList;

// Looks for the given source
int stringDigger(char *fName, char *sWord)
{   
    char **found;
    glob_t gstruct;
    int r;
    r = glob(fName, GLOB_ERR , NULL, &gstruct); // Need to look for an exact match

    if(r!= 0)
    {
        if(r==GLOB_NOMATCH)
        {   
           printf("Could not find '%s'\n", fName);
        }
    }
    int existValue = 0; // 0 false : 1 true
    found = gstruct.gl_pathv;
    if(gstruct.gl_pathc == 0)
    {
        exit(EXIT_FAILURE);
    }
    
    printf("%s\n",strrchr(*found, '/'));

    FILE* fp = fopen(*found, "r");
    while (fgets(*found,DEFAULT_SIZE,fp) != NULL)
    {
        char *ptr = strstr(*found, sWord);
        if(ptr != NULL)
        {
            existValue = 1;
            insertDirectory(dirList,fName);
            fclose(fp);
            break; //We can remove this if we want it to keep searching the file for numerous word occurances.

        }
    }
    fclose(fp);
    found++;
    return existValue;
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

void insertDirectory(linkedlist* dirList, char* absPath)
{
    fileStruct* file = (fileStruct*)malloc(sizeof(fileStruct));
    file->filePath = absPath;
    printf("absPath: %s\n",file->filePath);
    insertFirst(dirList, file);
}

void createIndexFile(linkedlist* dirlist)
{
    FILE* file = fopen("newTrove", "a");
    if(file == NULL)
    {
        exit(EXIT_FAILURE);
    }
    else
    {
        listnode* node = dirlist->head;
        fileStruct* fileStructure;
        while(node != NULL)
        {
            fileStructure = (fileStruct*)node->value;
            fprintf(file,"%s\n",fileStructure->filePath);
            printf("%s\n",fileStructure->filePath);
            node = node->next;
        }
        fclose(file);
        printf("Index file has been written.\n");
        free(dirlist);
    }
}

//Checks if arg is a file.
int isFile(char *input)
{
    struct stat statbuf;
    stat(input, &statbuf);
    return S_ISREG(statbuf.st_mode);
}

//Searches through all directories and sub-directories, storing all files found.
void list_directory(char *dirname)        
{   
    Files *filesQ = malloc(sizeof(Files) * DEFAULT_SIZE);
    DIR *dirp;
    struct dirent *dp;
    dirp = opendir(dirname);
    int position = 0;
    int forkCount = 0;
    if(dirp == NULL) {
        perror( dirname );
        exit(EXIT_FAILURE);
    }
    
    while((dp = readdir(dirp)) != NULL) 
    {
        if((isDirectory(dp->d_name) != 0) && (strcmp(dp->d_name, ".") != 0) && (strcmp(dp->d_name, "..") != 0)) 
        {
            char *path = malloc(sizeof(*realpath(dp->d_name, path)) * (DEFAULT_SIZE+1)); // allocate memory for the current path to look for.
            int proccessID = fork(); // Create a new proccess
            /*
            * If proccessID == 0,(child proccess) then show path, recurse and dig, if parent then count running childs, then wait to finish
            */
            if(proccessID == 0)
            {
                forkCount++;
                
                realpath(dp->d_name, path); 
                //printf("dir to search -> '%s'\n", path);
                //printf("Searching %s...\n",dp->d_name);
                list_directory(path);
                
                exit(0); // Exit after finish
            }
            else
            {
                free(path); // Free after child is done with the proccess.
                forks[forkCount] = proccessID; // May not need a malloc array for this, as we only need it to keep track of proccesses and forkcounts
                int status = 0;
                wait(&status); //put parent proccess to sleep, wait for child process to finish.

            }  
        }
        else if(strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            realpath(dp->d_name,filesQ[position].filePath);
            strcpy(filesQ[position].fileName, dp->d_name);
            ++position;
        }
        
    }
    free(filesQ);
    closedir(dirp);
}

void readTrovefile(char trovefile[], char* word)
{
    FILE* fp = fopen(trovefile, "rbw+");
    int bufLen = 1024;
    char line[bufLen];
    dirList = newlist();
    
    while(fgets(line, bufLen, fp) != NULL)
    {
        line[strcspn(line, "\r\n")] = 0; //Removes any trailing '\n'
        if(stringDigger(line, word) == 1 && fileExists(line))//Word was found in file
        {
            printf("- Found '%s' in %s\n\n",word,strrchr(line,'/'));
            continue;
            //Move onto next path in file and don't remove from trovefile.
        }
        else//File no longer exists or doesn't contain the word anymore.
        {
            printf("- '%s' NOT Found in %s\n\n",word,strrchr(line,'/'));
        }
    }

    fclose(fp);
    createIndexFile(dirList);
    printf("Program finished executing.\n");
    exit(EXIT_SUCCESS);
}
