#include <iostream>
using namespace std;

struct A {
	A() { cout << "A 构造\n"; }
	~A() { cout << "A 析构\n"; }
};
struct B {
	B() { cout << "B---构造\n"; }
	~B() { cout << "B---析构\n"; }
};
void f2() {
	A a1;
	A a2;
	B* pb1 = new B;
	cout << "f2(): before throw..\n";
	throw 1;
	delete pb1; //不会执行到
}
void f1() {
	A a3;
	B* pb2 = new B;
	cout << "--------------\n";
	try {
		f2();
	}
	catch (double e) {
		cout << "f1(): err_double\n";
	}
	catch (const char* e) {
		cout << "f1(): err_char*\n";
	}
	cout << "f1():2\n";
	delete pb2; //这里有没有被执行到？
}

int main() {
	try {
		f1();
	}
	catch(int e){
		cout << "main(): err_int\n";
	}
	return 0;
}