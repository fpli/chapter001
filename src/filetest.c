#include <stdio.h>
#include <string.h>

int main(int argc, const char *argv[])
{
    if (argc < 2){
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
    
    if (NULL != fp){
        fclose(fp);
    }
    return 0;
}