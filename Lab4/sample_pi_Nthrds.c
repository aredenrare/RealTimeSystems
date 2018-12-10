#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define N_STEPS (long int) 10000

double step=1/(double)N_STEPS, sum=0.0, x;

void *integral(void *param);

int main(void) {
    // thread init
    pthread_t tid;
    pthread_attr_t attr;
    char arg[10];

	int i;
	double pi;
    
	for(i=0; i<N_STEPS; i++) {
        sprintf(arg,"%d",i);
        pthread_attr_init(&attr);
        pthread_create(&tid,&attr,integral,arg);
        pthread_join(tid,NULL);
	}

	pi=step*sum;
	printf("Pi = %f\n", pi);
	return EXIT_SUCCESS;
}

void *integral(void *param){
    int i = atoi(param);
    x = (i+0.5)*step;
    sum+=4.0/(1.0+x*x);
    pthread_exit(0);
}