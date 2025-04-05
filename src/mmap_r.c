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
    int fd = open("student", O_RDONLY, 0644);
    size_t s = sizeof(struct person);
    //ftruncate(fd, s);
    struct person *p = mmap(NULL, s, PROT_READ, MAP_SHARED, fd, 0);

    close(fd);

    for (size_t i = 0; i < 100000; i++)
    {
        /* code */
        printf("id: %d, name: %s, age: %d \n", p->id, p->name, p->age);
        sleep(1);
    }
    
    unlink("student");

    return 0;
}