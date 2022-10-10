<<<<<<< HEAD
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

typedef struct {
    char fileName[100];
    char filePath[1000];
}Files;
=======

#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "finder.h"
#include <sys/types.h>
#include <unistd.h>
#include  <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#include <limits.h> /* PATH_MAX */
// char findWord(char path[], char word[])
// {
//   char absPath ;
//   //if(isDirectory(&path) == 0)
//        {
>>>>>>> e2e55a03bca666492db64faa3f7aa2a22974742e

static Files filesQ[1000]; //Perhaps need malloc()
int position = 0;

// fix this
int isDirectory(char *path)
{
    struct stat statbuf;
    if(stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
    {
        return 1;
    }
    return 0;
<<<<<<< HEAD
    if(stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
    {
        return 1;
    }
    return 0;
=======
    if(stat(path, &statbuf) != 0)
    {
        return 0;
    }
    return S_ISDIR(statbuf.st_mode);
>>>>>>> e2e55a03bca666492db64faa3f7aa2a22974742e
}

//Searches through all directories and sub-directories, storing all files found.
void list_directory(char *dirname)        
{   
    DIR             *dirp;
    struct dirent   *dp;
    dirp       = opendir(dirname);

<<<<<<< HEAD
=======

>>>>>>> e2e55a03bca666492db64faa3f7aa2a22974742e
    if(dirp == NULL) {
        perror( dirname );
        exit(EXIT_FAILURE);
    }

<<<<<<< HEAD
    while((dp = readdir(dirp)) != NULL) {
=======
//  READ FROM THE REQUIRED DIRECTORY, UNTIL WE REACH ITS END
    while((dp = readdir(dirp)) != NULL) {  
>>>>>>> e2e55a03bca666492db64faa3f7aa2a22974742e
        
        if(isDirectory(dp->d_name) != 0)
        {
            char path[1000];
<<<<<<< HEAD
            if((strcmp(dp->d_name, ".") != 0) && (strcmp(dp->d_name, "..") != 0)) {
                realpath(dp->d_name, path);
                printf("dir to search -> '%s'\n", path);
                printf("Searching %s...\n",dp->d_name);
                list_directory(path);
=======
            if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
                printf( "dir: %s\n", dp->d_name);
                realpath(dp->d_name, path);
                printf("Path: %s\n", path);
            //char* newPath = (char * )malloc(strlen(dirname) + strlen(dp->d_name)+1);
            
                list_directory(path);
            // free(newPath);
>>>>>>> e2e55a03bca666492db64faa3f7aa2a22974742e
            }
        }
        else
        {
<<<<<<< HEAD
            realpath(filesQ[position].filePath, dp->d_name);
            strcpy(filesQ[position].fileName, dp->d_name);
            printf("file: %s\n", filesQ[position].fileName);
            ++position;
        }

=======
            printf( "file: %s\n", dp->d_name );
        }
       
        
    
        
>>>>>>> e2e55a03bca666492db64faa3f7aa2a22974742e
    }

    closedir(dirp);
}




void pathFinder(char path[])
{
 // i think '-r' will require this to see if a path in the filelist is in the trove-file 
  
  //If a matching path is found, remove it from the trove-file
  
}