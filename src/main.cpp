#include <iostream>

#include "../include/dog.h"

void test01()
{
    std::cout << "test01" << std::endl;
}

int main(int argc, char const *argv[], char const *envp[])
{
    std::cout << "Hello World!" << std::endl;
    dog d;
    d.eat();
    return 0;
}