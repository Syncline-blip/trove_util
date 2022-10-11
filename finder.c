#define _DEFAULT_SOURCE
#include "finder.h"
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

typedef struct {
    char fileName[100];
    char filePath[1000];
}Files;

static Files filesQ[1000]; //Perhaps need malloc()
static int forks[1024];
int position = 0;


int isDirectory(char *path)
{
    struct stat statbuf;
    if(stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
    {
        return 1;
    }
    return 0;
}

//Searches through all directories and sub-directories, storing all files found.
void list_directory(char *dirname)        
{   
    DIR             *dirp;
    struct dirent   *dp;
    dirp       = opendir(dirname);
    int size = 1024;

    int forkCount = 0;
    if(dirp == NULL) {
        perror( dirname );
        exit(EXIT_FAILURE);
    }


    while((dp = readdir(dirp)) != NULL) 
    {
        if((isDirectory(dp->d_name) != 0) && (strcmp(dp->d_name, ".") != 0) && (strcmp(dp->d_name, "..") != 0)) 
        {
            int proccessID = fork(); // Create a new proccess
            /*
            * If proccessID == 0 (child proccess, then show path, recurse and dig, if parent then count running childs, then wait to finish)
            */
            if(proccessID == 0)
            {
                forkCount++;
                char *path = malloc(sizeof(*realpath(dp->d_name, path)) * (size+1)); // allocate memory for the current path to look for.
                realpath(dp->d_name, path); 
                printf("dir to search -> '%s'\n", path);
                printf("Searching %s...\n",dp->d_name);
                list_directory(path);
                free(path); // Free after child is done with the proccess.
                exit(0); // Exit after finish
            }
            else
            {
                forks[forkCount] = proccessID; // May not need a malloc array for this, as we only need it to keep track of proccesses and forkcounts
                int status = 0;
                wait(&status); // put parent proccess to sleep, wait for child process to finish.

            }  
        }
        else if(strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            realpath(filesQ[position].filePath, dp->d_name);
            strcpy(filesQ[position].fileName, dp->d_name);
            printf("file: %s\n", filesQ[position].fileName);
            
            ++position;
        }
        
    }


    closedir(dirp);

}


void pathFinder(char path[])
{
 // i think '-r' will require this to see if a path in the filelist is in the trove-file 
  
  //If a matching path is found, remove it from the trove-file
  
}