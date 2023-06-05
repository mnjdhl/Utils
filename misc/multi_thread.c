#include <stdio.h>
#include <pthread.h>

int amount = 20;

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void *threadFuncCredit(void *par) {

	int *id = (int *)par;

	pthread_mutex_lock(&mtx);
	amount = amount + 5;
	pthread_mutex_unlock(&mtx);
	printf("\nId = %d, amount = %d", *id, amount); 	
}

void *threadFuncDebit(void *par) {

	int *id = (int *)par;

	pthread_mutex_lock(&mtx);
	amount = amount - 4;
	pthread_mutex_unlock(&mtx);
	printf("\nId = %d, amount = %d", *id, amount); 	
}


void main() {

	pthread_t th_id[100], th_id2[100];
	int id1, id2;
	pthread_attr_t attr;
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	for (int i=0;i<100;i++)
	{
		id1 = i;
		pthread_create(&th_id[i], &attr, threadFuncCredit, (void *)&id1);
		id2 = 200 + i;
		pthread_create(&th_id2[i], &attr, threadFuncDebit, (void *)&id2);
	}

	pthread_attr_destroy(&attr);
	for (int i=0;i<100;i++)
	{
		pthread_join(th_id[i], NULL);
		pthread_join(th_id2[i], NULL);
	}
	printf("\nDONE\n");
	//pthread_exit(NULL);
}
