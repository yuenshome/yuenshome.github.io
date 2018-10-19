#include <iostream>
#include <string>
using namespace std;
class Base {
public:
	void f1_base() {}
	void f2_base() {}
	int b_int1;
	int b_int2;
};
class D1 :public Base {
public:
	void f1_d1(){}
	int d1_int;
};
class D2 :public D1 {
public:
	void f2_d2() {}
	int d2_int;
};
int main() {
	Base b;
	D1 d1;
	D2 d2;
	cout << sizeof(b) << "," 
		<< sizeof(d1) << "," 
		<< sizeof(d2) << endl;
	d2.f1_base();
	d2.f2_base();
	d2.f1_d1();
	d2.f2_base();
	d2.b_int1 = 1;
	d2.b_int2 = 2;
	d2.d1_int = 3;
	d2.d2_int = 4;
	return 0;
}