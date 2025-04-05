#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

struct person
{
    /* data */
    int id;
    char name[256];
    int age;
};


int main(int argc, char const *argv[])
{
    /* code */
    int fd = open("student", O_CREAT|O_RDWR|O_TRUNC, 0644);
    size_t s = sizeof(struct person);
    ftruncate(fd, s);
    struct person *p = mmap(NULL, s, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    close(fd);

    struct person person = {1, "王宝强", 20};
    for (size_t i = 0; i < 100000; i++)
    {
        /* code */
        memcpy(p, &person, s);
        person.id++;
        sleep(1);
    }
    
    unlink("student");

    return 0;
}
