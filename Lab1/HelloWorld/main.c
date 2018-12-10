/*
 ============================================================================
 Name        : 2017-10-10-POSIX-system-calls.c
 Author      : Paolo Torroni
 Version     :
 Copyright   : Use as you wish
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> // for open(2)
#include <sys/stat.h> // for open(2)
#include <fcntl.h> // for open(2)
#include <unistd.h> // for write(2)
#include <errno.h> // for perror(3)


int main(void) {
	int count=1;
	char buffer = '@';
	char buf[4];
	buf[0] = buffer;

	int fd = open("aFile.txt", O_WRONLY | O_APPEND);
	if(fd<0) {
		perror("could not open/create file");
		exit(1);
	}

	write(fd, buf, count);
	if(count!=1) {
		perror("could not write to file");
		exit(2);
	}

	close(fd);

	return EXIT_SUCCESS;
}
