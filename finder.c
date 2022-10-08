
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

//        }
//   // If is directory -> open and traverse subdirectory
//   // else -> read file and see if the word is contained in the file.
  
//   //return the realpath() to add to trove-file
//   return absPath;
// }

// fix this
int isDirectory(char *path)
{
    struct stat statbuf;
    if(stat(path, &statbuf) != 0)
    {
        return 0;
    }
    return S_ISDIR(statbuf.st_mode);
}


//Used to find words in a provided path (file or directory) match word length constraint.
// char* wordLength(char path[], int length)
// {
//   //Check if the path is a file or directory
//   // If is directory -> open and traverse subdirectory
//   //      Found words need to be 'attached' to the path so it can all be deleted if needed.
//   // else -> read file and see if there are words matching supplied length and add to an array.
  
//   //return array of words to add to trove-file
  
// }

//From workshop -> need to change to suit our situation
void list_directory(char *dirname)        
{   
    DIR             *dirp;
    struct dirent   *dp;

//  ENSURE THAT WE CAN OPEN (read-only) THE REQUIRED DIRECTORY
    dirp       = opendir(dirname);


    if(dirp == NULL) {
        perror( dirname );
        exit(EXIT_FAILURE);
    }
    char path[1000];

//  READ FROM THE REQUIRED DIRECTORY, UNTIL WE REACH ITS END
    while((dp = readdir(dirp)) != NULL) {  
        
        if(isDirectory(dp->d_name) != 0)
        {
            char*path = (char*)malloc(strlen(dirname) + strlen(dp->d_name)+1);
            if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
                printf( "   dir: %s\n", dp->d_name);
                realpath(dp->d_name, path);
                printf("    Path: %s\n", path);
            
            
                list_directory(path);
            free(newPath);
            }
           
        }
        else
        {    

            realpath(dp->d_name, path);
            printf( "file: %s\n", path);
        }
       
        
    
        
    }

//  CLOSE THE DIRECTORY
    closedir(dirp);
}




void pathFinder(char path[])
{
 // i think '-r' will require this to see if a path in the filelist is in the trove-file 
  
  //If a matching path is found, remove it from the trove-file
  
}

