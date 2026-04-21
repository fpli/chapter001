/**
 * 
 */
#include <iostream>
#include <vector>
#include <fstream>

int age = 10;

class person {
    public:
        person() {
            std::cout << "person constructor" << std::endl;
        }

        person(int age) {
            std::cout << "person constructor with age" << std::endl;
            this->age = age;
        }

        person(const person &p) {
            std::cout << "person copy constructor" << std::endl;
            this->age = p.age;
        }

        ~person() {
            std::cout << "person destructor" << std::endl;
        }

        int age;

};

namespace aa {
    int a = 20;
}

namespace aa {
    int b = 30;
}

using namespace std;

int main(int argc, char const *argv[])
{
    cout << aa::a << endl;
    cout << aa::b << endl;
    person p1(10);
    //person p2 = p1;
    vector<person> v;
    v.push_back(p1);

    int age = 200;
    cout << age << endl;
    cout << ::age << endl;
    return 0;
}
