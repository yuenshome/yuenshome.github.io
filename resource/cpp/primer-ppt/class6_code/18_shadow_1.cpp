#include <iostream>
using namespace std;
struct Base {
	Base() :d(0) { }
	void base_show() { 
		cout << "Base d: " << d << endl; 
	}
	int d;
};
struct D1: Base  {
	D1(int i) :d(i) { }
	void d1_show() { //这里的 d 是哪个d? D1中的!
		cout << "D1 d: " << d << endl; 
	}
	void d1_show2() {
		cout<<"d1_show2: Base::d "<<Base::d<<endl;
	}
	int d;
};
int main() {
	Base b1;
	b1.base_show(); // 0
	cout << b1.d << endl; // 0
	D1 d1(10);
	d1.d1_show(); // 10 
	cout << d1.d << endl; // 10
	d1.d1_show2(); // 0
	cout << d1.Base::d << endl; // 0
	return 0;
}