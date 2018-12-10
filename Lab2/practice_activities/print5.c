#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

pid_t child_pid;
int wstatus;

int main(void){

	int x = 0;
	child_pid = fork();

	if (child_pid == 0){
		// Child process
		printf("child before update, X = %d \n", x);
		x = 5;
		printf("child after update, X = %d \n", x);
		return x;
	}
	else if(child_pid > 0){
		// parent process
		wait(&wstatus);
		printf("after child, X = %d \n", x);
		if(WIFEXITED(wstatus)){
			printf("wstatus = %d, return value from child = %d \n", wstatus, WEXITSTATUS(wstatus));
		}
		return 0;
	}
	else{
		perror("fork() error\n");
		return 1;
	}
}