#include <iostream>
using namespace std;
class Base {
public:
	Base() { cout << "Base:默认构造" << endl; }
	~Base() { cout << "Base:析构" << endl; }
};
class D1 :public Base {
public:
	D1() { cout << "D1:默认构造" << endl; }
	~D1() { cout << "D1:析构" << endl; }
};
class D2 :public D1 {
public:
	D2() { cout << "D2:默认构造" << endl; }
	~D2() { cout << "D2:析构" << endl; }
};

int main() {
	D2 d2;
	cout << "---------" << endl;
	return 0;
}