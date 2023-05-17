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
    return 0;
}