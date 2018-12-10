
#ifndef MYSEM_H_
#define MYSEM_H_

#endif /* MYSEM_H_ */

#include <pthread.h>

typedef struct {
    int value, n_w;
    pthread_cond_t list;
    pthread_mutex_t m;
} sem_t;

int sem_init(sem_t *sem, int pshared, unsigned int value);
int sem_destroy(sem_t *sem);
int sem_wait(sem_t *sem);
int sem_post(sem_t *sem);