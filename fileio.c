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


typedef struct {
    char fileName[100];
    char filePath[DEFAULT_SIZE];
}Files;


static int forks[DEFAULT_SIZE];

// Looks for the given source
int stringDigger(char* fName, char* sWord)
{   
    char **found;
    glob_t gstruct;
    int r;
    int forkCount = 0;
    r = glob(fName, GLOB_ERR , NULL, &gstruct); // Need to look for an exact match

    if(r!= 0)
    {
        if(r==GLOB_NOMATCH)
        {
            perror(fName);
        }
    }
    // char line[1024];
    int existValue = 0; // 0 false : 1 true
    // char line[1024];
    found = gstruct.gl_pathv;
    if(gstruct.gl_pathc == 0)
    {
        exit(EXIT_FAILURE);
    }
    printf("Found %zu filename matches\n",gstruct.gl_pathc);
    while(*found)
    {
        printf("%s\n",*found);
        FILE* fp = fopen(*found, "r");
        int proccessID = fork();
        if(proccessID == 0)
        {
            while (fgets(*found,DEFAULT_SIZE,fp))
            {
                char *ptr = strstr(*found, sWord);
                if(ptr != NULL)
                {
                    existValue = 1;
                    printf("found!\n");
                    break;
                }
            }
            free(found);
            forkCount++;
            exit(0);
        }
        else
        {
            forks[forkCount] = proccessID; // May not need a malloc array for this, as we only need it to keep track of proccesses and forkcounts
            int status = 0;
            wait(&status); // put parent proccess to sleep, wait for child process to finish.

        }  
        fclose(fp);
        found++;
    }
    
    return existValue;
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


//Searches through all directories and sub-directories, storing all files found.
void list_directory(char *dirname)        
{   
    Files *filesQ = malloc(sizeof(Files) * DEFAULT_SIZE); //Perhaps need malloc

    DIR             *dirp;
    struct dirent   *dp;
    dirp       = opendir(dirname);
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
                wait(&status); // put parent proccess to sleep, wait for child process to finish.

            }  
        }
        else if(strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            realpath(dp->d_name,filesQ[position].filePath);
            strcpy(filesQ[position].fileName, dp->d_name);
            //printf("file: %s\n", filesQ[position].filePath);
            //searchString(filesQ[position].filePath, "hash"); test
            /// end
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

    printf("Boss im here in readTrovefile | '%s' | %d\n", trovefile, fp!=NULL);
    while(fgets(line, bufLen, fp))
    {
       

                if(stringDigger(line, word) == 1) //Word was found in file
                {
                    printf("found in this file");
                    continue; //Move onto next file and don't remove from trovefile.
                }
                else//File no longer exists or doesn't contain the word anymore.
                {
                    printf("Word not found -> delete line\n");
                    // fputs("Okay", fp);
                }
    }
    fclose(fp);
}