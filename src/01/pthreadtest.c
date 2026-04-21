
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>

void *t_run(void *arg)
{
    printf("thread is running before sleep ... \n");
    sleep(10);
    printf("thread is running\n");
    int *p = (int *)malloc(sizeof(int));
    *p = 200;
    pthread_exit((void *)p);
}

int main(int argc, char const *argv[])
{
    pid_t pid = fork();
    if (pid == 0)
    {
        printf("child process\n");
        pthread_t tid;
        int ret = pthread_create(&tid, NULL, (void *)t_run, NULL);
        if (ret != 0)
        {
            perror("pthread_create");
            exit(1);
        }
        // int *r = NULL;
        // ret = pthread_join(tid, (void **)&r);
        // if (ret != 0)
        // {
        //     perror("pthread_join");
        //     exit(1);
        // }
        // printf("thread return: %d\n", *r);
        // free(r);
        // int *p = (int *)malloc(sizeof(int));
        // *p = 25;
        // pthread_exit((void *)p);
        // printf("pthread_exit --- \n");
        exit(25);
        return 0;
    } 
    else if (pid > 0)
    {
        int status;
        pid = wait(&status);
        if (WIFEXITED(status))
        {
            printf("child process exit code: %d\n", WEXITSTATUS(status));
        } else {
            printf("child process exit abnormally %d \n", status);
            perror("wait");
        }
        char *home = getenv("HOME");
        printf("HOME: %s\n", home);
        char *path = getenv("PATH");
        printf("PATH: %s\n", path);
        return 0;
    }
}
