#include <stdio.h>

namespace A {
	int x = 10, y = 20;
}
namespace B {
	int x = 30;
}
int main() {
	{
		using namespace A;
		using namespace B;
		y++; // ok
	}

	{
		using namespace A;
		using namespace B;
		//x++; //A B 中都有x, 二义性 (错误)
	}
	{
		using namespace A;
		int x; //定义局部变量 (ok)
	}

	{
		using A::x;
		//int x; //错误
	}

	return 0;
}

