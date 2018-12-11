#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>

#define N_THREADS 5
#define N_CHOPS 10
#define FOREVER for(;;)

typedef enum {THINKING, HUNGRY, EATING} state_t;
typedef struct {
    pthread_mutex_t m;

    pthread_cond_t cv_phil[N_THREADS];
	int chop_pair[N_THREADS];
    state_t state_phil[N_THREADS];
    int chop_choice[N_THREADS]; 
} monitor_t;
monitor_t mon;


void pick_up(monitor_t *mon, int phil_nr);
void put_down(monitor_t *mon, int phil_nr);
void monitor_init(monitor_t *mon);
void monitor_destroy(monitor_t *mon);

void *philosopher_runner(void *arg); 

double spend_some_time(int);

// void simulate_thinking();
// void simulate_eating();