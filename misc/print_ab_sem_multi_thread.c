#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <semaphore.h>

int amount = 20;

sem_t sem_a, sem_b;


void *threadFuncA(void *par) {

	int *id = (int *)par;
	int cnt = 0;
	while (cnt < 100) {
		while (sem_wait(&sem_a) == -1)
			continue;
		printf("a"); 	
		sem_post(&sem_b);
		cnt++;
	}
}

void *threadFuncB(void *par) {

	int *id = (int *)par;

	int cnt = 0;
	while (cnt < 100) {
		while (sem_wait(&sem_b) == -1)
			continue;
		printf("b "); 	
		sem_post(&sem_a);
		cnt++;
	}
}


void main() {

	pthread_t th_id[100], th_id2[100];
	int id1, id2, i = 0;
	pthread_attr_t attr;
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	sem_init(&sem_a, 0, 1);
	sem_init(&sem_b, 0, 0);
	id1 = i;
	pthread_create(&th_id[i], &attr, threadFuncA, (void *)&id1);
	id2 = 200 + i;
	pthread_create(&th_id2[i], &attr, threadFuncB, (void *)&id2);
	//sem_post(&sem_a);

	pthread_join(th_id[i], NULL);
	pthread_join(th_id2[i], NULL);
	pthread_attr_destroy(&attr);
	printf("\nDONE\n");
}
