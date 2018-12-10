#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
pthread_barrier_t bar1;
pthread_barrierattr_t attr1;
unsigned int count = 2;
int ret1;

void *f_a(void* arg) {
	puts("!!!First Hello World!!!"); // CONCURRENT EXECUTION 1
    pthread_barrier_wait(&bar1);    
	puts("!!!Third Hello World!!!"); // CONCURRENT EXECUTION 2;
	return NULL;
}

void *f_b(void* arg) {
	puts("!!!Second Hello World!!!"); // CONCURRENT EXECUTION 1
    pthread_barrier_wait(&bar1);
	puts("!!!Fourth Hello World!!!"); // CONCURRENT EXECUTION 2
	return NULL;
}

int main(void) {
    ret1 = pthread_barrier_init(&bar1, &attr1, count);

    pthread_t thread_a, thread_b;
	pthread_create(&thread_a, NULL, f_a, NULL);
    pthread_create(&thread_b, NULL, f_b, NULL);

	pthread_join(thread_a,NULL);
	pthread_join(thread_b,NULL);

	return EXIT_SUCCESS;
}