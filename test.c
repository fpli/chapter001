#include <stdio.h>
int a ;
int a;
int b;

int b;

void f(){
     printf("In file:%s, function:%s() and line:%d \n",
           __FILE__, __func__, __LINE__);
    printf("b = %d \n", b);
}