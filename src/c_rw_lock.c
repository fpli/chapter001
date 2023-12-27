#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int counter;
pthread_rwlock_t rwlock;

void *th_write(void *arg)
{
    int i = (int)arg;
    int c;
    while (1)
    {
        pthread_rwlock_wrlock(&rwlock);
        c = counter;
        usleep(1000);
        printf("thread %lu - %d write %d \n", pthread_self(), i, counter++);
        pthread_rwlock_unlock(&rwlock);
        usleep(10000);
    }
    return NULL;
}

void *th_read(void *arg)
{
    int i = (int)arg;
    while (1)
    {
        pthread_rwlock_rdlock(&rwlock);
        printf("thread %lu - %d read %d \n", pthread_self(), i, counter);
        pthread_rwlock_unlock(&rwlock);
        usleep(2000);
    }
    return NULL;
}


int main(int argc, char const *argv[])
{
    int i;
    pthread_t tid[8];

    pthread_rwlock_init(&rwlock, NULL);

    for (i = 0; i < 3; i++)
    {
        pthread_create(&tid[i], NULL, th_write, (void *)i);
    }
    
    for (i = 0; i < 5; i++)
    {
        pthread_create(&tid[3 + i], NULL, th_read, (void *)i);
    }
    

    for (i = 0; i < 8; i++)
    {
        pthread_join(tid[i], NULL);
    }
    
    pthread_rwlock_destroy(&rwlock);

    return 0;
}
