#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<sys/wait.h>

int global_var = 100;

int main(int argc, char const *argv[])
{
    int fd = open("temp", O_CREAT|O_RDWR|O_TRUNC, 0644);
    if (fd < 0){
        perror("open");
        return -1;
    }

    int *p;

    ftruncate(fd, 4);
    /* code */
    p =  (int *)mmap(NULL, 4, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    //p =  (int *)mmap(NULL, 4, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (p == MAP_FAILED){
        perror("map");
        return -1;
    }
    printf("*p = %d, p = %p \n", *p, p);

    close(fd);

    pid_t pid = fork();
    if (pid == 0){
        *p = 2000;
        global_var = 200;
        printf("pid = %d, *p = %d, p = %p, global_var = %d, var add = %p \n", getpid(), *p, p, global_var, &global_var);
        exit(255);
    } else {
        global_var = 300;
        *p = 5000;
        sleep(1);
        printf("pid = %d, *p = %d, p = %p, global_var = %d, var add = %p \n", getpid(), *p, p, global_var, &global_var);
        munmap(p, 4);
        unlink("temp");
        int status;
        pid = wait(&status);
        // pid = waitpid(-1, &status, WNOHANG);
        if (status == -1){
            perror("wait error");
            exit(0);
        }

        if (WIFEXITED(status))
        {
            /* code */
            printf("child pid: %d, child status: %d \n", pid, WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            /* code */
            int sigNum = WTERMSIG(status);
            printf("child process %d, signal number: %d \n", pid, sigNum);
        } 
        else if (WIFSTOPPED(status))
        {
            /* code */
            int stop = WSTOPSIG(status);
            printf("child process %d, stop number: %d \n", pid, stop);
        }
        else
        {
            /* code */
            printf("unknowen error \n");
        }
    
        
    }
    return 0;
}
