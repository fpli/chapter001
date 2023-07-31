#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include <string.h>
#include <errno.h>

/**
 * need debug, not expected
*/
int main(int argc, char **argv)
{
    int fd, count = 0, loop = 50;
    void *ptr;
    pid_t pid;
    sem_t sem1, sem2;

    fd = open("c_semaphore_process_sync.txt", O_RDWR | O_CREAT, S_IRWXU);
    write(fd, &count, sizeof(int));
    ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    srand(time(NULL));

    sem_init(&sem1, 1, 1);
    sem_init(&sem2, 1, 0);

    pid = fork();
    if (0 == pid)
    {
        for (int i = 0; i < loop; i++)
        {
            sem_wait(&sem1);
            printf("child %d: %d\n", getpid(), (*(int *)ptr)++);
            // sleep(rand() % 1);
            sem_post(&sem2);
            sleep(rand() % 1);
        }
    }
    else if (pid > 0)
    {
        for (int i = 0; i < loop; i++)
        {
            sem_wait(&sem2);
            printf("parent %d: %d\n", getpid(), (*(int *)ptr)++);
            // sleep(rand() % 1);
            sem_post(&sem1);
            sleep(rand() % 1);
        }
    } else {
        perror("system call failed:");
        strerror(errno);
    }

    sem_destroy(&sem1);
    sem_destroy(&sem2);
    return 0;
}
