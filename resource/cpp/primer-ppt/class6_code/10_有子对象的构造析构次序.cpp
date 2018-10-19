#include <iostream>
using namespace std;
class A { //有默认构造
public:
	A() { cout << "A构造" << endl; }
	~A() { cout << "A析构" << endl; }
};
class B { //没有默认构造
public:
	B(int i) { cout << "B构造" << endl; }
	~B() { cout << "B析构" << endl; }
};
class Base {
public:
	Base(int i) :b(set_b(i)), bb(A()) //这里bb(A())可以省略
					  {	cout << "Base:构造" << endl;	}
	~Base() { cout << "Base:析构" << endl; }
	int set_b(int i) { cout << "b1初始化" << endl; return i; }
private:
	int b;
	A bb;
};
class D1 :public Base {
public:
	D1(int i) :d(set_d(i)), Base(i), dd(B(i))
						{ cout << "D1:构造" << endl; }
	~D1() { cout << "D1:析构" << endl; }
	int set_d(int i) { cout << "d初始化" << endl; return i; }
private:
	int d;
	B dd;
};
int main() {
	D1 d1(1);
	cout << "---------" << endl;
	return 0;
}