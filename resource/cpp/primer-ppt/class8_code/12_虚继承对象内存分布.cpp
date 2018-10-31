#include <iostream>
using namespace std;

class Base {
public:
	int x=1;
};
class D1 :virtual public Base {
public:
	int i=2;
};
class D2 :virtual public Base {
public:
	int j=3;
};
class Ok :public D1, public D2 {
public:
	int k=4;
};
int main() {
	cout << "Base尺寸: " << sizeof(Base) << endl; //4
	cout << "  D1尺寸: " << sizeof(D1) << endl;   //12
	cout << "  D2尺寸: " << sizeof(D2) << endl;   //12
	cout << "  Ok尺寸: " << sizeof(Ok) << endl;   //24
	cout << "----------------\n";
	D1 d1;
	cout << "d1地址: " << &d1 << endl;
	cout << " i地址: " << &d1.i << endl;
	cout << " x地址: " << &d1.x << endl;
	//观察d1的最前面4个字节, 指向虚基类表的指针 vbptr
	int *p = (int*)(&d1);
	int *p1 = (int*)(*p);
	cout << p1[0] << endl;
	cout << p1[1] << endl; //8 (偏移量)
	cout << "----------------\n";
	Ok ok;
	cout << "  ok地址: " << &ok << endl;
	Base *pb = &ok;
	cout << "Base地址: " << pb << endl;
	D1 *pd1 = &ok;
	cout << "  D1地址: " << pd1 << endl;
	D2 *pd2 = &ok;
	cout << "  D2地址: " << pd2 << endl;
	cout << " x地址: " << &ok.x << endl;
	cout << " i地址: " << &ok.i << endl;
	cout << " j地址: " << &ok.j << endl;
	cout << " k地址: " << &ok.k << endl;
	return 0;
}


