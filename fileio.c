#define DEFAULT_SIZE 1024
#define _DEFAULT_SOURCE
#define _GNU_SOURCE 
#define DELIMS  "-`~!@#$%^&*();[]\{}}|<>?"
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
#include <sys/wait.h>
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h> 
#include <glob.h>
#include "linkedlist.h"
#include "fileio.h"

typedef struct {
    char fileName[100];
    char filePath[DEFAULT_SIZE];
}Files;

// typedef struct 
// {
//     /* data */
//     char* filePath;
// }fileStruct;

static int forks[DEFAULT_SIZE];
linkedlist* dirList;
char *all_files[1024];
linkedlist* wordList;
int location = 0;
int wordCount = 0;

void add_files(char *pathname)
{
    all_files[location] = pathname;
    printf("'%s' added to all_files[]. \n",all_files[location]);
    ++location;
}

void printFilelist()
{
    for(int i = 0; i < location; i++)
    {
        printf("%d | %s\n",i,all_files[i]);
    }
}

// Looks for the given source
int stringDigger(char *fName, char *sWord)
{   
    char **found;
    glob_t gstruct;
    int r;
    r = glob(fName, GLOB_ERR , NULL, &gstruct); // Need to look for an exact match

    if(r!= 0)
    {
        if(r==GLOB_NOMATCH)
        {   
           printf("Could not find '%s'\n", fName);
        }
    }
    int existValue = 0; // 0 false : 1 true
    found = gstruct.gl_pathv;
    if(gstruct.gl_pathc == 0)
    {
        exit(EXIT_FAILURE);
    }
    
    printf("%s\n",strrchr(*found, '/'));

    FILE* fp = fopen(*found, "r");
    while (fgets(*found,DEFAULT_SIZE,fp) != NULL)
    {
        char *ptr = strstr(*found, sWord);
        if(ptr != NULL)
        {
            existValue = 1;
            insertDirectory(dirList,fName);
            break; //We can remove this if we want it to keep searching the file for numerous word occurances.

        }
    }
    fclose(fp);
    found++;
    return existValue;
}

int fileExists(char *fName)
{
    glob_t gstruct;
    int r;
    r = glob(fName, GLOB_ERR , NULL, &gstruct); // Need to look for an exact match

    if(r!= 0)
    {
        if(r==GLOB_NOMATCH)
        {
            return 0;
        }
    }
    
    return 1;
}

int isDirectory(char *path)
{
    struct stat statbuf;
    if(stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
    {
        return 1;
    }else
    {
        return 0;
    }
}

//Checks if arg is a file.
int isFile(char *input)
{
    struct stat statbuf;
    if (stat(input, &statbuf) != 0)
       return 0;
    return S_ISREG(statbuf.st_mode);
}

void insertDirectory(linkedlist* dirList, char* absPath)
{
    char* file = (char*)malloc(sizeof(char));
    file = absPath;
    printf("absPath: %s\n",absPath);
    insertFirst(dirList, file);
}

void printWords()
{
    printf("Inside printWords()\n");
    listnode* node = wordList->head;
    while(node != NULL)
    {
        char* path = (char*)node->value;
        printf("node-> %s \n",path);
        node = node->next;
    }
    printf("finished printing words\n");
}

void getWords(int wordLength)
{
    printf("Inside getWords()\n");
    char* pch;
    wordList = newlist();
    FILE* file = fopen("newTrove", "a");
    char line[DEFAULT_SIZE];
    while(fgets(line, DEFAULT_SIZE, file) != NULL)
    {
        pch = strtok(line, DELIMS);
        while(pch != NULL)
        {
            if(strlen(pch) == wordLength){
                insertFirst(wordList, pch);
                wordCount++;
            }
            pch = strtok(line, DELIMS);
        }
        
    }
    printWords();
}

void buildNewTrovefile(char* fileName,int inputLength)
{
    printf("Inside buildNewTrovefile()\n");
    remove(fileName);
    FILE* file = fopen(fileName, "w");
    if(file == NULL)
    {
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < location; i++)
    {
        //Puts all pathnames into file.
        fprintf(file,"%s\n",all_files[location]);
    }
    //Puts all words into file.
    getWords(inputLength);
    fclose(file);
    
}

void createIndexFile(linkedlist* dirlist)
{
    FILE* file = fopen("newTrove", "a");
    if(file == NULL)
    {
        exit(EXIT_FAILURE);
    }
    else
    {
        listnode* node = dirlist->head;
        while(node != NULL)
        {
            char* path = (char*)node->value;
            printf("node-> %s \n",path);
            fprintf(file,"%s\n",path);
            node = node->next;
            
        }
        fclose(file);
        printf("Index file has been written.\n");
        free(dirlist);
    }
}

void buildTrovefile(char* filename, char* path, int length)
{
    FILE* file = fopen("running_tempfile", "w");
    if(file == NULL)
    {
        printf("Null file.\n");
        return;
    }
    else
    {   
        fprintf(file,"%s\n",path);
        printf("%s\n",path);

        fclose(file);
        printf("Index file has been written.\n");
    }
}


//Searches through all directories and sub-directories, storing all files found.
void get_filelist(char *dirname)        
{   
    //printf("\tIn get_filelist() | %s\n",dirname);
    DIR *dirp;
    struct dirent *dp;
    dirp = opendir(dirname);
    int position = 0;
    int forkCount = 0;
    // if(dirp == NULL && !isFile(dirname)) {
    //     //printf("\t\t'%s' not found.\n",dirname);
    //     printf("\t\t\t - Yh not a directory or doesn't exist bossman\n");
    //     return;
    //     //perror( dirname );
    //     //exit(EXIT_FAILURE);
    // }else if(isFile(dirname))
    // {
    //     printf("Here %s\n",dirname);
    // }
    printf("dirname: %s | open: %d\n",dirname, dirp!=NULL);

    if(dirp != NULL){
        while((dp = readdir(dirp)) != NULL) 
        {
        char *pathTemp = malloc(sizeof(*realpath(dp->d_name, pathTemp)) * (DEFAULT_SIZE+1)); // allocate memory for the current path to look for.
        realpath(dp->d_name,pathTemp);
        //printf("Step 1. | %s | %d \n",dp->d_name,isDirectory(dp->d_name));
        printf("pathTemp: %s | %s | isFile() %d | isDir() %d\n",pathTemp,dp->d_name,isFile(pathTemp),isDirectory(pathTemp));

        if(isDirectory(pathTemp) && strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            puts("Step 2.");
            printf("* * '%s' is Directory * *\n",dp->d_name);
            int proccessID = fork(); // Create a new proccess
            /*
            * If proccessID == 0,(child proccess) then show path, recurse and dig, if parent then count running childs, then wait to finish
            */
            if(proccessID == 0)
            {
                puts("Step fork.");
                //printf("****** Forked ****** \n");
                forkCount++;
                //realpath(dp->d_name, path); 
                get_filelist(pathTemp);
                exit(0); // Exit after finish
            }
            else
            {
                puts("Step wait");
                free(pathTemp);
                forks[forkCount] = proccessID; // May not need a malloc array for this, as we only need it to keep track of proccesses and forkcounts
                int status = 0;
                wait(&status); //put parent proccess to sleep, wait for child process to finish.

            }  
        }
        else if(isFile(pathTemp) )      //strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            puts("\nStep final.");
            printf("realpath() %s | isDir() %d | isFile() %d\n\n",pathTemp, isDirectory(pathTemp), isFile(pathTemp));
            add_files(pathTemp);
            ++position;
        }
        
    }
    }
    else if(isFile(dirname))
    {
        char *path = malloc(sizeof(*realpath(dirname, path)) * (DEFAULT_SIZE+1));
        realpath(dirname,path);
        add_files(path);
        return;
    }
    else
    {
        printf("Error\n");
        exit(EXIT_FAILURE);
    }
    
    closedir(dirp);
}


void readTrovefile(char trovefile[], char* word)
{
    FILE* fp = fopen(trovefile, "rbw+");
    int bufLen = 1024;
    char line[bufLen];
    dirList = newlist();
    
    while(fgets(line, bufLen, fp) != NULL)
    {
        line[strcspn(line, "\r\n")] = 0; //Removes any trailing '\n'
        if(stringDigger(line, word) == 1 && fileExists(line))//Word was found in file
        {
            printf("- Found '%s' in %s\n\n",word,strrchr(line,'/'));
            //insertDirectory(dirList, line);
            continue;
            //Move onto next path in file and don't remove from trovefile.
        }
        else//File no longer exists or doesn't contain the word anymore.
        {
            printf("- '%s' NOT Found in %s\n\n",word,strrchr(line,'/'));
        }
    }

    fclose(fp);
    createIndexFile(dirList);
    printf("Program finished executing.\n");
    exit(EXIT_SUCCESS);
}
