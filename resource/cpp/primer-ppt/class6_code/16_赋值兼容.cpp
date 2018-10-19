#include <iostream>
using namespace std;
class Base {
public:
	Base() = default;
	Base(const Base& other) :b(other.b) 
			{ cout << "Base copy" << endl; }
	void f() { cout << "Base f()" << endl; }
private:
	int b;
};
class D1:public Base  {
public:
	D1() = default;
	D1(const D1& other) :Base(other), d(other.d) 
			{ cout << "D1 copy" << endl; }
	void f() { cout << "D1 f()" << endl; }
private:
	int d;
};
int main() {
	//1.基类对象 和 派生类对象 之间 的拷贝与赋值
	//只能将派生类对象拷贝或赋值给基类对象,反之不行
	D1 d1;
	Base b1(d1);  //ok,显式初始化,只执行 Base拷贝构造
	Base b2 = d1; //ok,拷贝初始化,只执行 Base拷贝构造
	b2 = d1;   //ok,可以将派生类对象赋值给基类对象
	//d1 = b1; //error,不能将基类对象赋值给派生类对象
	//[上面这样的做法虽然可以，但是一般不这样用]
	b1.f(); //Base f()
	d1.f(); //D1 f()

	//2.基类的引用：用派生类对象来初始化
	Base &rb1 = d1; //ok,基类引用 用派生类对象初始化
	cout << &rb1 << &d1 << endl; //地址一样，说明没有临时量
	//D1 &rd1 = b1; //error,派生类引用不能用基类对象初始化
	rb1.f(); //Base f(),rb1是Base的引用，所以调用Base的f

	//3.基类的指针：指向派生类对象
	Base *pb1 = &d1;//ok,基类指针 指向派生类对象
	//D1 *pd1 = &b1;//error,派生类指针不能指向基类对象
	pb1->f(); //Base f(),pb1是Base的指针，所以调用Base的f
	return 0;
}