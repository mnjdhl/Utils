#include <stdio.h>
#include <pthread.h>

#define THREAD_NUMS 4
pthread_barrier_t barrier;

void *t0(void *param)
{
    pthread_barrier_wait(&barrier);
    printf("t0 ready\n");
}
void *t1(void *param)
{
    pthread_barrier_wait(&barrier);
    printf("t1 ready\n");
}
void *t2(void *param)
{
    pthread_barrier_wait(&barrier);
    printf("t2 ready\n");
}

int main(void)
{
    pthread_t t[3];

    pthread_barrier_init(&barrier, NULL, THREAD_NUMS);

    pthread_create(&t[0], NULL, t0, NULL);
    pthread_create(&t[1], NULL, t1, NULL);
    pthread_create(&t[2], NULL, t2, NULL);

    pthread_barrier_wait(&barrier);
    printf("\nall sub threads ready, go!\n");
    pthread_join(t[0], NULL);
    pthread_join(t[1], NULL);
    pthread_join(t[2], NULL);
    pthread_barrier_destroy(&barrier);

}
