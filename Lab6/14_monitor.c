#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>

// CONSTANTS AND MACROS
// for readability
#define N_THREADS 5
#define FOREVER for(;;)

// DEFINITIONS OF NEW DATA TYPES
// for readability
typedef char thread_name_t[10];
typedef enum {THINKING, HUNGRY, EATING} state_t;
 // monitor also defined as a new data type
typedef struct {
    // monitor lock (one mutex lock per monitor)
    pthread_mutex_t m;
    // condvars (one condvar per queue)
    // best if give meaningful names instead of cv_1, cv_2, ...
    pthread_cond_t left_phil, right_phil; //, condvar_3, ...;
    // state variables
    int n_w1, n_w2, ..., counter, ...;
    state_t statevar, ...;
} monitor_t;

// GLOBAL VARIABLES
// the monitor should be defined as a global variable
monitor_t mon;


//  MONITOR API
void pick_up(monitor_t *mon, char* name, ...);
void put_down(monitor_t *mon, char* name, ...);
void monitor_init(monitor_t *mon);
void monitor_destroy(monitor_t *mon);

// OTHER FUNCTION DECLARATIONS
// functions corresponding to thread entry points
void *philosophers(void *arg); 
// spend_some_time could be useful to waste an unknown amount of CPU cycles, up to a given top 
double spend_some_time(int);

// simulate_action_* to be replaced by function names according to application: e.g., pick_up, put_down, ...

// void simulate_thinking() { spend_some_time(100); }
// void simulate_hungry() { spend_some_time(2); }
// void simulate_eating() { spend_some_time(10); }

void simulate_pick_up() { spend_some_time(3); }
void simulate_put_down() { spend_some_time(3); }

// IMPLEMENTATION OF MONITOR API
void pick_up(monitor_t *mon, char *name, ...) {
	pthread_mutex_lock(&mon->m);
		if( /* <case_1> */ ) {
			// update state
			while(!( <condition_1> )) {
				pthread_cond_wait(&mon->left_phil,&mon->m);
			}
			// update state after resource acquisition
			// signal/broadcast after acquisition
		}
		else if( /* <case_2> */ ) {
			// update state
			while(!( <condition_2> )) {
				pthread_cond_wait(&mon->right_phil,&mon->m);
			}
			// update state after resource acquisition
			// signal/broadcast after acquisition
		}
		else if( /* <case_3> */ ) {
			// update state
			while(!( <condition_3> )) {
				pthread_cond_wait(&mon->condvar_3,&mon->m);
			}
			// update state after resource acquisition
			// signal/broadcast after acquisition
		}
		else {
			...
		}
	pthread_mutex_unlock(&mon->m);
}

void put_down(monitor_t *mon, char *name, ...) {
	pthread_mutex_lock(&mon->m);
		if( /* <case_1> */ ) {
			// update state
			// signal/broadcast after release
		}
		else if( /* <case_2> */ ) {
			// update state
			// signal/broadcast after release
		}
		else ...
	pthread_mutex_unlock(&mon->m);
}

void monitor_init(monitor_t *mon) {
	// set initial value of monitor data structures, state variables, mutexes, counters, etc.
    // typically can use default attributes for monitor mutex and condvars
    pthread_mutex_init(&mon->m,NULL);
    pthread_cond_init(&mon->left_phil,NULL);
    pthread_cond_init(&mon->right_phil,NULL);
    ...
    // set all condvar counters to 0
    mon->n_w1=0;
    mon->n_w2=0;
    ...
    // initialize whatever other structures
    mon->counter=...;
}

void monitor_destroy(monitor_t *mon) {
    // set initial value of monitor data structures, state variables, mutexes, counters, etc.
    pthread_cond_destroy(&mon->left_phil);
    pthread_cond_destroy(&mon->right_phil);
    ...
    pthread_mutex_destroy(&mon->m);
}

// MAIN FUNCTION
int main(void) {
    // thread management data structures
    pthread_t my_threads[N_THREADS];
    thread_name_t my_thread_names[N_THREADS];
    int i;

    // initialize monitor data strcture before creating the threads
	monitor_init(&mon);

    for (i=0;i<N_THREADS;i++) {
     	sprintf(my_thread_names[i],"t%d",i);
        // create N_THREADS thread with same entry point 
        // these threads are distinguishable thanks to their argument (their name: "t1", "t2", ...)
        // thread names can also be used inside threads to show output messages
        pthread_create(&my_threads[i], NULL, philosophers, my_thread_names[i]);
    }

    for (i=0;i<N_THREADS;i++) {
        pthread_join(my_threads[i], NULL);
    }

    // free OS resources occupied by the monitor after creating the threads
    monitor_destroy(&mon);

    return EXIT_SUCCESS;
}

// TYPE 1 THREAD LOOP
void *philosophers(void *arg) {
	// local variables definition and initialization
	char *thread_name = ...; // thread_name = f(arg)
	FOREVER { // or any number of times
		simulate_action_1();

		method_acquire(&mon, thread_name, ...);
		simulate_action_2();
		method_release(&mon, thread_name, ...);

		simulate_action_3();

		method_acquire(&mon, thread_name, ...);
		simulate_action_4();
		method_release(&mon, thread_name, ...);
	}
	pthread_exit(NULL);
}

// AUXILIARY FUNCTIONS
double spend_some_time(int max_steps) {
    double x, sum=0.0, step;
    long i, N_STEPS=rand()%(max_steps*1000000);
    step = 1/(double)N_STEPS;
    for(i=0; i<N_STEPS; i++) {
        x = (i+0.5)*step;
        sum+=4.0/(1.0+x*x);
    }
    return step*sum;
}