#include "12.h"

int sem_init(sem_t *sem, int pshared, unsigned int value) {
	sem->value = value;
    pthread_mutex_init(&sem->m,NULL);
    pthread_cond_init(&sem->list,NULL);
	return 0;
}

int sem_destroy(sem_t *sem){
    pthread_mutex_destroy(&sem->m);
    pthread_cond_destroy(&sem->list);
	return 0;
}

int sem_wait(sem_t *sem){
    pthread_mutex_lock(&sem->m);
    sem->value--;
    sem->n_w++;
    while(sem->value<0){
        pthread_cond_wait(&sem->list,&sem->m);
    }
    sem->n_w--;
    pthread_mutex_unlock(&sem->m);
	return 0;
}

int sem_post(sem_t *sem){
    pthread_mutex_lock(&sem->m);
    sem->value++;
    if(sem->n_w > 0){
        pthread_cond_signal(&sem->list);
    }
    pthread_mutex_unlock(&sem->m);
	return 0;
}