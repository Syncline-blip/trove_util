// trove main source code
#include "trove.h"
#include "finder.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>

// usage()
void usage()
{   
    printf("Standard usage:\n");
    printf("./trove [-f <file>] given-word\n");
    printf("--------------------------------\n");
    printf("Index Builder usage:\n");
    printf("./trove [-f <file>] [-b  |  -r  |  -u]  [-l length]  filelist]\n");
    printf("\n\t-b  build a new trove-file from the contents of a filelist");
    printf("\n\t-f  trovefile	provide the name of the trove-file to be built or searched.");
    printf("\n\t-l  length only words of at least the indicated length should be added to the trove-file.");
    printf("\n\t-r  if any of the files from the filelist appear in the trove-file, remove all of their information from the trove-file.");
    printf("\n\t-u	update the trove-file with the contents of all files in the filelist.\n");
}


int main(int argc, char *argv[])
{
    // if(argc != 3)
    // {
    //     printf("Invalid Argument Count:\n Usage:");
    //     usage();
    //     exit(EXIT_FAILURE);
    // }
    // int res = isDirectory(argv[1]);
    // if(res == 0)
    // {
    //     printf("\ninput is a directory\n");
    // }
    // else
    // {
    //     printf("\nis file\n");
    // }
    list_directory(argv[1]);
    printf("\n");
    printf("Test if word 'HASHTABLE_SIZE' exists in hashtable.c\n");
    if(searchString("/home/lazylime001/Desktop/My General Folder/trove/trove_util/hashtable.c", "FEF") == 1)
    {
        printf("found!\n");
    }
    

}
