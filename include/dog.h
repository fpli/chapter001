#include "animal.h"
#include <stdio.h>

class dog: public animal
{
private:
    /* data */
public:
    dog(/* args */);
    ~dog();

    void eat()
    {
        printf("dog eat\n");
    }
};

dog::dog(/* args */)
{
}

dog::~dog()
{
}

