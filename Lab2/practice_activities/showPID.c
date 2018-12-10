#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

pid_t child_pid;

int main(void){
	
	child_pid=fork();
	if(child_pid==0){
		child_pid = getpid();
		printf("Child, pid = %d \n", child_pid);
		return 0;
	}
	else if(child_pid>0){
		child_pid = getpid();
		wait(NULL);
		printf("Parent, pid = %d \n", child_pid);
		return 0;
	}
	else{
		perror("fork() error\n");
		return 1;
	}
}
