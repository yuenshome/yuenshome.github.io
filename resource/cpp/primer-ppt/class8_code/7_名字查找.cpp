#include <iostream>
using namespace std;
struct A { int ai=1;};
struct B { int bi=2;};
class Base1 {
public:
	void fun() { cout << "Base1::fun()\n"; }
	void fun_1(int i) { cout << "Base1::fun_1(int)\n"; }
	void print() { cout << "Base::print()\n"; }
	int i=3;
};
class Base2 {
public:
	void fun() { cout << "Base2::fun()\n"; }
	void fun_1(const char*) { cout << "Base2::fun_1(const char*)\n"; }
	void print(int i) { cout << "Base2::print(int)\n"; }
	int j=4;
};
class D1 :public Base1 {
public:
	void fun_2(int) {}
	void fun_2(const char*) {}
	A a;
};
class Ok :public D1, public Base2 {
public:
	void print(const char*) { cout << "Ok::Print(const char*)\n"; }
	B b;
};
int main() {
	Ok ok;
	//ok.fun(); //二义性错误
	//Ok的两个基类中：都有 fun()函数

	//ok.fun_1(1);    二义性错误
	//ok.fun_1("abc");二义性错误
	//虽然两个基类中的fun_1参数列表不同,但是不构成重载
	
	ok.fun_2(1);
	ok.fun_2("abc");
	//只有在同一个类中同名不同参数列表的才构成重载

	//ok.print();
	//ok.print(1); //基类中的print被派生类的shadow了。
	ok.print("abc");
	ok.Base2::print(1); //显式指明Base2中的print

	Base2 *pb = &ok; //基类指针指向派生类
	pb->fun();
	pb->fun_1("abc");
	pb->print(1);
	//pb->print("abc"); //错
	//pb只能调用其静态类型中有的成员。
	return 0;
}
