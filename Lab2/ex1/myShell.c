#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
// path = /home/are/Documents/School/RealTimeSys/Labz/Lab2/ex1

/*
Using the system calls seen today, write a C program myshell that, repeatedly:
	reads from standard input a command (for simplicity, let the command be just a single string - no arguments);
	if the input command is "halt", exit 0, indicating success;
	otherwise, see to it that the command is executed;
	upon completion, output the PID and return value of the process that has executed the command;
	if a command returns a non-zero value, exit 1 (indicating an error condition);
for as long as the command returns 0.
*/

/* System calls used:
	getpid()
	sleep()
	wait()
	fork()
	exec()
*/

// path = /home/are/Documents/School/RealTimeSys/Labz/Lab2/ex1
#define FOREVER for(;;)
pid_t child_pid;
int status;
int main(void) {
	char command[250];

	FOREVER {
		printf("please type your command ... ");
		scanf("%s", command);
		if(!strcmp(command,"halt")) {
			child_pid = getpid();
			printf("current pid = %d, status = %d \n", child_pid, status);
			puts("So long");
			
			return EXIT_SUCCESS; // same as exit(0)
		}
		// my code

		child_pid = fork();
		if (child_pid == 0){
			// child process
			char path[250] = "/bin/";
			strcat(path,command);
			execl(path, command, NULL);
			exit(0);
		}
		else if(child_pid > 0){
			// parent process
			wait(&status);
		}
		else{
			perror("fork() error\n");
			return 1;
		}
	}
	return EXIT_SUCCESS;
}