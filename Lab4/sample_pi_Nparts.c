 /** 
 * Sample pi program 
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#define N_STEPS (long int) 10000
#define USE_MATH_DEFINES
#define N_THREADS 4

double step=1/(double)N_STEPS, sum, x;
void *integral(void *param);

int main(void) {
    // initializing thread
    pthread_t tid[N_THREADS];
    pthread_attr_t attr[N_THREADS];
    char arg[10];

	double pi;
    for (int i=0; i<N_THREADS;i++){
        
        pthread_attr_init(&attr[i]);
        pthread_create(&tid[i],&attr[i],integral,arg);
        pthread_join(tid[i],NULL);
    }
    // for (int i=0; i<N_THREADS;i++){
    //     sprintf(arg,"%d\n",i);
    //     pthread_create(&tid[i],&attr[i],integral,arg);    
    // }
    // for (int i=0; i<N_THREADS;i++){
    //     pthread_join(tid[i],NULL);
    // }

	pi=step*sum;
	printf("Pi = %f\n", pi);
	return EXIT_SUCCESS;
}
void *integral(void *param){
    int i = atoi(param);
    int lower = i*N_STEPS/N_THREADS;
    int upper = (i+1)*N_STEPS/N_THREADS;

    for(lower; lower<upper; lower++){
        x = (lower+0.5)*step;
        sum += 4.0/(1.0+x*x);
    }
    pthread_exit(0);
}