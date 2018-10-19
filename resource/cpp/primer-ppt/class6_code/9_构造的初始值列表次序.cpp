#include <iostream>
using namespace std;
class Base {
public:
	Base(int i) :b1(set_b1(i)), b2(set_b2(i))
					  {	cout << "Base:默认构造" << endl;	}
	~Base() { cout << "Base:析构" << endl; }
	int set_b1(int i) { cout << "b1初始化" << endl; return i; }
	int set_b2(int i) { cout << "b2初始化" << endl; return i; }
private:
	int b1;
	int b2;
};
class D1 :public Base {
public:
	D1(int i) :d1_2(set_d1_2(i)), d1_1(set_d1_1(i)), Base(i)
						{ cout << "D1:默认构造" << endl; }
	~D1() { cout << "D1:析构" << endl; }
	int set_d1_1(int i) { cout << "d1_1初始化" << endl; return i; }
	int set_d1_2(int i) { cout << "d1_2初始化" << endl; return i; }
private:
	int d1_1;
	int d1_2;
};
class D2 :public D1 {
public:
	D2(int i) :D1(i), d2_1(set_d2_1(i)), d2_2(set_d2_2(i)) 
								{ cout << "D2:默认构造" << endl; }
	~D2() { cout << "D2:析构" << endl; }
	int set_d2_1(int i) { cout << "d2_1初始化" << endl; return i; }
	int set_d2_2(int i) { cout << "d2_2初始化" << endl; return i; }
private:
	int d2_1;
	int d2_2;
};
int main() {
	D2 d2(1);
	cout << "---------" << endl;
	return 0;
}