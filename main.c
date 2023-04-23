#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int a = 10;
char *str = "global variable";

static int e = 2;

void* fun(void * argv)
{
    printf("hello thread\n");
    pthread_exit(NULL);
}

int getArea()
{

    return 20;
}


int main(int argc, char const *argv[], char * envp[])
{
    /* code */
    printf("a = %d\n", 10);
    for (int i = 0; ; i++)
    {
        if (envp[i] == NULL){
            break;
        }
        printf("env variable = %s\n", envp[i]);
    }

    pthread_t tid;
    pthread_create(&tid, NULL, fun, NULL);
    sleep(2);
    return 0;
}
