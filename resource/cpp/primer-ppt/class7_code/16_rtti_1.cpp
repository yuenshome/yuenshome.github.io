#include <iostream>
using namespace std;
class Base {
public:   //基类至少有一个虚函数
	virtual ~Base() = default; 
private:
	int b;
};
class Derived :public Base {
private:
	int d;
};

int main() {
	Derived d1;
	Base *pd1 = &d1;
	//将基类指针指向的对象，转为派生类指针指向的对象
	Derived *p1 = dynamic_cast<Derived*>(pd1); //成功
	if (p1 != nullptr)
		cout << "转换成功" << endl;
	else
		cout << "转换失败" << endl;

	Base b1;
	Base *pb1 = &b1;
	Derived *p2 = dynamic_cast<Derived*>(pb1); //失败
	if (p2 != nullptr)
		cout << "转换成功" << endl;
	else
		cout << "转换失败" << endl;

	return 0;
}