#include <stdio.h>                  // declares  sprintf()
#include <stdlib.h>                 // declares  atoi() and getenv()
#include <sys/time.h>               // declares  gettimeofday()

#include "support.h"

int     wordn   = 0;

void reset_words(void)
{
    char    *env    = getenv("WORD0");

    if(env != NULL) {
        wordn   = atoi(env);
    }
    else {
        wordn   = 0;
    }
}

char *next_word(void)
{
    static char word[50];

    sprintf(word, "%i+%i", wordn, wordn);
    ++wordn;
    return word;
}

long microseconds(void)
{
    struct timeval  t;

    gettimeofday( &t, NULL );       // timezone not required
    return (t.tv_sec * 1000000) + t.tv_usec;
}

