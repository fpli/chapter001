/**
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <syslog.h>
#include <sys/wait.h>

/**
 * memory alignment: 8bytes
 */
struct __attribute__ ((aligned(8))) student
{
    int age;
    char name[21];
};

/**
 * memory alignment: 1bytes, no padding
 */
struct __attribute__ ((__packed__)) teacher
{
    int age;
    char name[21];
};

/**
 * memory alignment: 4bytes, default
 */
struct student2
{
    int age;
    char name[21];
};

int main(int argc, char const *argv[])
{
    printf("%lu \n", sizeof(struct student));
    printf("%lu \n", sizeof(struct teacher));
    printf("%lu \n", sizeof(struct student2));

    pid_t pid, sid;
    pid = fork();
    if (pid == 0)
    {
         setsid();
         sleep(5);
         return 20;
    }
    
   
    int ret;
    wait(&ret);
    if (WIFEXITED(ret))
    {
        printf("Child exited with status %d\n", WEXITSTATUS(ret));
    }
    return 0;
}
