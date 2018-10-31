#include <iostream>
using namespace std;

class Base1 {
public:
	Base1() { cout << "Base1\n"; }
	virtual void f1() {}
	virtual void f2() {}
	int i = 1;
};
class Base2 {
public:
	Base2() { cout << "Base2\n"; }
	virtual void f2() {}
	virtual void f3() {}
	int j = 2;
};
class Base3 { //Ã»ÓÐÐéº¯Êý
public:
	Base3() { cout << "Base3\n"; }
	void fun(){}
	int k = 3;
};
class D1 :public Base1 {
public:
	D1() { cout << "D1\n"; }
	virtual void f2()override {}
	virtual void f4() {}
	int m = 4;
};
class Ok :public Base3,public D1,public Base2 {
public:
	Ok() { cout << "Ok\n"; }
	virtual void f2()override {}
	virtual void f5() {}
	int n = 5;
};
int main() {
	Ok ok;
	cout << "&ok: " << &ok << endl;
	cout << &ok.i << " " << &ok.j << " " << &ok.k 
		<< " " << &ok.m << " " << &ok.n << endl;
	Base1 *pb1 = &ok;
	Base2 *pb2 = &ok;
	Base3 *pb3 = &ok;
	cout << "pb1: " << pb1 << endl;
	cout << "pb2: " << pb2 << endl;
	cout << "pb3: " << pb3 << endl;
	cout << sizeof(ok) << endl;
	return 0;
}


