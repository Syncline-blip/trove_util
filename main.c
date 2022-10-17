#include <stdio.h>
#include <stdlib.h>
#include "fileio.h"
#define DEFAULT_TROVE_DIR "tmp/trove"
#define DEFAULT_LEN 4
#include <unistd.h> 
#include <getopt.h>
#include <stdbool.h>
#include <string.h>

#define OPTLIST "f:brul:"

void usage()
{
    //WIP
    printf("USAGE ERROR\n");
    exit(EXIT_FAILURE);
}

void fileNotFound(char *fileName)
{
    printf("ERROR: File '%s' not found\n",fileName);
}


int main(int argc, char *argv[])
{
    int options, inputLength = 4;
    bool fFlag = false, bFlag = false, uFlag = false, rFlag = false, lFlag = false;
    char* fileName;
    //DOESN'T WORK WHERE OPTIONS ARE NOT NEXT TO EACH OTHER IT JUST ABORTS AT SOMETHING UNRECOGNISED
    while((options = getopt(argc,argv, OPTLIST))  != -1)
    {
            switch(options)
            {
                case 'f': 
                    fFlag = true;
                    fileName = strdup(optarg);
                    break;
                case 'b':
                    bFlag = true;
                    break;
                case 'r':
                    if(bFlag == true){ usage(); }
                    rFlag = true;
                    break;
                case 'u':
                    if(bFlag == true || rFlag == true){ usage(); }
                    uFlag = true;
                    break;
                case 'l':
                    inputLength = atoi(optarg); //changed from (int)atol(optarg) because atoi() will throw errors for non integer arguments.
                    if(inputLength == 0)
                    {
                        printf("-l error: invalid argument\n");
                        exit(EXIT_FAILURE);
                    }
                    lFlag = true;
                    break;
                default:
                    argc = -1;
            }
    }
    // argc  += optind;
    // argv  -= optind;
    
    printf("\nFLAGS INVOKED:\n-f %d\n-b %d\n-u %d\n-r %d\n-l %d\n\n",fFlag, bFlag, uFlag, rFlag, lFlag);


    //Format: ./trove -f trovefile 'word' || ./trove 'word' -f trovefile (is the only 2 options for this)
    if(argc == 4 && fFlag == true)
    {
        //If format == ./trove -f trovefile word
        if(isFile(argv[3]) == 0 && isDirectory(argv[3]) == 0)
        {
            printf("./trove -f %s %s\n", argv[3], argv[2]);
            readTrovefile(fileName, argv[3]);
        }
        //else if format == ./trove -f trovefile word
        else if(isFile(argv[1]) == 0 && isDirectory(argv[1]) == 0)
        {
            printf("./trove -f %s %s\n", argv[3], argv[1]);
            readTrovefile(argv[3], argv[1]);
        }
        //else there's something dodgy with the arguments. 
        else{ usage(); }
    }

    //In this sitation the input would be './trove -f trovefile'  - which is invalid.
    if(argc == 3 && fFlag == true)
    {
        usage(); 
    } 
    else if(argc == 2) //format of ./trove fileName -> is assumed its a filelist size 1 and checks for existence of a trovefile called "/tmp/trove"
    {
        if(fileExists(argv[1]) == 1)
        {
            //Call functions to handle
            printf("- FEATURE NOT BUILT YET -\n");
        }
        else{ fileNotFound(argv[1]); }
    }
    
    else {}
    

    return 0;
}
