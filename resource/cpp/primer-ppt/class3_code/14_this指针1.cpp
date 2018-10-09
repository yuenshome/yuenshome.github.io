#include <iostream>
#include <string.h>
using namespace std;
class A {
public:
	A(int num) { A::num = num; }
	A(double num) { this->num = num; }
	int num; 
};
int main() {
	A a1(20);
	cout << a1.num << endl; //20
	A a2(1.2);
	cout << a2.num << endl; //1
	return 0;
}