#include <iostream>
#include <string>
using namespace std;
class Base {
public:
	void f() { cout << "Base f()" << endl; }
	void f(double d) { cout << "Base f(double)" << endl; }
};
class D1:public Base  {
public:
	void f(int i) { cout << "D1 f(int)" << endl; }
	void f(const string &s) { cout << "D1 f(string)" << endl; }
private:
	void f(short i) { cout << "D1 f(short)" << endl; }
};
int main() {
	Base b1;
	b1.f();  // Base f()
	b1.f(1.2); // Base f(double)
	D1 d1;
	d1.f(10);// D1 f(int)
	
	//d1.f(static_cast<short>(10)); //错,private类外无法访问
	//d1.f(); //错,Base继承下来的f()被遮掩(shadow)
	d1.Base::f(); //要调用Base的f(),要加Base::
	d1.Base::f(1);//Base f(int)

	//静态类型与动态类型不同时：
	Base &rb = d1;
	rb.f();  // Base f()
	rb.f(10);// Base f(double) 隐式转换10->10.0
	//rb.f(string("abc")); //错,Base中没有
	Base *pb = &d1;
	pb->f();	 // Base f()
	pb->f(10.0); // Base f(double)
	//pb->f(string("abc")); //错,Base中没有
	return 0;
}