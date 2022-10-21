//  CITS2002 Project 2 2022
//  Student1:   22957291   Smith   Owen   50%
//  Student2:   23419439   Lumagbas   John   50%
#include <stdio.h>
#include <stdlib.h>
#include "fileio.h"
#define DEFAULT_TROVE_DIR "/tmp/trove"
#define DEFAULT_LEN 4
#include <unistd.h> 
#include <getopt.h>
#include <stdbool.h>
#include <string.h>
#define OPTLIST "f:brul:" //COMMAND LINE OPTIONS

int size;//Stores the size of the -l option

 char *parsedFiles[10000];

  //Parses the filelist data into main.c, where it is checked for a directory or file.
  void get_files(int index)
  {
      for(int i = 0; i < index; i++)
      {
          //printf("\tIn get_files() | '%s'\n", parsedFiles[i]);
          traverse(parsedFiles[i]);
      }
    
    //printf("exit(EXIT_SUCCESS)\n");
    exit(EXIT_SUCCESS);
  }

//Sets the size variable from the -l value.
void setSize(int inputLength)
{
    size = inputLength;
}

//Returns the size variable which stores the -l value.
int getSize()
{
    return size;
}

//Prints the Usage details when an incorrect command line argument is parsed, calls exit().
void usage()
{
    printf("Usage:  \t./trove [-f filename] [-b | -r | -u] [-l length] filelist\n\tor\t./trove [-f filename] word\n");
    exit(EXIT_FAILURE);
}


// Recieves the arguments from the command line, checks the options and calls functions to perform 
// tasks based on the options provided. 
int main(int argc, char *argv[])
{
    int options, index = 0;
    int inputLength = DEFAULT_LEN ;
    bool fFlag = false, bFlag = false, uFlag = false, rFlag = false, lFlag = false;
    char *fileName = DEFAULT_TROVE_DIR;
    //Checks all of the options provided.
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
        }else
        {
            //Extra arguments provided that aren't options such as word or filelist.
            parsedFiles[index] = argv[optind];
            index++;
            optind++; //If is not an option argument we move to the next argument.
        }
    }
    //printf("\nargc: %d\n",argc);
    //printf("FLAGS INVOKED:\n-f %d\n-b %d\n-u %d\n-r %d\n-l %d\n\n",fFlag, bFlag, uFlag, rFlag, lFlag);

    //Checks options for argc == 2 and calls the correct functions.
    if(argc == 2)
    {   
        //Invalid format of ./trove -b || ./trove -u || ./trove -r
        if(bFlag || uFlag || rFlag) 
        {
            usage();
        }
        //format: ./trove word
        else
        {
            //printf("Format: ./trove %s | %s\n", argv[1],fileName);
            readTrovefile(fileName,argv[1]);
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
            if(checkGZ(fileName) != 1)
            {
                readTrovefile(decompress(fileName), argv[3]);
            }else
            {
                readTrovefile(fileName, argv[3]);
            }
            exit(EXIT_SUCCESS);
        }
        //format: ./trove word [-f trovefile]
        else if(isFile(argv[3]))
        {
            if(checkGZ(fileName) != 1)
            {
                readTrovefile(decompress(fileName), argv[1]);
            }else
            {
                readTrovefile(fileName, argv[1]);
            }
            exit(EXIT_SUCCESS);
        }
        else
        {
            usage();
        }
    }else if(bFlag && parsedFiles[0] != NULL) //at least one file to index was parsed into the program.
    {
        setName(fileName);
        setSize(inputLength);
        get_files(index);
        //build a new trove-file from the contents of a filelist.
        //The new trove-file will replace an existing trove-file of the same name.
    }
    else if(rFlag && parsedFiles[0] != NULL)
    {
        char *key = "1";
        printf("-r flag\n");
        for(int i = 0; i < index; i++)
        {
            printf("\tIn get_files() | '%s'\n", parsedFiles[i]);
            populate(parsedFiles[i], key);
            key++;
        }
        //removeFiles(fileName);
        
        /* NOT BUILT :( */

        exit(EXIT_SUCCESS);
    }
    else if(uFlag && parsedFiles[0] != NULL)
    {
        for(int i = 0; i < index; i++)
        {
          printf("\t-u invoked | '%s'\n", parsedFiles[i]);
          traverse(parsedFiles[i]);
        }

        /* NOT BUILT :( */

        //update trovefile.
        // -u                update the trove-file with the contents of all files in the filelist.
        //            If the information from any of the files already exists in the trove-file,
        //            then that (old) information is first removed.

        exit(EXIT_SUCCESS);
    }
    else
    {
        usage();
    }

    return 0;
}