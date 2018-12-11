#include "monitor.h"

void simulate_thinking() { spend_some_time(100); }
void simulate_eating() { spend_some_time(10); }

void pick_up(monitor_t *mon, int phil_nr) {
	pthread_mutex_lock(&mon->m);
	mon->chop_choice[phil_nr] = 0;
	mon->state_phil[phil_nr] = HUNGRY;
	printf("%*d : HUNGRY\n", phil_nr*6, phil_nr);

	int left_pos = phil_nr;
	int right_pos = (phil_nr+1)%N_THREADS;

	int chop_ll = phil_nr*2;
	int chop_l = phil_nr*2 + 1;
	int chop_r = (phil_nr*2 + 2)%N_CHOPS;
	int chop_rr = (phil_nr*2 + 3)%N_CHOPS;
	//printf("%*d: mon->chop_pair[left_pos]]: %d 		mon->chop_pair[right_pos]: %d \n",phil_nr*6, phil_nr, mon->chop_pair[left_pos], mon->chop_pair[right_pos]);
	while (!(mon->chop_pair[left_pos] + mon->chop_pair[right_pos] >= 3)){
		pthread_cond_wait(&mon->cv_phil[phil_nr],&mon->m);
	}
	if(mon->chop_pair[left_pos] == 2){
		mon->chop_choice[phil_nr] = 1;
		mon->chop_pair[left_pos] = 0;
		mon->chop_pair[right_pos] --;
		printf("%*d: Picked up chopstick %d, %d, and %d\n", phil_nr*6, phil_nr, chop_ll, chop_l, chop_r);
	}
	else {
		mon->chop_choice[phil_nr] = 2;
		mon->chop_pair[left_pos] --;
		mon->chop_pair[right_pos] = 0;
		printf("%*d: Picked up chopstick %d, %d, and %d\n",phil_nr*6, phil_nr, chop_l, chop_r, chop_rr);
	}
	mon->state_phil[phil_nr] = EATING;
	printf("%*d    EATING\n",phil_nr*6, phil_nr);
	pthread_mutex_unlock(&mon->m);
}

void put_down(monitor_t *mon, int phil_nr) {
	pthread_mutex_lock(&mon->m);

	int left_pos = phil_nr;
	int right_pos = (phil_nr+1)%N_THREADS;
	int left_phil = (phil_nr+(N_THREADS -1)%N_THREADS);
	int right_phil = right_pos;

	int chop_ll = phil_nr*2;
	int chop_l = phil_nr*2 + 1;
	int chop_r = (phil_nr*2 + 2)%N_CHOPS;
	int chop_rr = (phil_nr*2 + 3)%N_CHOPS;

	int mode = mon->chop_choice[phil_nr];
	switch(mode){
		case 1:
			mon->chop_pair[left_pos] = 2;
			mon->chop_pair[right_pos] ++;
			printf("%*d: Put down chopstick %d, %d, and %d\n", phil_nr*6, phil_nr, chop_ll, chop_l, chop_r);
			pthread_cond_broadcast(&(mon->cv_phil[left_phil]));
			pthread_cond_broadcast(&(mon->cv_phil[right_phil]));
			mon->state_phil[phil_nr] = THINKING;
			printf("%*d : THINKING\n",phil_nr*6, phil_nr);
			break;
		case 2:
			mon->chop_pair[left_pos] ++;
			mon->chop_pair[right_pos] = 2;
			printf("%*d: Put down chopstick %d, %d, and %d\n", phil_nr*6, phil_nr, chop_ll, chop_l, chop_r);
			pthread_cond_broadcast(&(mon->cv_phil[left_phil]));
			pthread_cond_broadcast(&(mon->cv_phil[right_phil]));
			mon->state_phil[phil_nr] = THINKING;
			printf("%*d : THINKING\n",phil_nr*6, phil_nr);
			break;
		default:
			printf("%*d: something went wrong\n", phil_nr*6, phil_nr);
	}
	pthread_mutex_unlock(&mon->m);
}

void monitor_init(monitor_t *mon) {
 pthread_mutex_init(&mon->m,NULL);
	for (int i=0; i<N_THREADS; i++){
		pthread_cond_init(&mon->cv_phil[i],NULL);
		mon->chop_pair[i] = 2;
	}
}

void monitor_destroy(monitor_t *mon) {
    for(int i = 0; i < N_THREADS; i++){
    	pthread_cond_destroy(&mon->cv_phil[i]);
	}
	pthread_mutex_destroy(&mon->m);
}

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

void *philosopher_runner(void *arg) {
	char *thread_name = arg;
	int phil_nr = atoi(thread_name);
	printf("%*d : THINKING\n", phil_nr*6, phil_nr);
	FOREVER {
		simulate_thinking();
		pick_up(&mon, phil_nr);
		simulate_eating();
		put_down(&mon, phil_nr);
	}
	pthread_exit(NULL);
}
