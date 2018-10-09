#include <iostream>
#include <stdio.h>
using namespace std;
class A {
public:
	A(int i = 0) {
		num = i;
		cout << "A()" << num << endl;
	}
	~A() { cout << "~A()" << num << endl;	}
private:
	int num;
};
class B {
public:
	B() { cout << "B()" << endl; }
	~B() { cout << "~B()" << endl; }
};
int main() {
	A a1;
	A a2(1);
	B b;
	A *pa = new A[2]{ 2,3 }; //连续构造2次
	delete[] pa;             //连续析构2次
	return 0;
}