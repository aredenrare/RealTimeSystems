/** 
 * Sample pi program 
 */

#include <stdio.h>
#include <stdlib.h>
#define N_STEPS (long int) 1000000

int main(void) {

	int i;
	double pi, step, x, sum=0.0;

	step =  1/(double)N_STEPS;

	for(i=0; i<N_STEPS; i++) {
		x = (i+0.5)*step;
		sum+=4.0/(1.0+x*x);
	}

	pi=step*sum;
	printf("Pi = %f\n", pi);
	return EXIT_SUCCESS;
}