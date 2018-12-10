#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
sem_t sem1;
sem_t sem2;

void *f_a(void* arg) {
	puts("!!!First Hello World!!!"); // CONCURRENT EXECUTION 1
    sem_post(&sem1); sem_wait(&sem2);
	puts("!!!Third Hello World!!!"); // CONCURRENT EXECUTION 2;
	return NULL;
}

void *f_b(void* arg) {
	puts("!!!Second Hello World!!!"); // CONCURRENT EXECUTION 1
    sem_post(&sem2); sem_wait(&sem1);
	puts("!!!Fourth Hello World!!!"); // CONCURRENT EXECUTION 2
	return NULL;
}

int main(void) {
    sem_init(&sem1,0,0);
	sem_init(&sem2,0,0);
    pthread_t thread_a, thread_b;
	pthread_create(&thread_a, NULL, f_a, NULL);
    pthread_create(&thread_b, NULL, f_b, NULL);

	pthread_join(thread_a,NULL);
	pthread_join(thread_b,NULL);

	return EXIT_SUCCESS;
}