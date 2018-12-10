/*
 ============================================================================
 Name        : philosophers.c
 Author      : Paolo Torroni
 Version     : Nov 20, 2017
 Copyright   : Use as you wish
 Description : An implementation of several solutions of the dining philosophers
 	 	 	   synchronization problem. To toggle between modes, check/unckeck
 	 	 	   options SOLUTION_TANENBAUM, DEADLOCK_FREE, and LOOP_N.
 ============================================================================
 */

/* comment/uncomment the following line to toggle between solutions */
//#define SOLUTION_TANENBAUM

#ifndef SOLUTION_TANENBAUM
	
    #define SOLUTION_BASIC
	/* comment/uncomment the following line in order to introduce/remove risk of deadlock */
	#define DEADLOCK_FREE
#endif /* SOLUTION_TANENBAUM */

/* comment/uncomment the following line in order to toggle between mortal/immortal philosophers */
// #define LOOP_N
#ifndef LOOP_N
	#define LOOP_FOREVER
#endif /* LOOP_N */

#define FOREVER for(;;)
#define LONG_TIME 100000000
#define SHORT_TIME 5000000
#define N_THREADS 5 // number of philosophers

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "12.h"
//#include <semaphore.h>

typedef char threadID_t[10];
void *philo(void *); // the function that implement a philosopher thread
double do_something(int, char *, long); // spend time, e.g. by EATING or THINKING
void pick_up(int philo); // pick up both chopsticks
void tabs(int n) {while(n-->0)putchar('\t');} // auxiliary function: for visualization purposes

pthread_mutex_t om=PTHREAD_MUTEX_INITIALIZER; // "output mutex" to ensure that the output of threads does not get mixed up


#ifdef SOLUTION_TANENBAUM
/*
 * Tanenbaum's solution with self[philo] semaphores
 */

sem_t self[N_THREADS];
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

enum {THINKING, EATING, HUNGRY} state[N_THREADS];

void test(int philo) {
	int left=(philo+N_THREADS-1)%N_THREADS, right=(philo+1)%N_THREADS;
	if(state[philo]==HUNGRY
			&& state[left]!=EATING
			&& state[right]!=EATING) {
		state[philo]=EATING;
		sem_post(&self[philo]);
	}
}

void pick_up(int philo) {
	pthread_mutex_lock(&mutex);
		state[philo]=HUNGRY;
		test(philo);
	pthread_mutex_unlock(&mutex);
	sem_wait(&self[philo]);
	pthread_mutex_lock(&om);
		tabs(philo);
		printf("%d: picking up chopsticks %d and %d\n", philo, philo, (philo+1)%N_THREADS);
	pthread_mutex_unlock(&om);
}

void put_down(int philo) {
	int left=(philo+N_THREADS-1)%N_THREADS, right=(philo+1)%N_THREADS;
	pthread_mutex_lock(&mutex);
		pthread_mutex_lock(&om);
			tabs(philo);
			printf("%d: putting down chopsticks %d and %d\n", philo, philo, right);
		pthread_mutex_unlock(&om);
		state[philo]=THINKING;
		test(right);
		test(left);
	pthread_mutex_unlock(&mutex);
}

#endif

#ifdef SOLUTION_BASIC

/* basic solution, with 1 right-handed and N_THREADS-1 left-handed philosophers
 *
 */

sem_t chopstick[5];

void pick_up(int philo) {
	int left=philo, right=(philo+1)%N_THREADS;
	int first=left, second=right;
#ifdef DEADLOCK_FREE
	if(!philo) {
		first=right;
		second=left;
	}
#endif
	sem_wait(&chopstick[first]);
	pthread_mutex_lock(&om);
		tabs(philo);
		printf("%d: picked up chopstick %d\n", philo, first);
	pthread_mutex_unlock(&om);
	do_something(philo,"WAITING A BIT BEFORE PICKING UP SECOND CHOPSTICK", LONG_TIME);
	sem_wait(&chopstick[second]);
	pthread_mutex_lock(&om);
		tabs(philo);
		printf("%d: picked up chopstick %d\n", philo, second);
	pthread_mutex_unlock(&om);
}

void put_down(int philo) {
	int left=philo, right=(philo+1)%N_THREADS;
	sem_post(&chopstick[left]);
	pthread_mutex_lock(&om);
		tabs(philo);
		printf("%d: put down chopstick %d\n", philo, left);
	pthread_mutex_unlock(&om);
	sem_post(&chopstick[right]);
	pthread_mutex_lock(&om);
		tabs(philo);
		printf("%d: put down chopstick %d\n", philo, right);
	pthread_mutex_unlock(&om);
}

#endif


/*
 * functions for animating the solution:
 * main() creates/joins the threads and takes care of initializing/destroying data structures
 * philo() is the thread's code
 * do_something() is used to simulate thread's work
 */

/*
 * main entry point
 */

int main(void) {
	int i; // used to iterate from 0 to N-1
	threadID_t id[N_THREADS]; // an array of N_THREADS strings, containing the IDs of philosophers
	pthread_t ph_thread[N_THREADS]; // N_THREADS philosopher threads

	// initialize synchronization devices
	for(i=0;i<N_THREADS;i++) {
#ifdef SOLUTION_BASIC
		sem_init(&chopstick[i], 0, 1);
#endif
#ifdef SOLUTION_TANENBAUM
		sem_init(&self[i], 0, 0);
#endif
	}

	// create threads
	for(i=0;i<N_THREADS;i++) {
		sprintf(id[i],"%d",i);
		pthread_create(&ph_thread[i], NULL, philo, id[i]);
	}

	// join threads
	for(i=0;i<N_THREADS;i++) {
		pthread_join(ph_thread[i],NULL);
	}

	// destroy synchronization devices
	for(i=0;i<N_THREADS;i++) {
#ifdef SOLUTION_BASIC
		sem_destroy(&chopstick[i]);
#endif
#ifdef SOLUTION_TANENBAUM
		sem_destroy(&self[i]);
		pthread_mutex_destroy(&mutex);
#endif
	}
	pthread_mutex_destroy(&om);

	return EXIT_SUCCESS;
}

/*
 * thread's entry point: notice that it's independent of the solution being adopted
 * the only argument, arg, is a string that shows philo's ID (0, 1, ...)
 * Knowing the philosopher's ID is necessary in order to use che correct chopsticks
 */

void *philo(void *arg) {
	int philo=atoi((char*)arg);
#ifdef LOOP_FOREVER
	FOREVER {
#endif
#ifdef LOOP_N
		int n=10;
		while(n--) {
#endif
		do_something(philo,"THINKING", LONG_TIME);
		pthread_mutex_lock(&om);
			tabs(philo);
			printf("%d: HUNGRY!!\n", philo);
		pthread_mutex_unlock(&om);
		pick_up(philo);
		do_something(philo,"EATING", LONG_TIME);
		put_down(philo);
	}
	return NULL;
}

/*
 * do_something is used just to waste time, so execution is slowed down, and for
 * animation purposes.
 * do_something computes PI. It lasts long if the third argument is a big number.
 * It doesn't last long if the third argument is small. The second argument is
 * a string, which is displayed twice: once at the beginning and once at the end.
 * The first argument is the philosopher's number (0, 1, ...).
 */

double do_something(int philo, char *what, long cycles) {
	double x, sum=0.0, step;
	long i, N_STEPS=rand()%cycles;
	pthread_mutex_lock(&om);
		tabs(philo);
		printf("%d: %s ...\n", philo, what);
	pthread_mutex_unlock(&om);
	step = 1/(double)N_STEPS;
	for(i=0; i<N_STEPS; i++) {
		x = (i+0.5)*step;
		sum+=4.0/(1.0+x*x);
	}
	pthread_mutex_lock(&om);
		tabs(philo);
		printf("%d: ... DONE %s\n", philo, what);
	pthread_mutex_unlock(&om);
	return step*sum;
}