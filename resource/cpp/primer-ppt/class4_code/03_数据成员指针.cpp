#include <iostream>
using namespace std;

struct A {
	A(double n1=0.0,int n2=0):num1(n1),num2(n2){}
	double num1;
	int num2;
};

int main(){
	int i = 10;
	int *pi = &i; //指针 指向普通变量

	A a(0.1, 1), b(1.1, 2);
	int *pa = &a.num2;
	cout << a.num2 << endl; //1

	int A::*p = &A::num2; //指针 指向成员变量
	cout << a.*p << b.*p << endl; // 1 2

	//指向数据成员的指针，是针对类的，是一个偏移量
	printf("%p\n", p); // 00000008 

	A* pA = new A(3.1, 3);
	cout << pA->*p << endl; //3

	auto p_double = &A::num1;
	cout << typeid(p_double).name() << endl; //double A::*

	delete pA;
	return 0;
}