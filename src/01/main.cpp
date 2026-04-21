/**
 * 
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <set>
using namespace std;

class Solution {
    public: 
        Solution() {
            cout << "Solution constructor" << endl;
        }

        Solution(int age) {
            cout << "Solution constructor with age" << endl;
            this->age = age;
        }

        Solution(const Solution &s) {
            cout << "Solution copy constructor" << endl;
            this->age = s.age;
        }

        ~Solution() {
            cout << "Solution destructor" << endl;
        }
        int age;
};

void test(Solution s1) {
    Solution s2 = s1;
}

int main(int argc, char const *argv[])
{
    char *p = "hello";
    cout << p << sizeof(p) << endl;
    Solution s1(10);
    Solution s2 = s1;
    test(s1);
    cout << "!!10: " << !!10 << endl;
    return 0;
}
