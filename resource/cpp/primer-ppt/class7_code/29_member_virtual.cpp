#include <iostream>
using namespace std;
class Base {
public:
	void f() {
		cout << "Base f()" << endl;
		cout <<"typeid: "<<typeid(*this).name()<<endl;  //Derived
		//可以做一些虚函数调用前的准备工作
		vf();
		//可以做一些虚函数调用后的善后工作
	}
	virtual ~Base(){}
private:
	virtual void vf() { cout<<"Base vf(): "<<i<<endl; }
	//假如派生类中需要调用父类的虚函数,可以将权限改为protected/public
	int i = 100;
};
class Derived :public Base {
private:
	virtual void vf()override { cout<<"Derived vf(): "<<j<< endl;}
	int j = 200;
};
int main() {
	Derived d;
	d.f();
	cout << "---------\n";
	Base *p = &d;
	p->f();
	return 0;
}