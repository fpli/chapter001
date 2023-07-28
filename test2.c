#include <stdio.h>
#include <unistd.h>

#include <stdlib.h>

extern char **environ; // from unistd.h

int main(int argc, char *argv[])
{
    char **p = environ;
    while (*p != NULL)
    {
        printf("%s (%p)\n", *p, *p);
        *p++;
    }

    printf("------- \n");
    char *pathvar;
    pathvar = getenv("PATH"); // getenv from stdlib.h
    printf("pathvar=%s",pathvar);


    int a[] = {12, 30, 40, 50};
    int *ptr = a;
    printf("\n sizeof a: %lu", sizeof(a));
    printf("\n sizeof ptr: %lu", sizeof(ptr));
    char str1[500] = {'a', 'b'};
    char * str2 = "hello world";
    printf("\n sizeof str1: %lu", sizeof(str1));
    printf("\n sizeof str2: %lu", sizeof(str2));
    printf("\n");
    return 0;
}