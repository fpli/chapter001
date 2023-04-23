#include <stdio.h>
int a;

int b;
int b = 50;

void f();

int main(int argc, char const *argv[])
{
    /* code */
    f();
    int i ;
    i = 50;
    
    const volatile int local = 10;
    int* ptr = (int*)&local;
    printf("Initial value of local : %d \n", local);
    *ptr = 100;
    printf("Modified value of local: %d \n", local);
    return 0;
}
