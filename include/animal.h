#include "common.h"

class animal;

void f1(animal &a)
{
    a.eat();
    cout << a.weight << endl;
}

class animal
{

friend void f1(animal &a);

private:
    char name[20];
    int age;
    int weight;
    double height;
public:
    animal(/* args */);
    ~animal();

    virtual void eat() = 0;
};

animal::animal(/* args */)
{
}

animal::~animal()
{
}
