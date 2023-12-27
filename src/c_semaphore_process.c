#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

int main(int argc, char **argv)
{
    int fd, count = 0, loop = 10;
    void *ptr;
    pid_t pid;
    sem_t sem;

    fd = open("log.txt", O_RDWR | O_CREAT, S_IRWXU);
    write(fd, &count, sizeof(int));
    ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    sem_init(&sem, 1, 1);
    pid = fork();
    if (0 == pid)
    {
        for (int i = 0; i < loop; i++)
        {
            sem_wait(&sem);
            printf("child %d: %d\n", getpid(), (*(int *)ptr)++);
            sleep(rand() % 3);
            sem_post(&sem);
        }
    }
    else
    {
        for (int i = 0; i < loop; i++)
        {
            sem_wait(&sem);
            printf("parent %d: %d\n", getpid(), (*(int *)ptr)++);
            sleep(rand() % 3);
            sem_post(&sem);
        }
    }
    sem_destroy(&sem);
    return 0;
}
