#include <iostream>
#include <typeinfo>
using namespace std;
class Base {
public:
	virtual ~Base() = default;
private:
	int b = 1;
};
class Derived :public Base {
private:
	int d1 = 2;
};
int main() {
	cout << typeid(int).name() << endl;			//int
	cout << typeid(double).name() << endl;		//double
	cout << typeid(char *).name() << endl;		//char *
	//顶层const不识别
	cout << typeid(char * const).name() << endl;//char *
	//底层const识别
	cout << typeid(const char *).name() << endl;//char const *
	cout << typeid(Base).name() << endl;	//class Base
	cout << typeid(Derived).name() << endl;	//class Derived

	Derived d;
	Base *pb = &d;
	cout << typeid(pb).name() << endl; //class Base * 指针看不出真实类型
	cout << typeid(*pb).name() << endl; //class Derived
	cout << typeid(d).name() << endl; //class Derived
	Base &rb = d;
	cout << typeid(rb).name() << endl; //class Derived

	Base *pb1 = &d;
	Derived *pd1 = &d;
	//1.判断两个对象的真实类型是否一致
	if (typeid(*pb1) == typeid(*pd1)) //注意 加*
		cout << "类型一致" << endl;
	else
		cout << "类型不一致" << endl;
	//2.判断某对象的真实类型是否某种类类型
	if(typeid(*pb1)==typeid(Derived))
		cout << "是Derived类对象" << endl;
	else
		cout << "不是Derived类对象" << endl;
	return 0;
}