//#define _XOPEN_SOURCE
#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

void signalhandler(int);

void catch_sigchd(int signal)
{
   printf("--------sigchld---------\n");
   int status;
   pid_t wpid;
   while ((wpid = wait(&status)) != -1)
   {
      /* code */
      printf("pid = %d \n", wpid);

      if (WIFEXITED(status))
      {
         /* code */
         WEXITSTATUS(status);
         printf("child dead normally \n");
      }
      else if (WIFSIGNALED(status))
      {
         /* code */
         int n = WTERMSIG(status);
         printf("pid = %d  signal = %d \n", wpid, n);
      }
   }
   
   // while ((wpid = waitpid(-1, &status, WNOHANG)) != -1)
   // {
   //    /* code */
   //    if (wpid == 0) 
   //       continue;
   //    printf("pid = %d \n", wpid);

   //    if (WIFEXITED(status))
   //    {
   //       /* code */
   //       WEXITSTATUS(status);
   //       printf("child dead normally \n");
   //    }
   //    else if (WIFSIGNALED(status))
   //    {
   //       /* code */
   //       int n = WTERMSIG(status);
   //       printf("pid = %d  signal = %d \n", wpid, n);
   //    }
   // }

   if (wpid == -1)
   {
      /* code */
      perror("waitpid");
   }
   
}

int main () {

   // first of all, block signal for child process
   sigset_t set;
   sigemptyset(&set);
   sigaddset(&set, SIGCHLD);

   sigprocmask(SIG_BLOCK, &set, NULL);

   int i, ret;
   for (i = 0; i < 15; i++)
   {
      /* code */
      ret = fork();
      if (ret == 0)
      {
         break;
      }

   }
   
   if (i == 15)
   {
      printf("I'm parent.\n");

      struct sigaction act, oact;
      sigemptyset(&act.sa_mask);
      sigaddset(&act.sa_mask, SIGCHLD);
      act.sa_handler = catch_sigchd;
      act.sa_flags = 0;

      sigaction(SIGCHLD, &act, &oact);

      // unblock signal
      sigprocmask(SIG_UNBLOCK, &set, NULL);

      sleep(10);
   } 
   else 
   {
      sleep(2);
      printf("child. i = %d \n", i);
   }



   //signal(SIGINT, signalhandler);

   

   // while(1) {
   //    printf("Going to sleep for a second...\n");
   //    sleep(1); 
   // }
   return 0;
}

void signalhandler(int signalnum) {
   printf("Caught signal %d, coming out...\n", signalnum);
   exit(1);
}