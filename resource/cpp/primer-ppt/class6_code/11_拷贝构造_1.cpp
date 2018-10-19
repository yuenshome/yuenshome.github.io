#include <iostream>
using namespace std;
class Base {
public:
	Base(int _b) :b(_b), pb(&b) { }
	int b;
	int *pb;
};
class D1 :public Base {
public:
	D1(int _b, int _d) :Base(_b), d(_d), pd(&d) { }
	int d;
	int *pd;
};
int main() {
	D1 d1(1,2);
	D1 d2(d1); //øΩ±¥ππ‘Ï
	cout << d2.b << " " << d2.d << endl; // 1 2
	cout << d1.pb << " " << d1.pd << endl; 
	cout << d2.pb << " " << d2.pd << endl; //«≥øΩ±¥ 
	D1 d3(0, 0);
	d3 = d2;   //∏≥÷µ‘ÀÀ„∑˚
	cout << d3.b << " " << d3.d << endl; // 1 2
	cout << d2.pb << " " << d2.pd << endl;
	cout << d3.pb << " " << d3.pd << endl; //«≥øΩ±¥ 
	return 0;
}