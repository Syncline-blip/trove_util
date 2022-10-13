// trove main source code
#include "trove.h"
#include "finder.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h> 
#include <getopt.h>
#define DEFAULT_DIR "/tmp/trove"
#define DEFAULT_LEN 4
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
    
        readTrovefile(argv[1], argv[2]);
    
    int options, inputLength;
    
        while (optind <argc)
        {
            
            if ((options = getopt(argc,argv, "f:brul:")) != -1)
            {
                    switch(options)
                    {
                        case 'f': {
                            
                            if(optarg == NULL)
                            {
                                printf("hentai : %s", optarg);
                                exit(EXIT_FAILURE);
                            }
                            printf("file name: %s\n", optarg);
                            break;
                        }
                        case 'b':
                            printf("case b success\n");
                            break;
                        case 'r':
                            printf("case r success\n");
                            break;
                        case 'u':
                            printf("case u success\n");
                            break;
                        case 'l':
                                            {
                            inputLength = (int)atol(optarg);
                            if(inputLength == 0)
                            {
                                printf("-l error: length cannot 0\n");
                                exit(EXIT_FAILURE);
                            }
                            
                            printf("length spec:     %d\n", inputLength);   
                            break;
                        }
                        default:
                            usage();
                            exit(EXIT_FAILURE);
                            break;
                    }
            }
            else
            {
                if(argc == 2)
                {
                    printf("%s", argv[1]);
                }
                else if(argc == 4)
                {
                    printf("%s", argv[3]);
                }
                optind++;
            }
        }
        
    

    
    
    // }
    
    // list_directory(argv[1]);
    // printf("\n");
    // printf("Test if word 'HASHTABLE_SIZE' exists in hashtable.c\n");

}
