#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

sem_t semaphore;
int number = 1;
int count = 10;

void *thread_run_1(void *arg)
{
    for (size_t i = 0; i < count; i++)
    {
        sem_wait(&semaphore);

        number++;
        printf("thread %lu run one, number: %d \n", pthread_self(), number);

        sem_post(&semaphore);
        sleep(1);
    }
}

void *thread_run_2(void *arg)
{
    for (size_t i = 0; i < count; i++)
    {
        sem_wait(&semaphore);

        number--;
        printf("thread %lu run two, number: %d \n", pthread_self(), number);
        
        sem_post(&semaphore);
        sleep(1);
    }
}

int main(int argc, char const *argv[])
{
    sem_init(&semaphore, 0, 1);

    pthread_t tid1, tid2;

    pthread_create(&tid1, NULL, thread_run_1, NULL);
    pthread_create(&tid2, NULL, thread_run_2, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    sem_destroy(&semaphore);
    return 0;
}
