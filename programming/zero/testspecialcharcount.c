/* This program is to test specialcharcount function.
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

void displayspecialcharfreq(long charfreq[], int size)
{	
	for(int i = 0; i < size; i++)
	  {
	  	switch(i)
	  	{
	  		case 0:
	  			printf(", -> %ld\n", charfreq[i]);
	  			break;
	  		case 1:
	  			printf(". -> %ld\n", charfreq[i]);
	  			break;
	  		case 2:
	  			printf(": -> %ld\n", charfreq[i]);
	  			break;
	  		case 3:
	  			printf("; -> %ld\n", charfreq[i]);
	  			break;
	  		case 4:
	  			printf("! -> %ld\n", charfreq[i]);
	  			break;
	  	}	  	
	  }
}

int main()
{
	  char *path = "../data/";           // the path of the directory including the *.txt files to be processed
	  char *filetowrite = "../result/specialresult.txt";    // the frequency of all alphabetical letters will be written in this file
	  
	  long charfreq[SPECIALCHARSIZE] = {0}; // array to store the frequency of 5 special characters
	  
	  specialcharcount(path, filetowrite, charfreq); // count the frequency of special characters and write into file
	  
	  displayspecialcharfreq(charfreq, SPECIALCHARSIZE); // print the array to output screen	  
}
