#include <iostream>
using namespace std;
int main() {
	int a; //a 的含义
	a = 10;
	int b;
	b = a + 20; //临时量
	b = a + 20.1;
	int *p = &b;

	//内置类型的临时量有const属性
	//(a + 2) = 10; //为啥不行？
	int &ra = a; 
	//int &ra1 = a + 2; //为啥不行？
	const int &cra = a + 2; //ok
	int &&rra = a + 2; //右值引用

	++++a; //ok
	//++a++; //不行

	return 0;
}