/* This program is to test alphabetcount function.
 * 
 */

#include <stdio.h> 
#include <dirent.h> 
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h> 
#include <stdbool.h> 
#include "count.h"
 
void displayalphabetfreq(long charfreq[], int size)
{
	for(int i = 0; i < size; i++)
	  {
	  	printf("%c -> %ld\n", (char)(i+97), charfreq[i]);
	  }
}

int main()
{
	  
	  char *path = "../data/";           // the data *.txt files are under this folder
	  char *filetowrite = "../result/result.txt";    // the frequency of all alphabetical letters will be written in this file
	  
	  long alphabetfreq[ALPHABETSIZE] = {0}; // array to store the frequency of each alphablet letter
	  
	  
	  alphabetlettercount(path, filetowrite, alphabetfreq);  // process the data files
	  
	  displayalphabetfreq(alphabetfreq, ALPHABETSIZE);  // print the frequency stored in the array to output screen
}
