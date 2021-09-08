#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {

	// Planning for two children
	pid_t pid, pid2;
	int status, status2;

	pid = fork();
	if(pid == 0) { // Child #1 code
		printf("CHILD <PID: %d> process is executing testalphabet program!\n", (int) getpid());
		char *args[] = {"./testalphabet", NULL};
		execvp("./testalphabet", args);
		exit(0);

	} else { // Child #2 code
		pid2 = fork();
		
		if(pid2 == 0) {
			printf("CHILD <PID: %d> process is executing testspecial program!\n", (int) getpid());
			char *args2[] = {"./testspecial", NULL};
			execvp("./testspecial", args2);
			exit(0);

        } else { // Parent - Child #2 has finished
			wait(&status2);
			printf("CHILD <PID: %d> process has done with testspecial program! See the results above!\n", (int) pid2);
        }
        
	} // Child #1 has finished 
	if (wait(&status) != 0)
		printf("CHILD <PID: %d> process has done with testalphabet program! See the results above!\n", (int) pid);
	
	return 0;	
}
