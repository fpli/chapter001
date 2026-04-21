
#include <stdio.h>
#include <unistd.h>

void f(int *a)
{
    while (a != NULL)
    {
        printf("a: %d\n", *a);
        a++;
    }
}

int main(int argc, char const *argv[])
{
    int a[5] = {1, 2, 3, 4, 5};
    //f(a);
    const int b = 20;
    int *p = (int *)&b;
    *p = 30;
    printf("b------");
    sleep(10);
    printf("*p: %d\n", *p);
    printf("b: %d\n", b);
    printf("&b: %p\n", &b);
    printf("p: %p\n", p);
    return 0;
}
