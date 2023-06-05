#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

int amount = 20;

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv_a = PTHREAD_COND_INITIALIZER;
pthread_cond_t cv_b = PTHREAD_COND_INITIALIZER;

bool flaga = true, flagb=false;

void *threadFuncA(void *par) {

	int *id = (int *)par;
	int cnt = 0;
	while (cnt < 100) {
		pthread_mutex_lock(&mtx);
		while (!flaga)
			pthread_cond_wait(&cv_a, &mtx);
		flaga = false;
		printf("a"); 	
		flagb = true;
		pthread_cond_signal(&cv_b);
		pthread_mutex_unlock(&mtx);
		cnt++;
	}
}

void *threadFuncB(void *par) {

	int *id = (int *)par;

	int cnt = 0;
	while (cnt < 100) {
		pthread_mutex_lock(&mtx);
		while (!flagb)
			pthread_cond_wait(&cv_b, &mtx);
		flagb = false;
		printf("b "); 	
		flaga = true;
		pthread_cond_signal(&cv_a);
		pthread_mutex_unlock(&mtx);
		cnt++;
	}
}


void main() {

	pthread_t th_id[100], th_id2[100];
	int id1, id2, i = 0;
	pthread_attr_t attr;
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	id1 = i;
	pthread_create(&th_id[i], &attr, threadFuncA, (void *)&id1);
	id2 = 200 + i;
	pthread_create(&th_id2[i], &attr, threadFuncB, (void *)&id2);
	pthread_cond_signal(&cv_a);

	pthread_join(th_id[i], NULL);
	pthread_join(th_id2[i], NULL);
	pthread_attr_destroy(&attr);
	pthread_cond_destroy(&cv_a);
	pthread_cond_destroy(&cv_b);
	printf("\nDONE\n");
}
