#include <stdio.h>
#include <stdlib.h>

int main(){
	FILE *fptr;

	char filename[1000], c;

	printf("Enter filenampe to open \n");
	scanf("%s", filename);

	fptr = fopen(filename, "r");
	if (fptr == NULL){
		printf("ERROR opening file");
		exit(0);
	}
	c = fgetc(fptr);
	while(c != EOF){
		printf("%c", c);
		c = fgetc(fptr);
	}
	fclose(fptr);
	return 0;
}

