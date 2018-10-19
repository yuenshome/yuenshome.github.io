#include <iostream>
using namespace std;

class Base{
public:
	virtual void f1()const{
		cout << "Base::f1()\n";
	}
	void f2()const{
		f1();
		cout << "Base::f2()\n";
	}
	virtual ~Base() = default;
};

class D1 : public Base{
public:
	virtual void f1()const override{
		cout << "D1::f1()\n";
	}
	virtual void f2()const{
		cout << "D1::f2()\n";
	}
};
int main(){
	Base *p = new D1;
	p->f2(); //输出结果是什么？
	delete p;
	return 0;
}