#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#define FOREVER for(;;)
/*
Write a program that defines a variable x and then enters an infinite loop. 
At every iteration, the program sleeps for a second, then: 
if x is 0, the program displays a line that goes "green", 
otherwise, if x is positive, it displays a line that goes "blue";
if x is non-negative, the value of x is toggled between 0 and positive.
The program escapes from the loop only if x is negative.
Be sure to terminate the program, once it starts, using the kill shell command.

Hint: applications can use SIGUSR1 and SIGUSR2 for notifying events that can change 
		the state of processes.

Extra: modify the code so as to toggle between the "green" and the "blue" mode 
	not at every iteration, but only when a given signal is received.
*/
int x;
pid_t child_pid;
int status;

void loop_breaker(int signo);

int main(void) {
	child_pid = fork();

	if(child_pid == 0){
		// child process
		printf("To halt, kill -%d %d\n", SIGUSR1, getpid());
		printf("x = %d\n", x);
		FOREVER {
			sleep(1);
			if(x==0){
				puts("green");
				signal(SIGUSR1,loop_breaker);
			}
			else if(x>0){
				puts("blue");
				signal(SIGUSR1,loop_breaker);
			}
			else{
				return EXIT_FAILURE;
			}
			if(x>=0){
				x=1-x;
			}
		}
	}
	if(child_pid > 0){
		// parent process
		wait(NULL);
	}
	else{
		printf("Fork failed\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

void loop_breaker(int signo) {
	switch(signo){
		case SIGUSR1:
			puts("SIGUSR1 received");
			kill(getpid(),SIGUSR1);
			break;
		case SIGUSR2:
			puts("SIGUSR2 received");
			kill(getpid(),SIGUSR2);
			break;
	}
	puts("program terminated");
	exit(0);
}
