/* charcount.h - This header file include type definitions (including function prototypes) and macros 
   used for the programing assignment two.
*/

#include <stdio.h> 

#define ALPHABETSIZE 26 // The total number of alphabetical letter from a - z (case insensitive)
#define MAXFILENUM 100 // The maximum number of files the program can process

void alphabetcountmulthreads(char *path, char *filetowrite, long alphabetfreq[], int num_threads);

