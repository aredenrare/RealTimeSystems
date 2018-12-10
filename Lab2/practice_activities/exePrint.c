#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int main(void){
	execlp("/home/are/Documents/School/RealTimeSys/Labz/Lab2/practice_activities/Print5",
	"./Print5", (char*) NULL);
	return 0;
}