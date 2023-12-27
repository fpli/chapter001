#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

struct node
{
    int value;
    struct node *next;
};

struct node *head;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

void *producer(void *arg)
{
    struct node *temp;
    while (1)
    {
        temp = malloc(sizeof(struct node));
        temp->value = rand() % 1000 + 1;
        printf("thread %lu produce %d \n", pthread_self(), temp->value);
        pthread_mutex_lock(&mutex);

        temp->next = head;
        head = temp;

        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mutex);

        sleep(rand() % 3);
    }
    return NULL;
}

void *consumer(void *arg)
{
    struct node *temp;
    for (;;)
    {
        pthread_mutex_lock(&mutex);

        while (NULL == head)
        {
            pthread_cond_wait(&not_empty, &mutex);
        }

        temp = head;
        head = head->next;

        pthread_mutex_unlock(&mutex);

        printf("thread %lu comsume %d \n", pthread_self(), temp->value);

        free(temp);

        sleep(rand() % 2);
    }

    return NULL;
}

int main(int argc, char const *argv[])
{

    pthread_t pid, cid;

    srand(time(NULL));

    pthread_create(&pid, NULL, producer, NULL);
    pthread_create(&cid, NULL, consumer, NULL);

    pthread_join(pid, NULL);
    pthread_join(cid, NULL);

    return 0;
}
