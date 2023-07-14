#include <stdio.h>
#include <iostream>

using namespace std;

template<class T1, class T2>
class Person;

template<class T1, class T2>
void printPerson2(const Person<T1, T2> &p)
{
    cout << "name: " << p.name << " age: " << p.age << endl;
}

template<class T1, class T2>
class Person {

    friend void printPerson(const Person<T1, T2> &p)
    {
        cout << p.name << p.age << endl;
    }

    friend void printPerson2<>(const Person<T1, T2> &p);


    friend void printFive(const Person<T1, T2> &p)
    {
        cout << "num:" << 5 << endl;
    }

    private:
        T1 name;
        T2 age;

    public:
        Person(T1 name, T2 age)
        {
            this->name = name;
            this->age = age;
        }

};

void f2(int &a){
    printf("a = %d \n", a);
    a = 50;
}

void test01()
{
    Person<string, int> p("Tom", 30);
    printPerson(p);
    printFive(p);
}

void test02()
{
    Person<string, int> p("Amy", 30);
    printPerson2(p);
}
 
class Box
{
   public:
      Box() { 
         cout << "invoke constructor function." <<endl;
      }
      ~Box() { 
         cout << "invoke destroy function." <<endl;
      }
};

int main(int argc, char const *argv[])
{
    test01();
    test02();

    Box* myBoxArray = new Box[4];
 
    delete [] myBoxArray; // 删除数组

    return 0;
}
