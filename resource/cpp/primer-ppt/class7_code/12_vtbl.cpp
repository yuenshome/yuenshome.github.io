#include <iostream>
using namespace std;
class Base { 
public:
	void func() { cout << "Base::func()\n"; }
	virtual void f1() { cout << "Base::f1()\n"; }
	virtual void f2() { cout << "Base::f2()\n"; }
};
class D1 :public Base {
public:
	void func1() { cout << "D1::func1()\n"; }
	virtual void f2()override { cout << "D1::f2()\n"; }
};
int main() {
	//class生成对象的size发生变化,32bit下是4
	//(没有数据成员时：以前是1,说明有虚函数时多了一些东西)
	cout << sizeof(Base) << "--" << sizeof(D1) << endl;
	Base b;
	int *p_vtbl = reinterpret_cast<int*>(&b); //重解释
	int *vtbl = reinterpret_cast<int*>(*p_vtbl);
	typedef void(*PFUN)();
	PFUN pf1 = reinterpret_cast<PFUN>(vtbl[0]);
	pf1();
	PFUN pf2 = reinterpret_cast<PFUN>(vtbl[1]);
	pf2();
	cout << "-----------------" << endl;
	D1 d;
	p_vtbl = reinterpret_cast<int*>(&d); //重解释
	vtbl = reinterpret_cast<int*>(*p_vtbl);
	pf1 = reinterpret_cast<PFUN>(vtbl[0]);
	pf1();
	pf2 = reinterpret_cast<PFUN>(vtbl[1]);
	pf2();
	return 0;
}