#include <stdio.h>
#include <stdlib.h>
#include "count.h"
#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

char *newpath[50];

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

void alphabetlettercount(char *path, char *filetowrite, long alphabetfreq[])
{
    struct dirent * entry;
    DIR *d = opendir(path);
    if(d == 0) {
        perror("opendir");
        return;
    }

    int num_files = 0;

    // Look for filenames ending in .txt
    while((entry = readdir(d)) != 0) {
        if(ends_with(entry->d_name, ".txt")) {
            num_files++;
        }
    }

    rewinddir(d);
    char *filelist[num_files];
    int i = 0;

    // Copy filenames ending in .txt to filelist array
    while((entry = readdir(d)) != 0) {  
        if(ends_with(entry->d_name, ".txt")) {
            filelist[i] = (char*) malloc (strlen(entry->d_name)+1);
            strncpy (filelist[i],entry->d_name, strlen(entry->d_name));
            i++;
        }
    }
    closedir(d);

    FILE *file;

    long freq[ALPHABETSIZE] = {0};

    int c;
    i = 0;

    // Create new array with full file path
    for(int i = 0; i < num_files; i++) {
        newpath[i] = malloc(strlen(path) + strlen(filelist[i]) +1);
        strcpy(newpath[i], path);
        strcat(newpath[i], filelist[i]);
    } 

    do {
        file = fopen (newpath[i], "r");
        if(file == NULL) {
            printf("Couldn't open file.");
            exit(1);
        }

        // Check the files for occurences of the alphabet characters 
        while((c = fgetc(file)) != EOF) {
            freq[(int)(tolower(c) - 'a')]++;
        }
        fclose(file);
        i++;

    } while (i < num_files);

    // Copy the occurences values to alphabetfreq[]
    for(int i = 0; i < ALPHABETSIZE; i++) {
        alphabetfreq[i] = freq[i];
    }

    file = fopen(filetowrite, "w");
    if(file == NULL) {
        printf("Couldn't open file.");
        exit(1);
    }
    
    // Write occurence values to filetowrite
    for(int i = 0; i < 26; i++)
        fprintf(file, "%c -> %ld\n", (char)(i+97), alphabetfreq[i]);
    fclose(file);
}