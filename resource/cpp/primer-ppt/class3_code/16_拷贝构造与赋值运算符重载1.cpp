#include <iostream>
#include <string>
using namespace std;

class A {
public:
	A(int n = 0) :num(n) {}
	A(const A &other):num(other.num) { }
private:
	int num; 
};
int main() {
	A a1;
	A a2(a1);  //直接初始化
	A a3 = a1; //拷贝初始化
	const A a4;
	A a5 = a4; //参数没有const,就错误
	return 0;
}