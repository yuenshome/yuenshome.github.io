#include <stdio.h>

namespace A {
	int x = 10;
	void print() { printf("A::x=%d\n", x); }
}
namespace B {
	int x = 20;
	void print() { printf("B::x=%d\n", x); }
}

int main() {
	{
		//打开A::x --> x 打开 A::print --> print
		using A::x; using A::print;
		printf("x=%d\n", x); //此时的x 是 A::x
		print();			 //此时的print是 A::print
	}
	{
		//全部打开B
		using namespace B;
		printf("x=%d\n", x); //此时的x 是 B::x
		print();			 //此时的print是 B::print
	}
	
	using A::x; using A::print; //using声明更加强力
	using namespace B;   
	printf("x=%d\n", x); //此时的x 是 A::x
	print();			 //此时的print是 A::print
	return 0;
}

