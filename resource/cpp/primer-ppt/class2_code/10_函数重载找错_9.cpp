#include <iostream>
using namespace std;
void f1(int a, int b) {}
void f1(int a) {}
int main() {
	void f1(int a); //在该作用域中声明函数
	//A中的f1和B中的f1构成重载吗?
	f1(10);
	//f1(10, 20); //错误
	return 0;
}

