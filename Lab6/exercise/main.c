#include "monitor.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>

typedef char thread_name_t[10];

int main(void) {
    pthread_t my_threads[N_THREADS];
    thread_name_t my_thread_names[N_THREADS];
    int i;

	monitor_init(&mon);
    for (i=0;i<N_THREADS;i++) {
     	sprintf(my_thread_names[i],"%d",i);
        pthread_create(&my_threads[i], NULL, philosopher_runner, my_thread_names[i]);
    }

    for (i=0;i<N_THREADS;i++) {
        pthread_join(my_threads[i], NULL);
    }

    monitor_destroy(&mon);

    return EXIT_SUCCESS;
}