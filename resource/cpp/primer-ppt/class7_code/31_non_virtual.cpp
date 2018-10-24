#include <iostream>
using namespace std;

class Base {
public:
	virtual ~Base() = default;
	void f1() { 
		cout << "Base f1()\n";
	}
};
class D1 :public Base {
public:
	void f1() {
		cout << "D1 f1():\n"; 
	}
};

int main() {
	D1 d;
	Base *pb = &d;
	pb->f1(); //Base f1()
	D1 *pd = &d;
	pd->f1(); //D1 f1()
	//非虚函数调用:
	//通过不同的指针类型调用结果不同！
	//(语法没有错误,但是对用户造成伤害！)
	//虚函数不会如此！
	return 0;
}

