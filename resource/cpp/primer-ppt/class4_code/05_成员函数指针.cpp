#include <iostream>
using namespace std;

struct A {
	int add(int a, int b) { cout << "add()" << endl; return a + b; }
	static void show() { cout << "show()" << endl; }
};
void f1(int a, int b) { cout << "f1()" << endl; }
void f2(int a, int b) { cout << "f2()" << endl; }
typedef void(*P_FUN)(int, int);  //typedef 定义函数指针
using P_FUN1 = void(*)(int, int);//using 定义函数指针

typedef int(A::*P_ADD)(int, int);
using P_ADD1 = int(A::*)(int, int);
int main(){
	void(*p_fun)(int, int) = &f1; //普通函数指针,可省略&
	p_fun(2, 2); // f1()
	p_fun = &f2;
	p_fun(1, 2); // f2()

	P_FUN p1 = &f1;
	p1(1, 1);  // f1()
	p1 = &f2;
	p1(2, 2);  // f2()

	P_FUN1 p2 = &f1;
	p2(1, 1);  // f1()
	p2 = &f2;
	p2(2, 2);  // f2()

	//非静态成员函数指针，区别于普通函数，主要是由于隐含的this指针
	int(A::*p_add)(int, int) = &A::add; //非静态成员函数指针
	A a;
	cout << (a.*p_add)(0,2) << endl;     //.* 必须有对象,注意括号 运行结果add() 2
	A* pa = new A;
	cout << (pa->*p_add)(1, 2) << endl;  //->* //运行结果add() 3

	auto p_add1 = &A::add; 
	cout << (a.*p_add1)(2, 2) << endl;	//运行结果add() 4

	P_ADD p11 = &A::add;
	cout << (a.*p11)(3, 2) << endl; //运行结果add() 5
	P_ADD1 p22 = &A::add;
	cout << (pa->*p22)(2, 4) << endl; //运行结果add() 6

	delete pa;

	void(*p_show)() = &A::show; //静态成员函数指针 与普通函数指针类似
	p_show();
	return 0;
}