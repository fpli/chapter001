#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

struct ret
{
    int a;
    long b;
    char g[256];
};


int main()
{
    int *p;
    char *l = "fgdgdffdfdfdsfvddfvdvdsvf";
    struct ret *m;
    m = (struct ret *)malloc(sizeof(struct ret));
    m->a = 30;
    m->b = 100L;
    strcpy(m->g, "gfgfgfg-hello, good afternoon. english is very good for us.");
    printf("%lu, %lu, %lu, %s \n", sizeof(p), sizeof(l), sizeof(m), m->g);
    return 0;
}