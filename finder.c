

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#include "finder.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include  <dirent.h>

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

int isDirectory(char *path)
{
    struct stat statbuf;
    stat(path, &statbuf);
    return S_ISREG(statbuf.st_mode);
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

//  READ FROM THE REQUIRED DIRECTORY, UNTIL WE REACH ITS END
    while((dp = readdir(dirp)) != NULL) {  
        printf( "%s\n", dp->d_name );
    }

//  CLOSE THE DIRECTORY
    closedir(dirp);
}


void pathFinder(char path[])
{
 // i think '-r' will require this to see if a path in the filelist is in the trove-file 
  
  //If a matching path is found, remove it from the trove-file
  
}

