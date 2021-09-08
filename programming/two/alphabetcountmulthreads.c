/*
 * alphabetcountmulthreads.c - this file implements the alphabetcountmulthreads function.
 */

#include <stdio.h> 
#include <stdlib.h>
#include "count.h"
#include <dirent.h>
#include <string.h> 
#include <stdbool.h>
#include <pthread.h>
#include <math.h>
#include <ctype.h>

// Global variables
char *filelist[MAXFILENUM];
char *newpath[MAXFILENUM];

pthread_mutex_t mtx;

struct alpha_occur_struct {
    int starting_index; // Varying starting index per thread
    int ending_index; // Varying ending index per thread 
    long *occurrences;
};

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

void *alpha_occurrence(void *arg) {

    struct alpha_occur_struct *arg_struct = (struct alpha_occur_struct*)arg;

    printf("Thread ID: %u starts processing files with index from %d to %d!\n", (int) pthread_self(), arg_struct->starting_index, arg_struct->ending_index);

    FILE *file;

    int c;
    int i = arg_struct->starting_index;
    
    do {
        file = fopen (newpath[i], "r");
        if(file == NULL) {
            printf("Couldn't open file.");
            exit(1);
        }

        printf("Thread ID: %u is processing file %s\n", (int) pthread_self(), filelist[i]);

        // Check the files for occurences of the alphabet characters
        while((c = fgetc(file)) != EOF) {
            int index = (tolower(c) - 'a');
            if(index >= 0 && index <= 26) {
                pthread_mutex_lock(&mtx);
                arg_struct->occurrences[index]++;
                pthread_mutex_unlock(&mtx);
            }
        }

        fclose(file);
        i++;

    } while (i < arg_struct->ending_index);

    printf("Thread ID: %u is done!\n", (int) pthread_self());

    pthread_exit(0);
}

void alphabetcountmulthreads(char *path, char *filetowrite, long alphabetfreq[], int num_threads)
{
	pthread_mutex_init(&mtx, NULL);

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

    if(num_threads > num_files) {
        printf("There is more threads than there is files. Exiting.\n");
        exit(0);
    }

    rewinddir(d);
   
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

    // Add the path to the filenames in a new array
    for(int i = 0; i < num_files; i++) {
        newpath[i] = malloc(strlen(path) + strlen(filelist[i]) +1);
        strcpy(newpath[i], path);
        strcat(newpath[i], filelist[i]);
    } 

    struct alpha_occur_struct args[num_files];

    int files_to_read = 0;

    // Launch multiple threads in parallel
    pthread_t tids[num_threads];
    for(int i = 0; i < num_threads; i++) {
            args[i].starting_index = files_to_read;
        if((num_files % num_threads) != 0 && i == (num_threads - 1)) {
            files_to_read+=((num_files/num_threads) + (num_files % num_threads));
        } else {
            files_to_read+=(num_files/num_threads);
        }
        args[i].ending_index = files_to_read; 
        args[i].occurrences = alphabetfreq;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&tids[i], &attr, alpha_occurrence, &args[i]);           
    }
    
    // Update occurence values        
    for(int i = 0; i < num_threads; i++) {
        pthread_join(tids[i], NULL);
    } 

    FILE *file;
    
    file = fopen(filetowrite, "w");
    if(file == NULL) {
        printf("Couldn't open file.");
        exit(1);
    }

    // Write occurence values to filetowrite
    for(int i = 0; i < ALPHABETSIZE; i++) {
        fprintf(file, "%c -> %ld\n", (char)(i+97), alphabetfreq[i]);
    }

    fclose(file);
    pthread_mutex_destroy(&mtx);	
}
