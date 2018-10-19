#include <iostream>
using namespace std;
struct Base {
	void f() { cout << "Base f()" << endl; }
};
struct D1: Base  {
	void f(int i) { cout<<"D1 f(int)"<<endl; }
};
int main() {
	Base b1;
	b1.f(); // Base f()
	//b1.f(1); //错,Base类中没有 f(int)函数
	D1 d1;
	d1.f(10); // D1 f(int)
	//d1.f(); //错,Base继承下来的f()被遮掩(shadow)
	d1.Base::f(); //要调用Base的f(),要加Base::
	return 0;
}