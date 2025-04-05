#include <stdio.h>
#include <string.h>
#include <strings.h>

int main(int argc, const char *argv[])
{
    if (argc < 2)
    {
        return 0;
    }
    FILE *fp = fopen(argv[1], "r");
    char buf[1024];
    memset(buf, 0, 1024);
    fgets(buf, 1024, fp);
    printf("%s \n", buf);
    while (!feof(fp))
    {
        memset(buf, 0, 1024);
        fgets(buf, 1024, fp);
        printf("%s \n", buf);
    }

    if (NULL != fp)
    {
        fclose(fp);
    }
    return 0;
}

void readFile(const char *filename)
{
    FILE *f = fopen(filename, "r");
    char buffer[4096];
    bzero(buffer, sizeof(buffer));
    
    while (fgets(buffer, 4096, f) != NULL)
    {
        /* code */
        printf("%s", buffer);
        memset(buffer, 0, 1024);
    }

    if (NULL != f)
    {
        fclose(f);
    }
}