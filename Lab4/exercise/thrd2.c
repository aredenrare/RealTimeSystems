#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define N_STEPS (long int) 65000000
#define N_THREADS 4

double pi_final; /* this data is shared by the thread(s) */
typedef char tname_t[5];

int compute_pi(int interval, int N) {

	int i;
	double pi, step, x, sum=0.0;

	step =  1/(double)N_STEPS;

	for(i=(N_STEPS/N)*interval; i<(N_STEPS/N)*(interval+1); i++) {
		x = (i+0.5)*step;
		sum+=4.0/(1.0+x*x);
	}

	pi=step*sum;
	pi_final+=pi;
	printf("compute_pi(%i,%i): pi = %f,\tpi_final = %f\n", interval, N, pi, pi_final);
	return EXIT_SUCCESS;
}

void *runner(void *param); /* the thread */

int main(int argc, char *argv[])
{
	int tn; // thread_number
	tname_t arg[5]; // thread name
	pthread_t tid[N_THREADS]; /* the thread identifiers */
	pthread_attr_t attr[N_THREADS]; /* set of attributes for the thread */

	struct timespec tv_start, tv_end;
	clock_gettime(CLOCK_REALTIME,&tv_start);

	/* get the default attributes */
	for(tn=0;tn<N_THREADS;tn++)
		pthread_attr_init(&attr[tn]);

	/* create the threads */
	for(tn=0;tn<N_THREADS;tn++) {
		sprintf(arg[tn],"%d\n",tn);
		pthread_create(&tid[tn],&attr[tn],runner,arg[tn]);
	}

	puts("I am parent");


	/* now wait for the thread to exit */
	for(tn=0;tn<N_THREADS;tn++)
		pthread_join(tid[tn],NULL);

	clock_gettime(CLOCK_REALTIME,&tv_end);
	printf("parent:\t pi_final = %.10f\n",pi_final);
	printf("parent:\t actual PI: %.10f\n", M_PI);
	printf("parent:\t N_THREADS: %d, N_STEPS: %.0e, total time: %.3f (msec)\n", N_THREADS, (double) N_STEPS, 
	(tv_end.tv_nsec-tv_start.tv_nsec + 1e9 * (tv_end.tv_sec-tv_start.tv_sec))/(double)1e6);
	return EXIT_SUCCESS;
}

/**
 * The thread will begin control in this function
 */
void *runner(void *param) {
	printf("I am thread %s", (char *) param);
	compute_pi(atoi(param),N_THREADS);
	pthread_exit(0);
	
}