#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>

#define FOREVER while(!time_ended)
#define N_THREADS 10
#define PREFIX "cart-"
/* the following values are just examples of the possible duration 
 * of each action and of the simulation: feel free to change them */
#define MAX_DELAY_PLACE_BOX 2		//2
#define MAX_DELAY_PICK_UP_BOX 10	//10
#define MAX_DELAY_UNLOAD_BOX 1		//1
#define MAX_DELAY_BRING_BOX_BACK 12	//12
#define END_OF_TIME 60
#define TRUE 1
#define FALSE 0
#define MAX_BOXES_IN_ALLEY 5

typedef char name_t[20];
typedef int boolean;

time_t big_bang;
boolean time_ended=FALSE;

// semaphores used
sem_t sem_alley;
sem_t sem_cart;
sem_t sem_forklift;
// counters used
int boxes_in_alley = 0;
int boxes_delivered = 0;
int boxes_returned = 0;


void do_action(char *thread_name, char *action_name, int max_delay) {
	int delay=rand()%max_delay+1;
	// int delay=max_delay; /* used to test with constant delays */
	printf("[%4.0f]\t%s: %s (%d) started\n", difftime(time(NULL),big_bang), thread_name, action_name, delay);
	sleep(delay);
	printf("[%4.0f]\t%s: %s (%d) ended\n", difftime(time(NULL),big_bang), thread_name, action_name, delay);
}

void *forklift(void *arg) {
	FOREVER {
		sem_wait(&sem_forklift);
		sem_wait(&sem_alley);
		boxes_in_alley--;
		sem_post(&sem_alley);
		do_action("fork lift", "place box and return to alley", MAX_DELAY_PLACE_BOX);
		boxes_delivered++;
		printf("forklift done- delivered: %d, in alley: %d\n", boxes_delivered, boxes_in_alley);
	}
	puts("Terminating fork lift.");
	pthread_exit(NULL);
}

void *cart(void *thread_name) {
	int id=atoi(thread_name+strlen(PREFIX));
	FOREVER {
		do_action(thread_name, "pick up box and go to alley", MAX_DELAY_PICK_UP_BOX);
		sem_wait(&sem_cart);
		if(boxes_in_alley < MAX_BOXES_IN_ALLEY){
			sem_wait(&sem_alley);
			boxes_in_alley++;
			sem_post(&sem_alley);
			sem_post(&sem_cart);
			do_action(thread_name, "unload box", MAX_DELAY_UNLOAD_BOX);
			sem_post(&sem_forklift);
		}
		else {
			sem_post(&sem_cart);
			do_action(thread_name, "bring box back", MAX_DELAY_BRING_BOX_BACK);
			boxes_returned++;
			printf("boxes returned: %d\n", boxes_returned);
		}
	}
	printf("Terminating cart %d.\n", id);
	pthread_exit(NULL);
}

int main(void) {
	pthread_t fork_id, cart_id[N_THREADS];
	name_t cart_name[N_THREADS];
	int i;

	sem_init(&sem_alley,0,1);
	sem_init(&sem_cart,0,1);
	sem_init(&sem_forklift,0,0);
	time(&big_bang);

	pthread_create(&fork_id, NULL, forklift, NULL);
	for(i=0;i<N_THREADS;i++) {
		sprintf(cart_name[i],"%s%d", PREFIX, i);
		pthread_create(cart_id+i,NULL,cart,cart_name+i);
	}

	sleep(END_OF_TIME);
	time_ended=TRUE;

	pthread_join(fork_id,NULL);
	for(i=0;i<N_THREADS;i++) {
		pthread_join(cart_id[i],NULL);
	}

	sem_destroy(&sem_alley);
	sem_destroy(&sem_cart);
	sem_destroy(&sem_forklift);
	return EXIT_SUCCESS;
}