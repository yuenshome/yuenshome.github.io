#include <iostream>
using namespace std;
//通过函数重载方式的swap:
//虽然函数实现方式雷同，
//但是：有一种类型就要写一个函数
void swap(int& a, int& b) {
	int tmp = a;
	a = b;
	b = tmp;
}
void swap(double& a, double& b) {
	double tmp = a;
	a = b;
	b = tmp;
}
//用函数模板，只写一套代码解决问题
template<typename T>
void swap(T& a, T& b) {
	T tmp = a;
	a = b;
	b = tmp;
}
int main() {
	int a = 10, b = 20;
	swap(a, b);
	double a1 = 1.0, b1 = 2.0;
	swap(a1, b1);
	const char * c1 = "abc", *c2 = "123";
	::swap(c1, c2);
	::swap<const char*>(c1, c2);
	char cc1 = '1', cc2 = '2';
	::swap(cc1, cc2);
	::swap<char>(cc1, cc2);
	return 0;
}