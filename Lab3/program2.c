#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> // for mkfifo
#include <sys/stat.h> // for mkfifo
#include <fcntl.h> // for mode argument in mkfifo
#include <unistd.h> // for write

int main(int argc, char *argv[]) {
	int fd, n;
	char * created_fifo = "/tmp/myfifo"; 

	mkfifo(created_fifo, 06666);
	while(1){
		fd = open(created_fifo,O_WRONLY);
		printf("Write in a positive integer n:\n");
		scanf("%d",&n);

		write(fd,&n,sizeof(n));
		close(fd);
		if(n==69){
			puts("lol");
		} else if(n<0){
			puts("n negative, terminates program");
			exit(0);
		}
	}

	return 0;
}