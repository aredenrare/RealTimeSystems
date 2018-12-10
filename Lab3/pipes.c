#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe
#include <signal.h> // for signals
#include <wait.h> // for wait

void sigpipe_handler(int signo) {
	puts(" -->o Got a SIGPIPE");
}

int compute_sum(int n) {
	int i,sum=0;
	for(i=0;i<n;i++,sum+=i);
	return sum;
}

int main(void) {
	int fd[2], // file descriptors for the pipe:
				   // fd[1] is the write-end of the pipe
				   // fd[0] is the read-end of the pipe
		buf=0, // 1-int-long buffer for parent/child communication
		child_pid; // for the fork system call
	signal(SIGPIPE,sigpipe_handler);

	pipe(fd);

	child_pid=fork();
	if(child_pid<0) {
		perror("Could not fork child");
		exit(1);
	}else if(child_pid==0){
        // Child
       close(fd[1]);            // closes unused write part
       int check = read(fd[0],&buf,sizeof(buf));
       close(fd[0]);

       printf("read from pipe = %d\n", buf);

        exit(0);
	}else{
        // Parent
        close(fd[0]);           // closes unused read part
        int sum = compute_sum(10);
        write(fd[1], &sum, sizeof(sum)+1);
        close(fd[1]);

        wait(NULL);
	}
	return EXIT_SUCCESS;
}