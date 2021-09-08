#include <stdio.h>
#include <stdlib.h>
#include "count.h"
#include <stdbool.h>
#include <string.h>
#include <dirent.h>

bool ends_with(char *str, char *sub){
    if (str == NULL && sub == NULL)
    {
        return true;
    }
    if (str == NULL || sub == NULL)
    {
        return false;
    }
    char *last_instance_of_sub = rindex(str, *sub); 
    int sub_len = strlen(sub);
    if (last_instance_of_sub == NULL || strlen(last_instance_of_sub) != sub_len)
    {
        return false;
    }
    return strncmp(last_instance_of_sub, sub, sub_len) == 0;
}

void specialcharcount(char *path, char *filetowrite, long charfreq[])
{
  	struct dirent * entry;
    DIR *d = opendir(path);
    if(d == 0) {
        perror("opendir");
        return;
    }
    int num_files = 0;

    // Find the files ending with .txt 
    while((entry = readdir(d)) != 0) {
        if(ends_with(entry->d_name, ".txt")) {
            num_files++;
        }
    }
    rewinddir(d);
    char *filelist[num_files];
    int i = 0;

    // Copy the files ending in .txt to an array filelist
    while((entry = readdir(d)) != 0) {  
        if(ends_with(entry->d_name, ".txt")) {
            filelist[i] = (char*) malloc (strlen(entry->d_name)+1);
            strncpy (filelist[i],entry->d_name, strlen(entry->d_name));
            i++;
        }
    }
    closedir(d);

    FILE *file;

    int sym_1 = 0;
    int sym_2 = 0;
    int sym_3 = 0;
    int sym_4 = 0;
    int sym_5 = 0;

    int c;

    i = 0;

    do {
        char *newpath = malloc(strlen(path) + strlen(filelist[i]) +1);
        strcpy(newpath, path);
        strcat(newpath, filelist[i]);
        file = fopen (newpath, "r");
        if(file == NULL) {
            printf("Couldn't open file.");
            exit(1);
        }

        // Check the file for occurences of the given symbols
        while((c = fgetc(file)) != EOF) {
            if(strchr(",", c) != NULL)
                sym_1++;
            if(strchr(".", c) != NULL)
                sym_2++;    
            if(strchr(":", c) != NULL)
                sym_3++; 
            if(strchr(";", c) != NULL)
                sym_4++;
            if(strchr("!", c) != NULL)
                sym_5++;
        }
        fclose(file);
        i++;

    } while (i < num_files);

    // Update occurence values    
    charfreq[0] = sym_1;
    charfreq[1] = sym_2;
    charfreq[2] = sym_3;
    charfreq[3] = sym_4;
    charfreq[4] = sym_5;

    file = fopen(filetowrite, "w");
    if(file == NULL) {
        printf("Couldn't open file.");
        exit(1);
    }
    
    // Write occurence values to filetowrite
    for(int i = 0; i < 5; i++)
	  {
	  	switch(i)
	  	{
	  		case 0:
	  			fprintf(file, ", -> %ld\n", charfreq[i]);
	  			break;
	  		case 1:
	  			fprintf(file, ". -> %ld\n", charfreq[i]);
	  			break;
	  		case 2:
	  			fprintf(file, ": -> %ld\n", charfreq[i]);
	  			break;
	  		case 3:
	  			fprintf(file, "; -> %ld\n", charfreq[i]);
	  			break;
	  		case 4:
	  			fprintf(file, "! -> %ld\n", charfreq[i]);
	  			break;
	  	}	  	
	  }
    fclose(file);
}