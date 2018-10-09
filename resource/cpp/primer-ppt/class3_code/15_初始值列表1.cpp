#include <iostream>
#include <string.h>
using namespace std;
class A {
public:
	A(int n, double f) { num = n; fd = f; }
private:
	int num;
	double fd;
};
class B {
public:
	              //构造函数的初始值列表
	B(int n, double f) :num(n), fd(f) {}
private:
	int num;
	double fd;
};
int main() {
	A a(10, 1.2);
	B b(10, 1.2);
	return 0;
}