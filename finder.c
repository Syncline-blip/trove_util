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
#include <dirent.h>
#include <errno.h>
#include <limits.h>
 #include <sys/wait.h>

typedef struct {
    char fileName[100];
    char filePath[1000];
}Files;

static Files filesQ[1000]; //Perhaps need malloc()
int listOfPIDS[1000];
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
    int forkCount= 0;
    if(dirp == NULL) {
        perror( dirname );
        exit(EXIT_FAILURE);
    }
   

    while((dp = readdir(dirp)) != NULL) {

           
           
       
                if((isDirectory(dp->d_name) != 0) && (strcmp(dp->d_name, ".") != 0) && (strcmp(dp->d_name, "..") != 0)) {
                    int PID = fork();
                    if(PID == 0)
                    {   
                        forkCount++;
                        char path[1000];
                        realpath(dp->d_name, path);
                        printf("dir to search -> '%s'\n", path);
                        printf("Searching %s...\n",dp->d_name);
                        list_directory(path);
                        exit(0); // If child proccess is succesfull, it shud exit
                    }
                    else
                    {
                        // waits until it finishes
                        
                        listOfPIDS[forkCount] = PID;
                        int status = 0;
                        wait(&status);
                    }
                }
                else
                {
                    realpath(filesQ[position].filePath, dp->d_name);
                    strcpy(filesQ[position].fileName, dp->d_name);
                    printf("file: %s\n", filesQ[position].fileName);
                    ++position;


                }


    }

    closedir(dirp);
}

// void list_directory(char *dirname)        
// {   
//    struct dirent *currentDir;  
//    int forkCount = 0;
//     DIR *traverser = opendir(dirname);

//     if (traverser == NULL)
//     {
//         printf("Error: Could not open directory.\n");

//     }

//     while ((currentDir = readdir(traverser)) != NULL)
//     {       
//         if (isDirectory(currentDir->d_name) != 0 && strcmp(currentDir->d_name, ".") != 0 && strcmp(currentDir->d_name, "..") != 0)
//         {
//             int PID = fork();

//             if (PID == 0)
//             {
//                 forkCount++;        
//                 char pathBuffer[1024];
                
//                 list_directory(pathBuffer);
//                 exit(0);
//             }
//             else
//             {
//                 forkCount = PID;
//                 int status = 0;
//                 wait(&status); //wait at end
//             }
//         }
//         else if (strcmp(currentDir->d_name, ".") != 0 && strcmp(currentDir->d_name, "..") != 0)
//         {
//             printf("%s: Proc'd by %d\n", currentDir->d_name, getpid());
//         }
//     }

//     closedir(traverser);

    

// }

void pathFinder(char path[])
{
 // i think '-r' will require this to see if a path in the filelist is in the trove-file 
  
  //If a matching path is found, remove it from the trove-file
  
}