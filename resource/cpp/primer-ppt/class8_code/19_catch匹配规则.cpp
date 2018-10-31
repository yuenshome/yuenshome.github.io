#include <iostream>
using namespace std;
class A {
public:
	A() { cout << "A\n"; }
	~A() { cout << "~A\n"; }
	/*explicit*/ A(const A&) { cout << "拷贝构造\n"; }
	A(A&&) { cout << "移动构造\n"; }
	int a = 100;
};
void f1() {
	try {
		A a;
		throw a; // 有 explicit 的拷贝构造就不行了
				 //上面throw a;抛出异常表达式,会执行拷贝构造(或移动构造)
				 //构造出来的异常对象,位于一个特殊空间内(编译器管理)
				 //异常处理完毕后，该对象销毁。
	}
	catch (A e) {
		cout << "f1():----------\n";
	}
}
void f2() {
	try {
		A a;
		throw &a; //错误！抛出了指向局部对象的指针
	}
	catch (A* pe) {
		//此处pe所指向的对象已经销毁,结果是未定义的
		cout << "f2():----------\n";
	}
}
void f3() {
	try {
		throw 1;
	}
	catch (double e) { //不会匹配(没有隐式转换)
		cout << "f3(double):----\n";
	}
	catch (int e) { //精确匹配
		cout << "f3(int):-------\n";
	}
}
int main() {
	f1();
	f2();
	f3();
	return 0;
}