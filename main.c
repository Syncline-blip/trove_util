#include <stdio.h>
#include <stdlib.h>
#include "fileio.h"
#define DEFAULT_TROVE_DIR "tmp/trove"
#define DEFAULT_LEN 4
#include <unistd.h> 
#include <getopt.h>

void usage()
{
    //WIP
    printf("N/A");
}

int main(int argc, char *argv[])
{
    // stringDigger(argv[1], argv[2]);
    readTrovefile(argv[1], argv[2]);

    // Options currently broken
    // int options, inputLength;

    // while (optind <argc)
    // {

    //     if ((options = getopt(argc,argv, "f:brul:")) != -1)
    //     {
    //             switch(options)
    //             {
    //                 case 'f': {

    //                     if(optarg == NULL)
    //                     {
    //                         printf("hentai : %s", optarg);
    //                         exit(EXIT_FAILURE);
    //                     }
    //                     printf("file name: %s\n", optarg);
    //                     break;
    //                 }
    //                 case 'b':
    //                     printf("case b success\n");
    //                     break;
    //                 case 'r':
    //                     printf("case r success\n");
    //                     break;
    //                 case 'u':
    //                     printf("case u success\n");
    //                     break;
    //                 case 'l':
    //                                     {
    //                     inputLength = (int)atol(optarg);
    //                     if(inputLength == 0)
    //                     {
    //                         printf("-l error: length cannot 0\n");
    //                         exit(EXIT_FAILURE);
    //                     }

    //                     printf("length spec:     %d\n", inputLength);   
    //                     break;
    //                 }
    //                 default:
    //                     usage();
    //                     exit(EXIT_FAILURE);
    //                     break;
    //             }
    //     }
    //     else
    //     {
    //         if(argc == 2)
    //         {
    //             printf("%s", argv[1]);
    //         }
    //         else if(argc == 4)
    //         {
    //             printf("%s", argv[3]);
    //         }
    //         optind++;
    //     }
    // }
    return 0;
}
