#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> // for mkfifo
#include <sys/stat.h> // for mkfifo
#include <fcntl.h> // for mode argument in mkfifo
#include <unistd.h> // for write

int compute_sum(int n);

int main(int argc, char *argv[]) {
	int fd, buf, n;

	// FIFO file path
	char * created_fifo = "/tmp/myfifo"; 

	// Creating the named file(FIFO)
	mkfifo(created_fifo,0666);

	while(1){
		fd = open(created_fifo, O_RDONLY);
		printf("waiting to receive a number n to compute the sum of the n first positive integers..\n");
		read(fd,&n,sizeof(n));
		printf("received from program2, n = %d \n", n);
		buf = compute_sum(n);
		if(n>=0){
			printf("sum of %d first integers = %d \n", n, buf);
		}
		close(fd);
		if(n==69){
			puts("nice");
		} else if(n<0){
			puts("n negative, terminates program");
			exit(0);
		}
	}
	return 0;
}

int compute_sum(int n) {
	int i,sum=0;
	for(i=0;i<n;i++,sum+=i);
	return sum;
}