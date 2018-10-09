#include <stdio.h>

namespace A {
	int x = 10;
	void print() { printf("A::x=%d\n", x); }
}
namespace B {
	int x = 20;
	void print() { printf("B::x=%d\n", x); }
}

int x = 30;
void print() { printf("x=%d\n", x); }
int main() {
	int x = 40;
	printf("main:x=%d\n", x); //局部变量x 40
	printf("全局:x=%d\n", ::x); //全局的x 30
	printf("命名空间A::x=%d\n", A::x);//A中的x 10
	printf("命名空间B::x=%d\n", B::x);//B中的x 20
	A::print(); //10
	B::print(); //20
	print();    //30
	return 0;
}

