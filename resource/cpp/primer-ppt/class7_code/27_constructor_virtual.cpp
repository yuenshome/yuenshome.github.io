#include <iostream>
using namespace std;
class Base {
public:
	Base() {
		this->f2();   //就是 f2();
		this->func(); //就是 func();
		//这里调用虚函数，只会调用本类的，而不会是派生类的
		cout <<"typeid: "<< typeid(*this).name() << endl; //Base
	}
	virtual void func() { cout << "Base func()" << endl; }
	void f2() { cout << "Base f2()" << endl; }
	virtual ~Base() {}
private:
	int i;
};
class Derived :public Base {
public:
	Derived() :Base() {
		f2();
		func(); //等价于 this->func();
		cout << "typeid: " << typeid(*this).name() << endl; //Derived
	}
	virtual void func() override{ cout << "Derived func()" << endl; }
	void f2() { cout << "Derived f2()" << endl; }
private:
	int j;
};
int main() {
	Derived d1;
	return 0;
}