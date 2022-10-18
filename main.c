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

char *files[1024];

void usage()
{
    printf("Usage:  \t./trove [-f filename] [-b | -r | -u] [-l length] filelist\n\tor\t./trove [-f filename] word\n");
    exit(EXIT_FAILURE);
}

void fileNotFound(char *fileName)
{
    printf("ERROR: File '%s' not found\n",fileName);
    exit(EXIT_FAILURE);
}

void trovefileNotFound()
{
    printf("No trove file found.\n");
    exit(EXIT_FAILURE);
}

void getFiles(int argc, char *args[], char *trovefileName)
{
    //Files from argv that're not the trove file in fileName
    for(int i = 1; i < argc; i++)
    {
        //seems to sometimes say yes sometimes no for the same argument.
        if((isFile(args[i]) || isDirectory(args[i])) && strcmp(args[i], trovefileName) != 0)
        {
            files[i] = args[i];
        }
    }
}

int main(int argc, char *argv[])
{
    int options, inputLength = 4;
    bool fFlag = false, bFlag = false, uFlag = false, rFlag = false, lFlag = false;
    char *fileName = DEFAULT_TROVE_DIR;
    while (optind < argc){
        if((options = getopt(argc,argv, OPTLIST))  != -1)
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
                        break;
                }
        }else{optind++;}
    }
    // argc  += optind;
    // argv  -= optind;
    
    printf("\nargc: %d\n",argc);
    printf("FLAGS INVOKED:\n-f %d\n-b %d\n-u %d\n-r %d\n-l %d\n\n",fFlag, bFlag, uFlag, rFlag, lFlag);

    if(argc == 2)
    {
        if(bFlag || uFlag || rFlag) //Invalid format of ./trove -b || ./trove -u || ./trove -r
        {
            usage();
        }
        //format of ./trove fileName
        else if(fileExists(argv[1]))
        {
            //Call functions to handle
            printf("- FEATURE NOT BUILT YET -\n");
        }
        else if(fileExists(fileName))
        {   //format: ./trove word
            readTrovefile(fileName,argv[1]);
        }
        else
        {
            trovefileNotFound();
        }
    } 
    //In this sitation the input would be './trove -f trovefile' (which is invalid).
    else if(argc == 3 && fFlag)
    {
        usage(); 
    } 
    //Format: ./trove -f trovefile 'word' || ./trove 'word' -f trovefile (is the only 2 options for this)
    else if(argc == 4 && fFlag)
    {
        //format: ./trove [-f trovefile] word
        if(isFile(argv[2]))
        {
            printf("Format: ./trove [-f %s] %s\n", argv[2], argv[3]);
            readTrovefile(fileName, argv[3]);
        }
        //format: ./trove word [-f trovefile]
        else if(isFile(argv[3]))
        {
            printf("Format: ./trove [-f %s] %s\n", argv[3], argv[1]);
            readTrovefile(fileName, argv[1]);
        }
        //else there's something dodgy with the arguments. 
        else
        {
            usage();
        }
    }else if(bFlag)
    {
        //build a new trove-file from the contents of a filelist. 
        //The new trove-file will replace an existing trove-file of the same name.
    }
    else if(rFlag)
    {
        //if any of the files from the filelist appear in the trove-file, 
        //            remove all of their information from the trove-file.
    }
    else if(uFlag)
    {
        //update trovefile.
        // -u	            update the trove-file with the contents of all files in the filelist. 
        //            If the information from any of the files already exists in the trove-file, 
        //            then that (old) information is first removed. 
    
    
    }
    else
    {
        //Loop through argv and get files from the given 'filelist'
        getFiles(argc, argv, fileName);

    }


/*
    -b	            build a new trove-file from the contents of a filelist. 
                    The new trove-file will replace an existing trove-file of the same name.

    -f trovefile	provide the name of the trove-file to be built or searched. 
                    If the -f option is not provided, the default name of the trove-file is /tmp/trove

    -l length	    only words of at least the indicated length should be added to the trove-file. 
                    length must be a positive integer, for example: -l 6.
                    If the -l option is not provided, the default length is 4.

    -r	            if any of the files from the filelist appear in the trove-file, 
                    remove all of their information from the trove-file.

    -u	            update the trove-file with the contents of all files in the filelist. 
                    If the information from any of the files already exists in the trove-file, 
                    then that (old) information is first removed. 
*/

    return 0;
}
