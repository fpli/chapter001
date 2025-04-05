#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

pthread_mutex_t mutex;

void print_pid_tid()
{
    printf("pid = %d, tid = %lu \n", getpid(), pthread_self());
}

void *tfn(void *arg)
{
    srand(time(NULL));
    while (1)
    {
        pthread_mutex_lock(&mutex);
        print_pid_tid();
        printf("hello ");
        sleep(rand() % 3);
        printf("world\n");
        pthread_mutex_unlock(&mutex);
        sleep(rand() % 3);
    }
    return NULL;
}


int main(int argc, char const *argv[])
{
    pthread_mutex_init(&mutex, NULL);
    pthread_t tid;
    pthread_create(&tid, NULL, tfn, NULL);
    srand(time(NULL));
    while (1)
    {
        pthread_mutex_lock(&mutex);
        print_pid_tid();
        printf("HELLO ");
        sleep(rand() % 3);
        printf("WORLD\n");
        pthread_mutex_unlock(&mutex);
        sleep(rand() % 3);
    }
    pthread_join(tid, NULL);
    return 0;
}