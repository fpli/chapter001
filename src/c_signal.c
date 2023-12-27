#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void signalhandler(int);

int main () {
   signal(SIGINT, signalhandler);

   while(1) {
      printf("Going to sleep for a second...\n");
      sleep(1); 
   }
   return(0);
}

void signalhandler(int signalnum) {
   printf("Caught signal %d, coming out...\n", signalnum);
   exit(1);
}