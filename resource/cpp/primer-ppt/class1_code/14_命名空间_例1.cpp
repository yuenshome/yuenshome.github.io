#include <stdio.h>

namespace A {
	int x = 10, y = 20;
}
int x = 30;
int main() {
	//打开了A, A中的名字被“添加”到全局作用域
	{
		using namespace A;
		y++; //此处y是 A::y (ok)
		//x++; //此处的x可能是::x也可能是A::x (错误)
		::x++; //指明了是全局作用域的x (ok)
		A::x++; //指明了是 A::x (ok)
		int x = 31; //当前的局部变量x (ok)
		x++;  //此处的x 是上面的局部变量 31 (ok)
	}

	{
		using A::x;
		x++; //此处的x是 A::x (ok)
	}

	return 0;
}

