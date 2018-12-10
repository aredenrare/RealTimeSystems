/*

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
pthread_barrier_t bar1;
pthread_barrier_t bar2;
pthread_barrierattr_t attr1;
pthread_barrierattr_t attr2;
unsigned int count = 2;
int ret1;
int ret2;

void *f_a(void *arg) {
	puts("!!!First Hello World!!!");
    pthread_barrier_wait(&bar1);
    return NULL;
}

void *f_b(void *arg) {
	pthread_barrier_wait(&bar1);
    puts("!!!Second Hello World!!!");
    pthread_barrier_wait(&bar2);
    return NULL;
}

int main(void) {
    ret1 = pthread_barrier_init(&bar1, &attr1, count);
    ret2 = pthread_barrier_init(&bar2, &attr2, count);
    pthread_t thread_a, thread_b;
    pthread_create(&thread_a, NULL, f_a, NULL);
    pthread_create(&thread_b, NULL, f_b, NULL);
    
    pthread_barrier_wait(&bar2);
    
    puts("!!!Third Hello World!!!");

	pthread_join(thread_a,NULL);
    pthread_join(thread_b,NULL);

	puts("!!!Last Hello World!!!");
    pthread_barrier_destroy(&bar1);
    pthread_barrier_destroy(&bar2);

	return EXIT_SUCCESS;
}